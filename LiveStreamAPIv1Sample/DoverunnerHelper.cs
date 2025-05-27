using Google.Protobuf.Collections;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;

namespace Doverunner
{
    class DoverunnerHelper
    {
        public class SecretPayload
        {
            public IList<Dictionary<string, object>>? encryptionKeys { get; set; }
        }
        public static string Base64ToHex(string strInput)
        {
            try
            {
                var bytes = Convert.FromBase64String(strInput);
                var hex = BitConverter.ToString(bytes);
                return hex.Replace("-", "").ToLower();
            }
            catch (Exception)
            {
                return "-1";
            }
        }
        public static string GetSecretKeyDataFromDoverunnerKMS(string kms_url, string content_id,
            RepeatedField<string> widevineMuxStreams, RepeatedField<string> playreadyMuxStreams, RepeatedField<string> fairplayMuxStreams)
        {
            // Get the packaging information from the KMS Server
            CpixClientWrapper doverunnerCpixClientWrapper = new CpixClientWrapper(kms_url);
            ContentPackagingInfo contentPackagingInfo = doverunnerCpixClientWrapper.GetContentKeyInfoFromDoverunnerKMS(content_id, DrmType.WIDEVINE | DrmType.PLAYREADY | DrmType.FAIRPLAY, EncryptionScheme.CENC, TrackType.ALL_TRACKS, 0);

            // Set the string data to meet Google Cloud Secret Key format
            var widevineDict = new Dictionary<string, object>();
            widevineDict["keyId"] = contentPackagingInfo.DrmInfos[0].KeyId;
            widevineDict["key"] = Base64ToHex(contentPackagingInfo.DrmInfos[0].Key);
            var widevineHSD = contentPackagingInfo.DrmInfos[0].WidevineHlsSignalingDataMedia;
            string widevineKeyUri = "";
            if(widevineHSD.Contains("URI=\""))
                widevineKeyUri = widevineHSD.Split(new[] { "URI=\"" }, StringSplitOptions.None)[1].Split('"')[0];
            widevineDict["keyUri"] = widevineKeyUri; // Currently, pssh input does not work in LiveStreamAPI v1.
            widevineDict["matchers"] = new List<object> { new Dictionary<string, object> { ["muxStreams"] = widevineMuxStreams.ToList() } };

            var playreadyDict = new Dictionary<string, object>();
            playreadyDict["keyId"] = contentPackagingInfo.DrmInfos[0].KeyId;
            playreadyDict["key"] = Base64ToHex(contentPackagingInfo.DrmInfos[0].Key);
            var playreadyHSD = contentPackagingInfo.DrmInfos[0].PlayreadyHlsSignalingDataMedia;
            string playreadyKeyUri = "";
            if (playreadyHSD.Contains("URI=\""))
                playreadyKeyUri = playreadyHSD.Split(new[] { "URI=\"" }, StringSplitOptions.None)[1].Split('"')[0];
            playreadyDict["keyUri"] = playreadyKeyUri; // Currently, pssh input does not work in LiveStreamAPI v1.
            playreadyDict["matchers"] =  new List<object> { new Dictionary<string, object> { ["muxStreams"] = playreadyMuxStreams.ToList() } };

            var fairplayDict = new Dictionary<string, object>();
            fairplayDict["keyId"] = contentPackagingInfo.DrmInfos[0].KeyId;
            fairplayDict["key"] = Base64ToHex(contentPackagingInfo.DrmInfos[0].Key);
            fairplayDict["iv"] = Base64ToHex(contentPackagingInfo.DrmInfos[0].Iv);
            fairplayDict["keyUri"] = contentPackagingInfo.DrmInfos[0].FairplayHlsKeyUri;
            fairplayDict["matchers"] =  new List<object> { new Dictionary<string, object> { ["muxStreams"] = fairplayMuxStreams.ToList() } };

            var jsonSecretPayload = new SecretPayload
            {
                encryptionKeys = new List<Dictionary<string, object>>() { widevineDict, playreadyDict, fairplayDict }
            };

            return JsonSerializer.Serialize(jsonSecretPayload);
        }
    }
}
