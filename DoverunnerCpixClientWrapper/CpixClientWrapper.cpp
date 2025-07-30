#include <iostream>
#include <msclr/marshal_cppstd.h>

#include "CpixClientWrapper.h"

namespace DoveRunner {
	DoveRunner::CpixClientWrapper::CpixClientWrapper(String^ kmsUrl)
		: _cpixClient(new doverunner::CpixClient(msclr::interop::marshal_as<std::string>(kmsUrl)))
	{
	}

	DoveRunner::CpixClientWrapper::~CpixClientWrapper()
	{
		delete _cpixClient;
		_cpixClient = nullptr;
	}

	int CpixClientWrapper::GetLastRequestStatus()
	{
		return _cpixClient->GetLastRequestStatus();
	}

	String^ CpixClientWrapper::GetLastRequestRowData()
	{
		return gcnew String(_cpixClient->GetLastRequestRowData().c_str());
	}

	String^ CpixClientWrapper::GetLastResponseRowData()
	{
		return gcnew String(_cpixClient->GetLastResponseRowData().c_str());
	}

	ContentPackagingInfo DoveRunner::CpixClientWrapper::GetContentKeyInfoFromDoveRunnerKMS(String^ cid, DrmType drmType, EncryptionScheme encryptionScheme, TrackType trackType, long periodIndex)
	{
		ContentPackagingInfo packInfos;
		try {
			doverunner::ContentPackagingInfo contentPackInfo = _cpixClient->GetContentKeyInfoFromDoveRunnerKMS(msclr::interop::marshal_as<std::string>(cid)
				, static_cast<doverunner::DrmType>(drmType), static_cast<doverunner::EncryptionScheme>(encryptionScheme), static_cast<doverunner::TrackType>(trackType), periodIndex);

			packInfos.ContentId = gcnew String(contentPackInfo.contentId.c_str());
			packInfos.DrmInfos = gcnew List<MultiDrmInfo>;

			for (auto multiDrmInfo : contentPackInfo.multiDrmInfos)
			{
				MultiDrmInfo drmInfo;
				drmInfo.TrackType = gcnew String(multiDrmInfo.trackType.c_str());
				drmInfo.Key = gcnew String(multiDrmInfo.key.c_str());
				drmInfo.KeyId = gcnew String(multiDrmInfo.keyId.c_str());
				drmInfo.Iv = gcnew String(multiDrmInfo.iv.c_str());
				drmInfo.PeriodIndex = gcnew String(multiDrmInfo.periodIndex.c_str());
				drmInfo.WidevinePSSH = gcnew String(multiDrmInfo.widevinePSSH.c_str());
				drmInfo.WidevinePSSHpayload = gcnew String(multiDrmInfo.widevinePSSHpayload.c_str());
				drmInfo.WidevineHlsSignalingDataMaster = gcnew String(multiDrmInfo.widevineHlsSignalingDataMaster.c_str());
				drmInfo.WidevineHlsSignalingDataMedia = gcnew String(multiDrmInfo.widevineHlsSignalingDataMedia.c_str());
				drmInfo.PlayreadyPSSH = gcnew String(multiDrmInfo.playreadyPSSH.c_str());
				drmInfo.PlayreadyPSSHpayload = gcnew String(multiDrmInfo.playreadyPSSHpayload.c_str());
				drmInfo.PlayreadySmoothStreamingData = gcnew String(multiDrmInfo.playreadySmoothStreamingData.c_str());
				drmInfo.PlayreadyHlsSignalingDataMaster = gcnew String(multiDrmInfo.playreadyHlsSignalingDataMaster.c_str());
				drmInfo.PlayreadyHlsSignalingDataMedia = gcnew String(multiDrmInfo.playreadyHlsSignalingDataMedia.c_str());
				drmInfo.FairplayHlsKeyUri = gcnew String(multiDrmInfo.fairplayHlsKeyUri.c_str());
				drmInfo.FairplayHlsSignalingDataMaster = gcnew String(multiDrmInfo.fairplayHlsSignalingDataMaster.c_str());
				drmInfo.FairplayHlsSignalingDataMedia = gcnew String(multiDrmInfo.fairplayHlsSignalingDataMedia.c_str());
				drmInfo.WiseplayPSSH = gcnew String(multiDrmInfo.wiseplayPSSH.c_str());
				drmInfo.WiseplayPSSHpayload = gcnew String(multiDrmInfo.wiseplayPSSHpayload.c_str());
				drmInfo.WiseplayHlsSignalingDataMaster = gcnew String(multiDrmInfo.wiseplayHlsSignalingDataMaster.c_str());
				drmInfo.WiseplayHlsSignalingDataMedia = gcnew String(multiDrmInfo.wiseplayHlsSignalingDataMedia.c_str());
				drmInfo.NcgCek = gcnew String(multiDrmInfo.ncgCek.c_str());
				drmInfo.NcghlsAes128KeyUri = gcnew String(multiDrmInfo.ncghlsAes128KeyUri.c_str());
				drmInfo.NcghlsAes128HlsSignalingDataMaster = gcnew String(multiDrmInfo.ncghlsAes128HlsSignalingDataMaster.c_str());
				drmInfo.NcghlsAes128HlsSignalingDataMedia = gcnew String(multiDrmInfo.ncghlsAes128HlsSignalingDataMedia.c_str());
				drmInfo.NcghlsSampleAesKeyUri = gcnew String(multiDrmInfo.ncghlsSampleAesKeyUri.c_str());
				drmInfo.NcghlsSampleAesHlsSignalingDataMaster = gcnew String(multiDrmInfo.ncghlsSampleAesHlsSignalingDataMaster.c_str());
				drmInfo.NcghlsSampleAesHlsSignalingDataMedia = gcnew String(multiDrmInfo.ncghlsSampleAesHlsSignalingDataMedia.c_str());
				drmInfo.Aes128KeyUri = gcnew String(multiDrmInfo.aes128KeyUri.c_str());
				drmInfo.Aes128HlsSignalingDataMaster = gcnew String(multiDrmInfo.aes128HlsSignalingDataMaster.c_str());
				drmInfo.Aes128HlsSignalingDataMedia = gcnew String(multiDrmInfo.aes128HlsSignalingDataMedia.c_str());
				drmInfo.SampleAesKeyUri = gcnew String(multiDrmInfo.sampleAesKeyUri.c_str());
				drmInfo.SampleAesHlsSignalingDataMaster = gcnew String(multiDrmInfo.sampleAesHlsSignalingDataMaster.c_str());
				drmInfo.SampleAesHlsSignalingDataMedia = gcnew String(multiDrmInfo.sampleAesHlsSignalingDataMedia.c_str());

				packInfos.DrmInfos->Add(drmInfo);
			}
		}
		catch (doverunner::CpixClientException& e)
		{
			std::string errMsg = "An error has occurred in the CPIX Client module : \n";
			errMsg.append(e.what());
			throw gcnew Exception(gcnew String(errMsg.c_str()));
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}

		return packInfos;
	}
}