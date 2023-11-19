using System.Net;
using System.Net.Sockets;
using System.Text.Json;
using lvfucs.Core.Models;
using lvfucs.Core.Utilities.Converter;
using lvfucs.Core.Utilities.Producer;

namespace lvfucs.Core.Utilities
{
	public class JsonData
	{
		public static void GenerateJson(string filePath)
		{
            // create empty LunaJson object
            LunaJson lJson = new LunaJson();

			// remove if existing file
			if (File.Exists(filePath))
			{
                File.Delete(filePath);
            }
			else
			{
				Console.WriteLine($"Error deleting existing file: {filePath}");
				Environment.Exit(1);
            }

            // Get hostname
            string hostname = Dns.GetHostName();

            // Get public IP address
            string publicIP;
            using (var udpClient = new UdpClient())
            {
                udpClient.Connect(IPAddress.Parse("8.8.8.8"), 53);
                var localEndPoint = (IPEndPoint)udpClient.Client.LocalEndPoint!;
                publicIP = localEndPoint.Address.ToString();
            }

            // Read server.uuid file
            string serverUid;
            string uuidFilePath = "/app/lunavpn/server.uuid";

            if (File.Exists(uuidFilePath))
            {
                using (StreamReader uuidFile = new StreamReader(uuidFilePath))
                {
                    serverUid = uuidFile.ReadLine()!;
                }
            }
            else
            {
                serverUid = string.Empty;
            }

            // Read server.type file
            string serverType = string.Empty;
            string typeFilePath = "/app/lunavpn/server.type";

            if (File.Exists(typeFilePath))
            {
                using (StreamReader typeFile = new StreamReader(typeFilePath))
                {
                    serverType = (typeFile.ReadLine() ?? string.Empty).Trim();
                }
            }
            else
            {
                serverType = string.Empty;
            }

            // assign the early components of the VPS
            lJson.PublicIP = publicIP;
            lJson.ServerName = hostname;
            lJson.ServerName = serverType;
            lJson.ServerUUID = serverUid;

            // wireguard peers && squidproxy
            switch (serverType)
            {
                case "squid-proxy":
                    // set the squidCredsFilePath (this is predetermined by the scripts used to install it)
                    string squidCredsFilePath = "/app/squid/squid.creds";

                    // map the squid credentials
                    var mappedSquidCreds = GenProd.ReadSquidCredentials(squidCredsFilePathIn: squidCredsFilePath);

                    // append mappedSquidCreds to lJson object
                    lJson.SquidCreds = mappedSquidCreds;
                    break;

                case "wireguard":
                    string wgPeers = "/app/wg/config";
                    if (Directory.Exists(wgPeers))
                    {
                        List<string> peerNames = new List<string>();

                        // iterate over the contents of the directory wgPeers
                        foreach (var entry in Directory.EnumerateDirectories(wgPeers))
                        {
                            // check if the directory meets naming convention
                            if (Directory.Exists(entry))
                            {
                                string folderName = Path.GetFileName(entry);

                                // check if the folder name matches the convention "peerX"
                                if (folderName.Length > 4 && folderName.StartsWith("peer"))
                                {
                                    string remainingDigits = folderName.Substring(4);

                                    // check if the remaining part is a valid integer
                                    if (remainingDigits.All(char.IsDigit))
                                    {
                                        peerNames.Add(folderName);
                                    }
                                }
                            }
                        }

                        // map the proxyPeers values
                        var mappedProxyPeers = GenProd.GenerateProxyPeers(wgPeersIn: wgPeers, peerNamesIn: peerNames);

                        // append mappedProxyPeers to lJson object
                        lJson.ProxyPeers = mappedProxyPeers;
                    }
                    break;

                case "squid-wg":
                    break;

                default:
                    break;
            }

            // Serialize the object to JSON
            string jsonString = JsonSerializer.Serialize(lJson);

            // Write the JSON string to the file
            try
            {
                File.WriteAllText(filePath, jsonString);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Unable to write to file: {filePath}");
                Console.WriteLine($"Error: {ex.Message}");
            }
        }
    }
}

