#include <iostream>
#include <string>

#include "../../../Include/DynamsoftCaptureVisionRouter.h"
#include "../../../Include/DynamsoftUtility.h"

using namespace std;
using namespace dynamsoft::cvr;
using namespace dynamsoft::ddn;
using namespace dynamsoft::license;
using namespace dynamsoft::basic_structures;
using namespace dynamsoft::utility;

// The following code only applies to Windows.
#if defined(_WIN64) || defined(_WIN32)
#ifdef _WIN64
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftCaptureVisionRouterx64.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftCorex64.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftUtilityx64.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x64/DynamsoftLicensex64.lib")
#else
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftCaptureVisionRouterx86.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftCorex86.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftUtilityx86.lib")
#pragma comment(lib, "../../../Distributables/Lib/Windows/x86/DynamsoftLicensex86.lib")
#endif
#endif

class MyImageSourceStateListener : public CImageSourceStateListener
{
private:
	CCaptureVisionRouter *m_router;

public:
	MyImageSourceStateListener(CCaptureVisionRouter *router)
	{
		m_router = router;
	}

	virtual void OnImageSourceStateReceived(ImageSourceState state)
	{
		if (state == ISS_EXHAUSTED)
			m_router->StopCapturing();
	}
};

class MyResultReceiver : public CCapturedResultReceiver
{
public:
	virtual void OnNormalizedImagesReceived(CNormalizedImagesResult *pResult)
	{
		const CFileImageTag *tag = dynamic_cast<const CFileImageTag *>(pResult->GetOriginalImageTag());
		cout << "File: " << tag->GetFilePath() << endl;

		string fileName = tag->GetFilePath();
		size_t pos = fileName.find_last_of('\\');
		if (pos == string::npos)
			pos = fileName.find_last_of('/');
		if (pos != string::npos)
		{
			fileName = fileName.substr(pos + 1);
		}
		else
		{
			static int resultCount = 0;
			fileName = to_string(resultCount);
			resultCount++;
		}
		if (pResult->GetErrorCode() != EC_OK)
		{
			cout << "Error: " << pResult->GetErrorString() << endl;
		}
		else
		{
			CImageManager manager;
			int lCount = pResult->GetItemsCount();
			cout << "Normalized " << lCount << " documents" << endl;
			for (int li = 0; li < lCount; ++li)
			{
				const CNormalizedImageResultItem *item = pResult->GetItem(li);

				string outPath = "normalizeImage_";
				outPath += fileName + "_" + to_string(li) + ".png";
				manager.SaveToFile(item->GetImageData(), outPath.c_str());

				cout << "Document " << li << " file: " << outPath << endl;
			}
		}

		cout << endl;
	}
};

int main()
{
	int errorcode = 0;
	char error[512];

	// 1.Initialize license.
	// You can request and extend a trial license from https://www.dynamsoft.com/customer/license/trialLicense?product=ddn&utm_source=samples&package=c_cpp
	// The string 'DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9' here is a free public trial license. Note that network connection is required for this license to work.
	errorcode = CLicenseManager::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", error, 512);
	if (errorcode != ErrorCode::EC_OK && errorcode != ErrorCode::EC_LICENSE_CACHE_USED)
	{
		cout << "License initialization failed: ErrorCode: " << errorcode << ", ErrorString: " << error << endl;
	}
	else
	{
		// 2.Create an instance of CCaptureVisionRouter.
		CCaptureVisionRouter *router = new CCaptureVisionRouter;

		// 3.Set input image source
		CDirectoryFetcher *dirFetcher = new CDirectoryFetcher;
		// Replace it with your image directory
		dirFetcher->SetDirectory("../../../Images");

		router->SetInput(dirFetcher);

		// 4. Add image source state listener
		CImageSourceStateListener *listener = new MyImageSourceStateListener(router);
		router->AddImageSourceStateListener(listener);

		// 5. Add captured result receiver
		CCapturedResultReceiver *recv = new MyResultReceiver;
		router->AddResultReceiver(recv);

		// 6. Start capturing
		router->StartCapturing(CPresetTemplate::PT_DETECT_AND_NORMALIZE_DOCUMENT, true);

		// 7. Release the allocated memeory
		delete router, router = NULL;
		delete dirFetcher, dirFetcher = NULL;
		delete listener, listener = NULL;
		delete recv, recv = NULL;
	}
	return 0;
}