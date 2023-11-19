using System.Text.Json.Serialization;

namespace lvfucs.Core.Models
{
	public class SquidCreds
	{
        [JsonPropertyName("squid_user")]
        public string? SquidUser { get; set; }

        [JsonPropertyName("squid_pass")]
        public string? SquidPass { get; set; }
    }
}