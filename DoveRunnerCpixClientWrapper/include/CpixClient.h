#pragma once

#include <string>
#include <map>
#include <vector>
#include "CpixClientException.h"

namespace doverunner
{
	enum EncryptionScheme {
		NONE,
		CENC,
		CBC1,
		CENS,
		CBCS
	};

	enum DrmType {
		WIDEVINE = (1 << 0),
		PLAYREADY = (1 << 1),
		FAIRPLAY = (1 << 2),
		WISEPLAY = (1 << 3),
		NCG = (1 << 4),
		NCGHLS_AES128 = (1 << 5),
		NCGHLS_SAMPLEAES = (1 << 6),
		AES128 = (1 << 7),
		SAMPLEAES = (1 << 8)
	};

	enum TrackType {
		ALL_TRACKS,
		AUDIO = (1 << 0),
		SD = (1 << 1),
		HD = (1 << 2),
		UHD1 = (1 << 3),
		UHD2 = (1 << 4)
	};

	inline const char* TrackTypeToString(TrackType t) {
		switch (t) 
		{
			case AUDIO: return "AUDIO";
			case SD: return "SD";
			case HD: return "HD";
			case UHD1: return "UHD1";
			case UHD2: return "UHD2";
			default: return "Unknown";
		}
	}

	inline DrmType operator&(DrmType type_a, DrmType type_b) {
		return static_cast<DrmType>(static_cast<int>(type_a) & static_cast<int>(type_b));
	}

	inline DrmType operator|(DrmType type_a, DrmType type_b) {
		return static_cast<DrmType>(static_cast<int>(type_a) | static_cast<int>(type_b));
	}

	inline DrmType operator^(DrmType type_a, DrmType type_b) {
		return static_cast<DrmType>(static_cast<int>(type_a) ^ static_cast<int>(type_b));
	}

	inline DrmType& operator&=(DrmType& type_a, DrmType type_b) {
		return (DrmType&)((int&)(type_a) &= (int)(type_b));
	}

	inline DrmType& operator|=(DrmType& type_a, DrmType type_b) {
		return (DrmType&)((int&)(type_a) |= (int)(type_b));
	}

	inline DrmType& operator^=(DrmType& type_a, DrmType type_b) {
		return (DrmType&)((int&)(type_a) ^= (int)(type_b));
	}

	inline TrackType operator&(TrackType type_a, TrackType type_b) {
		return static_cast<TrackType>(static_cast<int>(type_a) & static_cast<int>(type_b));
	}

	inline TrackType operator|(TrackType type_a, TrackType type_b) {
		return static_cast<TrackType>(static_cast<int>(type_a) | static_cast<int>(type_b));
	}

	inline TrackType operator^(TrackType type_a, TrackType type_b) {
		return static_cast<TrackType>(static_cast<int>(type_a) ^ static_cast<int>(type_b));
	}

	inline TrackType& operator&=(TrackType& type_a, TrackType type_b) {
		return (TrackType&)((int&)(type_a) &= (int)(type_b));
	}

	inline TrackType& operator|=(TrackType& type_a, TrackType type_b) {
		return (TrackType&)((int&)(type_a) |= (int)(type_b));
	}

	inline TrackType& operator^=(TrackType& type_a, TrackType type_b) {
		return (TrackType&)((int&)(type_a) ^= (int)(type_b));
	}

	struct MultiDrmInfo
	{
		std::string trackType;
		std::string key;
		std::string keyId;
		std::string iv;
		std::string periodIndex;
		std::string widevinePSSH;
		std::string widevinePSSHpayload;
		std::string widevineHlsSignalingDataMaster;
		std::string widevineHlsSignalingDataMedia;
		std::string playreadyPSSH;
		std::string playreadyPSSHpayload;
		std::string playreadySmoothStreamingData;
		std::string playreadyHlsSignalingDataMaster;
		std::string playreadyHlsSignalingDataMedia;
		std::string fairplayHlsKeyUri;
		std::string fairplayHlsSignalingDataMaster;
		std::string fairplayHlsSignalingDataMedia;
		std::string wiseplayPSSH;
		std::string wiseplayPSSHpayload;
		std::string wiseplayHlsSignalingDataMaster;
		std::string wiseplayHlsSignalingDataMedia;
		std::string ncgCek;
		std::string ncghlsAes128KeyUri;
		std::string ncghlsAes128HlsSignalingDataMaster;
		std::string ncghlsAes128HlsSignalingDataMedia;
		std::string ncghlsSampleAesKeyUri;
		std::string ncghlsSampleAesHlsSignalingDataMaster;
		std::string ncghlsSampleAesHlsSignalingDataMedia;
		std::string aes128KeyUri;
		std::string aes128HlsSignalingDataMaster;
		std::string aes128HlsSignalingDataMedia;
		std::string sampleAesKeyUri;
		std::string sampleAesHlsSignalingDataMaster;
		std::string sampleAesHlsSignalingDataMedia;
	};

	struct ContentPackagingInfo
	{
		std::string contentId;
		std::vector<MultiDrmInfo> multiDrmInfos;
	};

	class CpixClient
	{
	private:
		std::string _kmsUrl;

		int _lastRequestStatus;
		std::string _lastRequestRowData;
		std::string _lastResponseRowData;
	
		std::map<std::string, std::string> _keyMap;

		std::string GetRequestData(std::string contentId, DrmType drmType, EncryptionScheme encryptionScheme, TrackType trackType, long periodIndex);
		ContentPackagingInfo ParseResponse(const std::string& responseBody);

	public:
		/**
		* Constructor.
		*
		* @param kmsUrl					KMS Server URL. The end of the KMS URL should contain an enc-token.
		*/
		CpixClient(std::string kmsUrl);
		~CpixClient();


		/**
		* Get the last request status value.
		*/
		int GetLastRequestStatus() const {
			return _lastRequestStatus;
		}

		/**
		* Get the last request data.
		*/
		std::string GetLastRequestRowData() const {
			return _lastRequestRowData;
		}

		/**
		* Get the last response data.
		*/
		std::string GetLastResponseRowData() const {
			return _lastResponseRowData;
		}

		/**
		* Receive packaging information from KMS server.
		*
		* @param contentId				Content id
		* @param drmType				DRM type. (e.g. WIDEVINE|PLAYREADY|FAIRPLAY)
		* @param encryptionScheme		Encryption scheme. (e.g. CENC, CBCS, etc)
		* @param trackType				Track type for multi-key packaging. (e.g. SD|HD|AUDIO)
		*								For single-key packaging, it should be ALL_TRACKS.
		* @param periodIndex			Period index for key rotation. 
										Setting a value greater than 0 enables key rotation.
		*/
		ContentPackagingInfo GetContentKeyInfoFromDoveRunnerKMS(const std::string contentId, DrmType drmType, EncryptionScheme encryptionScheme = NONE, TrackType trackType = ALL_TRACKS, long periodIndex = 0);
	};
}

