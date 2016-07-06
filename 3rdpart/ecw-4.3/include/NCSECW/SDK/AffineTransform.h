/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     AffineTransform.h
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 03Aug04 tfl  Added integer member to NCSJP2PCSBox
**			 [02] 07Dec04 tfl  Moved GeodataUsage enum declaration to NCSECWClient.h
**			 [03] 17Dec04 tfl  Added static CFile::Shutdown() function called by CFile::Shutdown()
 *******************************************************/

#ifndef NCSAFFINETRANSFORM_H
#define NCSAFFINETRANSFORM_H

#include "NCSECWClient.h"
#include "NCSEcw/JP2/File.h" //for TIFF tag

namespace NCS {

class CAffineTransform 
{

public:
	IEEE8 fScaleX;
	IEEE8 fScaleY;
	IEEE8 fRotX;
	IEEE8 fRotY;
	IEEE8 fTransX;
	IEEE8 fTransY;
	
	CAffineTransform();
	CAffineTransform(IEEE4 fPrecision);
	~CAffineTransform();

	bool GetFromFileInfoEx(NCSFileViewFileInfoEx &FileInfo, bool bRasterPixelIsArea);
	bool GetFromFileInfo(NCSFileViewFileInfo &FileInfo, bool bRasterPixelIsArea);
	bool GetFromGeoTIFFModelTransformationTag(JP2::CFile::CPCSBox::TIFFTag *pTransformTag);
	bool PopulateFileInfoEx(NCSFileViewFileInfoEx &FileInfo, bool bRasterPixelIsArea);
	bool PopulateFileInfo(NCSFileViewFileInfo &FileInfo);
	bool IsValid();
protected:
	void Standardize();
	IEEE8 m_fPrecision;
};
}

#endif
