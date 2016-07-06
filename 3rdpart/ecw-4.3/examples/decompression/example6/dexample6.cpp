/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
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
** FILE:   	example6.cpp
** CREATED:	June 2010
** AUTHOR: 	Chris Tapley
** PURPOSE: An example on how to use the header editor interface
**********************************************************/

#include "stdafx.h"
#include "NCSUtil.h"
#include "NCSEcw/API/HeaderEditor.h"

void PrintUsage()
{
#ifdef _DEBUG
		printf("Usage: dexample6d.exe file [-proj PROJECTION] [-datum DATUM] [-originx ORIGINX]\n");
#else
		printf("Usage: dexample6.exe file [-proj PROJECTION] [-datum DATUM] [-originx ORIGINX]\n");
#endif
		printf("  [-originy ORIGINY] [-cellx CELLX] [-celly CELLY]\n\n");
}

bool ParseArgs(int argc, _TCHAR* argv[], NCS::CHeaderEditor *pHeaderEditor) {
	CNCSString sArg, sValue;
	bool bValid = true;

	for(int i = 2; i < argc-1 && bValid; i++) {
		sArg = argv[i];
		char *szArg = (char*)sArg.a_str();
		if(szArg[0] == '-' || szArg[0] == '/') { 
			if(strcmp(szArg, "-proj") == 0) {
				sValue = argv[++i];
				if(sValue.size() > 0) {
					pHeaderEditor->SetProjection((char*)sValue.a_str());
				} else {
					bValid = false;
				}
			} else if(strcmp(szArg, "-datum") == 0) {
				sValue = argv[++i];
				if(sValue.size() > 0) {
					pHeaderEditor->SetDatum((char*)sValue.a_str());
				} else {
					bValid = false;
				}
			} else if(strcmp(szArg, "-originx") == 0) {
				sValue = argv[++i];
				try {
					pHeaderEditor->SetOriginX(atof(sValue.a_str()));
				} catch(...) {
					bValid = false;
				}
			} else if(strcmp(szArg, "-originy") == 0) {
				sValue = argv[++i];
				try {
					pHeaderEditor->SetOriginY(atof(sValue.a_str()));
				} catch(...) {
					bValid = false;
				}
			} else if(strcmp(szArg, "-cellx") == 0) {
				sValue = argv[++i];
				try {
					pHeaderEditor->SetCellIncrementX(atof(sValue.a_str()));
				} catch(...) {
					bValid = false;
				}
			} else if(strcmp(szArg, "-celly") == 0) {
				sValue = argv[++i];
				try {
					pHeaderEditor->SetCellIncrementY(atof(sValue.a_str()));
				} catch(...) {
					bValid = false;
				}
			}
		}
	}
	return bValid;
}

void PrintComparison(NCSEcwEditInfo *pOld, NCSEcwEditInfo *pNew)
{
	printf("ORIGINAL:\nNCSEcwEditInfo {\n");
	//printf("\tnVersion:\t%d\n", pOld->nVersion);
	printf("\tfCellIncX:\t%f\n", pOld->fCellIncrementX);
	printf("\tfCellIncY:\t%f\n", pOld->fCellIncrementY);
	printf("\tfOriginX:\t%f\n", pOld->fOriginX);
	printf("\tfOriginY:\t%f\n", pOld->fOriginY);
	printf("\tszDatum:\t%s\n", pOld->szDatum);
	printf("\tszProjection:\t%s\n", pOld->szProjection);
	//printf("\tEPSGcode:\t%d\n", pOld->EPSGcode);
	printf("}\n\n");

	printf("\nUPDATED:\nNCSEcwEditInfo {\n");
	//printf("\tnVersion:\t%d\n", pNew->nVersion);
	printf("\tfCellIncX:\t%f\n", pNew->fCellIncrementX);
	printf("\tfCellIncY:\t%f\n", pNew->fCellIncrementY);
	printf("\tfOriginX:\t%f\n", pNew->fOriginX);
	printf("\tfOriginY:\t%f\n", pNew->fOriginY);
	printf("\tszDatum:\t%s\n", pNew->szDatum);
	printf("\tszProjection:\t%s\n", pNew->szProjection);
	//printf("\tEPSGcode:\t%d\n", pNew->EPSGcode);
	printf("}\n\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	CNCSError Error;
	CNCSString sFileName;
	if(argc < 4) {
		PrintUsage();
		exit(1);
	}
	NCSEcwEditInfo *pOriginalInfo = NULL;

	sFileName = argv[1];
	NCS::CHeaderEditor HeaderEditor;
	if((Error = HeaderEditor.SetFile((char*)sFileName.a_str())) != NCS_SUCCESS) {
		printf("Failed to open the file.\n\n%s\n\n", NCSGetErrorText(Error));
		exit(1);
	}

	NCSEcwEditCopyInfo(HeaderEditor.GetEditInfo(), &pOriginalInfo);

	if(ParseArgs(argc, argv, &HeaderEditor)) {
		if((Error = HeaderEditor.ApplyHeaderChanges()) != NCS_SUCCESS) {
			printf("Failed to write to the file.\n\n%s\n\n", NCSGetErrorText(Error));
			exit(1);
		}
	} else {
		printf("Invalid argument.\n\n");
		exit(1);
	}
	printf("Operation Completed Successfully.\n\n");

	PrintComparison(pOriginalInfo, HeaderEditor.GetEditInfo());

	return 0;
}

