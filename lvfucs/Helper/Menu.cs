namespace lvfucs.Helper
{
	public class Menu
	{
		public static void PrintHelp()
		{
            Console.WriteLine("###################################################");
            Console.WriteLine("#              LunaVPN fu vX.X.X                  #");
            Console.WriteLine("###################################################\n");
            Console.WriteLine("Welcome to LunaVPN fu - Your Functioning Unit for LunaVPN needs!");
            Console.WriteLine("Copyright © RePass Cloud Pty Ltd 2023\n");
            Console.WriteLine("Usage: LunaVPN_fu [-v] | [-h] | [-g <path_to_output.json>]\n");
            Console.WriteLine("  -v|--version                                Display version information");
            Console.WriteLine("  -h|--help                                   Display help");
            Console.WriteLine("  -gj|--generate-json <path_to_output.json>   Generate the JSON file");
            Console.WriteLine("  -gs|--generate-squid <path_to_output.json>  Generate the Squid Proxy credentials\n");
            Console.WriteLine("Usage: LunaVPN_fu [-u <URL> -h <HEADER> -d <path_to_input.json>]\n");
            Console.WriteLine("HTTP POST request");
            Console.WriteLine("  -u|--url <URL>                        Specify the URL for HTTP POST request");
            Console.WriteLine("  -h|-header <HEADER>                   Specify the header for HTTP POST request");
            Console.WriteLine("  -d|--data-file <path_to_input.json>   Specify the data file for HTTP POST request\n");
        }

        public static void Version()
        {
            Console.WriteLine("###################################################");
            Console.WriteLine("#             LunaVPN fu vX.X.X C#                 #");
            Console.WriteLine("###################################################\n");
            Console.WriteLine("Welcome to LunaVPN fu - Your Functioning Unit for networking needs!");
            Console.WriteLine("C# Edition");
            Console.WriteLine("Copyright © RePass Cloud Pty Ltd 2023\n\n");
            Console.WriteLine("LunaVPN fu vX.X.X");
        }
    }
}

