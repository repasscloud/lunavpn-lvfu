namespace lvfucs.Core.Utilities.Converter
{
	public class Base64Coder
	{
        /// <summary>
        /// Encodes a file to base64 string
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        public static string Encode(string filePath)
        {
            try
            {
                byte[] fileBytes = File.ReadAllBytes(filePath);
                return Convert.ToBase64String(fileBytes);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error encoding file: {ex.Message}");
                return string.Empty;
            }
        }

        /// <summary>
        /// Decodes a base64 string to decoded bytes
        /// </summary>
        /// <param name="b64string"></param>
        /// <returns></returns>
        public static string Decode(string b64string)
        {
            try
            {
                byte[] decodedBytes = Convert.FromBase64String(b64string);
                return System.Text.Encoding.UTF8.GetString(decodedBytes);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error decoding Base64 string: {ex.Message}");
                return string.Empty;
            }
        }
    }
}

