/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     WorldFile.h 
** CREATED:  02/09/2004 3:27:34 PM
** AUTHOR:   Tom Lynch
** PURPOSE:  Utility class for reading six-value world files
** EDITS:    [xx]	ddMmmyy	NAME	COMMENTS
 *******************************************************/

#include "NCSEcw/SDK/AffineTransform.h"
#ifndef ERS_JP2_WORLD_FILE_EXTS
#define ERS_JP2_WORLD_FILE_EXTS	{ ".j2w" , ".jxw" , ".jfw" }  
#endif

namespace NCS {
namespace SDK {

class CWorldFile: public CAffineTransform
{
public:
	CWorldFile();
	CWorldFile(IEEE8 fPrecision);
	~CWorldFile();

	bool Read(char *pWorldFilename);
	bool WriteMSB(char *pWorldFilename);
	bool ConstructWorldFilename(char *szRasterFilename, char **szWorldFilename);
private:
	static inline bool ScanValue(FILE *pFile, IEEE8 *pValue);
};

}
}
