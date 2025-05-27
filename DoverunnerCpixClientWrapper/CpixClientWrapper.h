#pragma once

#include "CpixClient.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Doverunner {
	public enum class EncryptionScheme {
		NONE,
		CENC,
		CBC1,
		CENS,
		CBCS
	};

	public enum class DrmType {
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

	public enum class TrackType {
		ALL_TRACKS,
		AUDIO = (1 << 0),
		SD = (1 << 1),
		HD = (1 << 2),
		UHD1 = (1 << 3),
		UHD2 = (1 << 4)
	};

	public value struct MultiDrmInfo {
		String^ TrackType;
		String^ Key;
		String^ KeyId;
		String^ Iv;
		String^ PeriodIndex;
		String^ WidevinePSSH;
		String^ WidevinePSSHpayload;
		String^ WidevineHlsSignalingDataMaster;
		String^ WidevineHlsSignalingDataMedia;
		String^ PlayreadyPSSH;
		String^ PlayreadyPSSHpayload;
		String^ PlayreadySmoothStreamingData;
		String^ PlayreadyHlsSignalingDataMaster;
		String^ PlayreadyHlsSignalingDataMedia;
		String^ FairplayHlsKeyUri;
		String^ FairplayHlsSignalingDataMaster;
		String^ FairplayHlsSignalingDataMedia;
		String^ WiseplayPSSH;
		String^ WiseplayPSSHpayload;
		String^ WiseplayHlsSignalingDataMaster;
		String^ WiseplayHlsSignalingDataMedia;
		String^ NcgCek;
		String^ NcghlsAes128KeyUri;
		String^ NcghlsAes128HlsSignalingDataMaster;
		String^ NcghlsAes128HlsSignalingDataMedia;
		String^ NcghlsSampleAesKeyUri;
		String^ NcghlsSampleAesHlsSignalingDataMaster;
		String^ NcghlsSampleAesHlsSignalingDataMedia;
		String^ Aes128KeyUri;
		String^ Aes128HlsSignalingDataMaster;
		String^ Aes128HlsSignalingDataMedia;
		String^ SampleAesKeyUri;
		String^ SampleAesHlsSignalingDataMaster;
		String^ SampleAesHlsSignalingDataMedia;
	};

	public value struct ContentPackagingInfo {
		String^ ContentId;
		List<MultiDrmInfo>^ DrmInfos;
	};

	public ref class CpixClientWrapper
	{
	private:
		doverunner::CpixClient* _cpixClient;

	public:
		CpixClientWrapper(String^ kmsUrl);
		virtual ~CpixClientWrapper();

		int GetLastRequestStatus();
		String^ GetLastRequestRowData();
		String^ GetLastResponseRowData();

		ContentPackagingInfo GetContentKeyInfoFromDoverunnerKMS(String^ cid, DrmType drmType, EncryptionScheme encryptionScheme, TrackType trackType, long periodIndex);
	};
}
