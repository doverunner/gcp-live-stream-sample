---------------------------------------

# Google Cloud Live Stream API & DoveRunner DRM Integration Sample
This sample shows how to integrate DoveRunner Multi DRM with Google Cloud Live Stream API v1 using [API Client Libraries](https://cloud.google.com/livestream/docs/reference/libraries). Since this sample focused on DRM integration, only a simple scenario of applying Widevine, PlayReady, and FairPlay DRM to a live stream in fmp4 format is used, see the [references link](https://cloud.google.com/dotnet/docs/reference/Google.Cloud.Video.LiveStream.V1/latest) for more information about Live Stream API v1 features.



---------------------------------------
## Prerequisites
- A Windows 10/11 PC
- Visual Studio 2022 (Windows 10/11)
- .NET 6.0 SDK : https://dotnet.microsoft.com/download
- Google Cloud account : https://cloud.google.com/
  - Create a project : https://cloud.google.com/resource-manager/docs/creating-managing-projects
  - Create a bucket : https://cloud.google.com/storage/docs/creating-buckets
- Google Application Default Credentials (ADC) : https://cloud.google.com/docs/authentication/provide-credentials-adc

- Identity and Access Management (IAM)

  - roles/livestream.editor

  - roles/storage.admin

  - roles/secretmanager.admin

  - roles/secretmanager.secretAccessor
    - This role should be granted to the service account, not the user, as it is the permission to access Google Secret from the service API. 

    - https://cloud.google.com/livestream/docs/access-control#access_to_gcs


- {enc-token} used for CPIX API communication with DoveRunner KMS. This is an API authentication token that is generated when you sign up DoveRunner service, and can be found on the DoveRunner Console site.
- Encoder to generate the input stream that the API processes.

  - In this sample, [ffmpeg](https://ffmpeg.org/download.html) is used.

  
---------------------------------------
## How to launch the project and test
1. Clone or download this sample repository.
2. Open the root /DoveRunnerGoogleLiveStreamSample.sln and select the active project to launch in Visual Studio.
3. Make sure you have your Google Cloud project, bucket information and DoveRunner KMS related information.
4. Set the values of the variables at the top of the main method.
5. Run the project.
6. Copy the  `<RTMP input endpoint uri>` that is printed to the console.
7. When the streamingState is `AwaitingInput` and you see the output saying `the channel is ready`, send a live stream to the input endpoint as shown below.

   ```shell
   $ ffmpeg -re -f lavfi -i "testsrc=size=1280x720 [out0]; sine=frequency=500 [out1]" \
     -acodec aac -vcodec h264 -f flv <RTMP input endpoint uri>
   ```
8. Check out your packaging results on the Buckets page in Google Cloud console.

   
---------------------------------------
## DoveRunnerCpixClientWrapper
C++/CLI project for wrap a C++ library(*DoveRunnerCpixClient.lib*) to communicate with DoveRunner KMS server.
The _GetContentKeyInfoFromDoveRunnerKMS_ function allows you to obtain packaging information from the KMS server.



```c#
ContentPackagingInfo DoveRunner::CpixClientWrapper::GetContentKeyInfoFromDoveRunnerKMS(String^ cid, DrmType drmType, EncryptionScheme encryptionScheme, TrackType trackType, long periodIndex)
{
    ContentPackagingInfo packInfos;
    try {
        doverunner::ContentPackagingInfo contentPackInfo = _cpixClient->GetContentKeyInfoFromDoveRunnerKMS(msclr::interop::marshal_as<std::string>(cid), static_cast<doverunner::DrmType>(drmType), static_cast<doverunner::EncryptionScheme>(encryptionScheme), static_cast<doverunner::TrackType>(trackType), periodIndex);
        
        packInfos.ContentId = gcnew String(contentPackInfo.contentId.c_str());
        packInfos.DrmInfos = gcnew List<MultiDrmInfo>;

        for (auto multiDrmInfo : contentPackInfo.multiDrmInfos)
        {
            ...
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
```



---------------------------------------

## References
- https://doverunner.com/docs/en/multidrm/
- https://doverunner.com/docs/en/multidrm/packaging/cpix-api/
- https://cloud.google.com/livestream/docs/reference/libraries
- https://cloud.google.com/secret-manager/docs/reference/libraries#client-libraries-install-csharp
- https://cloud.google.com/livestream/docs/reference/drm#string
---------------------------------------
