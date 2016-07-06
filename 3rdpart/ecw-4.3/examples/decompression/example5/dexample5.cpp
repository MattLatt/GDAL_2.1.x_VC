/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
*
* This document contains unpublished source code of
* ERDAS, Inc. This notice does
* not indicate any intention to publish the source
* code contained herein.
*
* Use of this product is subject to End-User License Agreement for ERDAS,
* Inc.  Use of the examples provided to design, develop,
* and test your own applications is bound by the terms of the license.  Please
* refer to license.txt under the program folder for the contents
* of the license. The license contains important rights and obligations and
* should be read carefully.  If you do not agree to be bound by the terms and
* conditions of the license, you must immediately uninstall and destroy all
* copies of the product.
*  
** FILE:   	dexample5.cpp
** CREATED:	1st June 2010
** AUTHOR: 	Chris Tapley
**
** PURPOSE:	Example program demonstrating reading an entire image at dataset
**			resolution.
**
*******************************************************/

#include "stdafx.h"
#include "NCSFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2) {
#ifdef _DEBUG
		printf("Usage: dexample5d.exe <file>\n\n");
#else
		printf("Usage: dexample5.exe <file>\n\n");
#endif
		exit(1);
	}

	NCSecwSetConfig(NCSCFG_CACHE_MAXMEM, 1000000000);

	NCSTimeStampMs tsStart = NCSGetTimeStampMs();

	CNCSString sFileName = argv[1];
	CNCSFile File;
	CNCSError Error;
	if((Error = File.Open(sFileName, false, false)) == NCS_SUCCESS) {
		
		NCSFileViewFileInfoEx *pInfo = File.GetFileInfo();
		UINT32 *pBandList = (UINT32*)NCSMalloc(pInfo->nBands * sizeof(UINT32), FALSE);
		for(UINT16 nBand = 0; nBand < pInfo->nBands; nBand++) {
			pBandList[nBand] = nBand;
		}
		if((Error = File.SetView(pInfo->nBands, pBandList, 0, 0, pInfo->nSizeX-1, pInfo->nSizeY-1, pInfo->nSizeX, pInfo->nSizeY)) == NCS_SUCCESS) {
			// Reading each line as UINT8, this may not be desirable in your app when working with non 8 bit jp2's.
			UINT8 **pLine = (UINT8**)NCSMalloc(pInfo->nBands * sizeof(UINT8*), TRUE);
			for(UINT16 nBand = 0; nBand < pInfo->nBands; nBand++) {
				pLine[nBand] = (UINT8*)NCSMalloc(pInfo->nSizeX * sizeof(UINT8), TRUE);
			}
			for(UINT32 nLine = 0; nLine < pInfo->nSizeY && Error == NCS_SUCCESS; nLine++) {
				if(File.ReadLineBIL(pLine) == NCSECW_READ_OK) {
					fprintf(stdout, "Read: %d%% Complete\r", (int)(((((float)nLine) / pInfo->nSizeY) * 100) + 0.5));
					fflush(stdout); 
				} else {
					Error = NCS_FILE_IO_ERROR;
				}
			}
			for(UINT16 nBand = 0; nBand < pInfo->nBands; nBand++) {
				NCSFree(pLine[nBand]);
			}
			NCSFree(pLine);
		}

		// passing true to close clears the cache for this file if there 
		// are no other file views open to this file
		File.Close(true);
	} else {
		printf("Unable to open file: %s\n\n", NCSGetLastErrorText(Error));
		exit(1);
	}
	if(Error == NCS_SUCCESS) {
		printf("Read completed successfully in %I64d seconds\n", (NCSGetTimeStampMs()-tsStart)/1000);
	}

	return 0;
}

