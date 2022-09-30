// HelloWorld.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

// Include headers of DynamsoftDocumentNormalizer SDK
#include "../../../Include/DynamsoftDocumentNormalizer.h"


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

void main()
{
    void* ddn = NULL;
    int errorCode = 0;
    char szErrorMsg[256];
    NormalizedImageResult* normalizedResult = NULL;

    // 1.Initialize license.
    // The string "DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9" here is a free public trial license. Note that network connection is required for this license to work.
    // If you don't have a license yet, you can request a trial from https://www.dynamsoft.com/customer/license/trialLicense?product=ddn&utm_source=samples&package=c_cpp 
    errorCode = DC_InitLicense("DLS2eyJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSJ9", szErrorMsg, 256);
    if (errorCode != DM_OK)
    {
        printf("%s\r\n", szErrorMsg);
    }

    // 2. Create an instance of Dynamsoft Document Normalizer
    ddn = DDN_CreateInstance();

    // 3. Apply normalization for an image file
    errorCode = DDN_NormalizeFile(ddn, "../../../Images/sample-image.png", "", NULL, &normalizedResult);
    if (errorCode != DM_OK)
        printf("%s\r\n", DC_GetErrorString(errorCode));

    // 4. Save the normalized image to a local file
    if (normalizedResult != NULL)
    {
        NormalizedImageResult_SaveToFile(normalizedResult, "result-image.png");
    }

    // 5. Free the memory allocated for normalized image result and Dynamsoft Document Normalizer instance
    if (normalizedResult != NULL)
        DDN_FreeNormalizedImageResult(&normalizedResult);
    DDN_DestroyInstance(ddn);

    printf("Press any key to quit...");
    getchar();
}

