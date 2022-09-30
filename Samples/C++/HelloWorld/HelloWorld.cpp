// HelloWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Include headers of DynamsoftDocumentNormalizer SDK
#include "../../../Include/DynamsoftDocumentNormalizer.h"

using namespace std;
using namespace dynamsoft::ddn;
using namespace dynamsoft::core;

// Link libraries of DynamsoftDocumentNormalizer SDK for Windows.
#if defined(_WIN64) || defined(_WIN32)
#ifdef _WIN64
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftCorex64.lib")
#pragma comment(lib, "../../../Lib/Windows/x64/DynamsoftDocumentNormalizerx64.lib")
#else
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftCorex86.lib")
#pragma comment(lib, "../../../Lib/Windows/x86/DynamsoftDocumentNormalizerx86.lib")
#endif
#endif

int main()
{
    int errorCode = 0;

     // 1.Initialize license.
    // The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a free public trial license. Note that network connection is required for this license to work.
    // If you don't have a license yet, you can request a trial from https://www.dynamsoft.com/customer/license/trialLicense?product=ddn&utm_source=samples&package=c_cpp 
    char szErrorMsg[256];
    errorCode = CLicenseManager::InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", szErrorMsg, 256);
    if (errorCode != DM_OK)
    {
        cout << szErrorMsg << endl;
    }

    // 2. Create an instance of Dynamsoft Document Normalizer
    CDocumentNormalizer ddn;

    // 3. Apply normalization for an image file
    CNormalizedImageResult* normalizedResult = NULL;
    errorCode = ddn.Normalize("../../../Images/sample-image.png", "", NULL, &normalizedResult);
    if (errorCode != DM_OK)
        cout << DC_GetErrorString(errorCode) << endl;

    // 4. Save the normalized image to a local file
    if (normalizedResult != NULL)
    {
        normalizedResult->SaveToFile("result-image.png");
    }

    // 5. Free the memory allocated for normalized image result
    if (normalizedResult != NULL)
        CDocumentNormalizer::FreeNormalizedImageResult(&normalizedResult);

    cout << "Press any key to quit..." << endl;
    cin.ignore();
    return 0;
}

