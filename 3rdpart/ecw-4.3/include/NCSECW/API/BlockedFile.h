/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	BlockedFile.h
** CREATED:	Wed 04/10/2006
** AUTHOR: 	Simon Cope
** PURPOSE:	OTDF File management class
** EDITS:
 *******************************************************/

#ifndef NCSBLOCKEDFILE_H
#define NCSBLOCKEDFILE_H

#pragma warning(push)
#pragma warning(disable: 4251)

#ifndef NCSTYPES_H
#include "NCSDefs.h"
#endif
#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif

#ifndef NCSERRORS_H
#include "NCSErrors.h"
#endif

#ifndef NCSMULTIVIEW_H
#include "NCSEcw/API/MultiView.h"
#endif

#ifndef NCSMEMORYIOSTREAM_H
#include "NCSMemoryIOStream.h"
#endif

namespace NCS {
	/**
	 * CBlockedFile class - Low-level OTDF file management class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: simon $ $Date: 2007/03/29 $ 
	 */	
class NCS_EXPORT CBlockedFile: public NCS::CMultiView
{
public:
		/** Default Constructor */
	CBlockedFile();
		/** Virtual destructor */
	virtual ~CBlockedFile();

#ifdef NCS_BUILD_UNICODE
	virtual CError Open(wchar_t *pURLPath);
#endif
	virtual CError Open(char *pURLPath);
	virtual CError Open(NCS::CIOStream *pStream, bool bProgressiveDisplay = false);
	virtual CError Close ( bool bFreeCache = false );
	
	virtual CError SetTileSize(UINT16 nTileSize) { m_nTileSize = nTileSize; return(NCS_SUCCESS); };
	virtual UINT16 GetTileSize() { return(m_nTileSize); };
	virtual CError SetTileBlockSize(UINT16 nTileBlockSize);
	virtual UINT16 GetTileBlockSize() { return(m_nTileBlockSize); };
           	
	virtual CError ReadTileBlock(UINT64 nTileBlock, UINT32 &nWidth, UINT32 &nHeight, NCS::SDK::CBuffer2DVector &img, bool bNoICC); 
	virtual CError ReadTileBlock(UINT64 nTileBlock, UINT32 &nWidth, UINT32 &nHeight, void **ppData, UINT32 &nLen, CString &Format); 

	virtual CError GetTileBlockFromCoords(UINT64 &nBlock, INT32 l, INT32 tx, INT32 ty);
	virtual CError ValidateCoords(INT32 &l, INT32 &tx, INT32 &ty);
	virtual CError GetTileCoordsFromBlock(UINT32 nBlock, INT32 &l32, INT32 &tx, INT32 &ty);
	virtual CError ValidateBlock(UINT64 &nBlock);

	virtual bool EncodeJPGFromBuffer(NCS::CMemoryIOStream &buf, NCS::SDK::CBuffer2DVector &img, NCSFileColorSpace eColor);
protected:
	class CLevel {
	public:
		UINT8	m_nLevel;
		CBlockedFile *m_pFile;
		UINT32 m_nWidth;
		UINT32 m_nHeight;
		UINT32 m_nTileBlocksWide;
		UINT32 m_nTileBlocksHigh;
		UINT64 m_nFirstTileBlock;

		CLevel(CBlockedFile *pFile, UINT8 level) {
				m_nFirstTileBlock = 0;
				m_pFile = pFile;
				m_nLevel = level;
				NCSFileViewFileInfoEx *pFileInfo = pFile->GetFileInfo();
				m_nWidth = NCSCeilDiv(pFileInfo->nSizeX, NCS2Pow(level));
				m_nHeight = NCSCeilDiv(pFileInfo->nSizeY, NCS2Pow(level));
				m_nTileBlocksWide = NCSCeilDiv(pFileInfo->nSizeX, pFile->m_nTileBlockSize * NCS2Pow(level));
				m_nTileBlocksHigh = NCSCeilDiv(pFileInfo->nSizeY, pFile->m_nTileBlockSize * NCS2Pow(level));
			};
		virtual ~CLevel() {};
	};

	UINT16	m_nTileSize;
	UINT16	m_nTileBlockSize;
	UINT64	m_nTileBlocks;
	std::vector<CLevel*> m_Levels;
};

}


#pragma warning(pop)

#endif /* NCSBLOCKEDFILE_H */
