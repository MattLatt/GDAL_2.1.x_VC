/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     TilePartHeader.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CTilePartHeader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCTILEPARTHEADER_H
#define NCSJPCTILEPARTHEADER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/SOTMarker.h"
#include "NCSEcw/JPC/Markers/CODMarker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSEcw/JPC/Markers/QCCMarker.h"
#include "NCSEcw/JPC/Markers/RGNMarker.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSEcw/JPC/Markers/SODMarker.h"
#include "NCSEcw/JPC/Markers/PLTMarker.h"
#include "NCSEcw/JPC/Markers/PPTMarker.h"
#include "NCSEcw/JPC/Markers/COMMarker.h"
#include "NCSEcw/JPC/Nodes/Component.h"
#include "NCSEcw/JPC/Progression.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/JPC.h"

namespace NCS {
namespace JPC {
	/**
	 * CTilePartHeader class - the JPC codestream Tile-Part Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #4 $ $Author: jshao $ $Date: 2012/01/09 $ 
	 */	
class NCSECW_IMPEX CTilePartHeader: public SDK::CNode2D, public CProgression {
public:
		// Tile-Part absolute index within codestream
	UINT32	m_nZtp;
		/** pointer to main JPC */
	CJPC *m_pJPC;

		/** Tile-part header is valid */
	bool	m_bValid;

		/** SOC start of codestream Marker - required */
	CSOTMarker	m_SOT;
		/** COD coding style default Marker - optional */
	CCODMarker	m_COD;
		/** QCD quantization default Marker - optional */
	CQCDMarker	m_QCD;
		/** RGN markers */
	std::vector<CRGNMarker> m_Regions;

		/** POC progression order change marker - optional
		 * NOTE: Over-rides any main-header POC and any COD Marker 
		 */
	CPOCMarker	m_POC;

		/** SOD start of data */
	CSODMarker	m_SOD;

		/** Packet length(s) for this tile-part */
	std::vector<CPLTMarker> m_PLTs;
		/** Hashed index to PLTs, in mod 32768 */
	std::vector<UINT32> m_PLTIndex;

		/** Packet header(s) for this tile-part */
	std::vector<CPPTMarker> m_PPTs;
		/** Are the PPT's from the file or dynamically created? */
	bool				m_bFilePPTs;

		/** If the tile has more than 1 tile part and this is tile part 0, this vectors
			contains all the other tile parts */
	std::vector<CTilePartHeader *> m_TileParts;

		/** Are the SOTs read in already? */
	bool				m_bSOTsReadAlready; // Against bug EMS-407, added 12/12/2011
		/** Are the Tile Parts and PLTs read clearable? */
		/** We might create precincts only within range of the current SetView. */
		/** We may need to create the same tile's other precincts which were not done at current SetView, so problem occures when PLTs (the same tile won't be read) are deleted */
	bool				m_bTilePartsAndPLTsClearble; // Against bug EMS-407, added 16/12/2011

		/** Tile Components */
	std::vector<CComponent*> m_Components;

		/** Tile Visibility */
	//bool	m_bVisible; // Against bug EMS-407, added 27/10/2011

		/** Tile m_nDeepestAllocatedResolution increased  */
	//bool	m_bDeepestAllocatedResolutionIncreased; // Against bug EMS-407, added 17/11/2011

		/** Encoder PLT Stream */
//	CIOStream		*m_pEncoderPLTFile;

		/** Default constructor, initialises members */
	CTilePartHeader(CJPC *pJPC);
		/** Virtual destructor */
	virtual ~CTilePartHeader();

		/** 
		 * Recalc the structure sizes required based on the current decoding info.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError RecalcSizes();
		/** 
		 * check if the structure is simple
		 * @return      void
		 */

	virtual void RecalcSimpleStructure(); // Against bug EMS-407, added 19/10/2011
		/** 
		 * Init the tile rect based on the currently available info.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */

	virtual CError InitRect();
	
		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		inOption	the required tile part ranges 
		 * @param		inTileNrStart: start tile number [0, CBuffer2D::MAXUINT32)
		 * @param		inTileNrEnd: end tile number [0, CBuffer2D::MAXUINT32), inclusive
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, UINT8 inOption = 0, 
		UINT32 inTileNrStart = 0, UINT32 inTileNrEnd = (unsigned)0xffffffff); //CBuffer2D::MAXUINT32);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

		/**
		 * Get the Tile Index
		 * @return		INT32		Index of the tile, >= 0
		 */
	INT32 GetIndex();
		/**
		 * Get the Tile P index
		 * @return		INT32		P index of the tile, >= 0
		 */
	INT32 GetP();
		/**
		 * Get the Tile Q index
		 * @return		INT32		Q of the tile, >= 0
		 */
	INT32 GetQ();

		/**
		 * Get the PPx from the component resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		PPx
		 */
	UINT32 GetPPx(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the PPy from the component resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		PPy
		 */
	UINT32 GetPPy(UINT16 iComponent, UINT8 nResolution);

		/**
		 * Get the xcb' from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		xcb'
		 */
	UINT32 GetXCBPrime(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the ycb' from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		ycb'
		 */
	UINT32 GetYCBPrime(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the CB Width from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		xcb'
		 */
	UINT32 GetCBWidth(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the CB Height from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		ycb'
		 */
	UINT32 GetCBHeight(UINT16 iComponent, UINT8 nResolution);

		/** 
		 * Increment the progression order from the current location, parsing packet headers in the process.
		 * @param		pData		Data for Step function
		 * @param		bComplete	Progression order is complete.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Iterate(CTilePartHeader *pMainTP, void *pData, bool &bComplete);
		/** 
		 * Perform the "Step" for this progression.
		 * @return      bool		true, or false on error & Error is set to actual error code.
		 */
	virtual bool Step(CComponent *pComponent,
					  CResolution *pResolution,
					  CPrecinct *pPrecinct);

	virtual bool StepRow(class CComponent *pComponent,
						 class CResolution *pResolution);

		/**
		 * Get the Packet Header for the specified packet number
		 * @param		nPacket		Packet number
		 * @return		CPacket	Packet Header struct (may be unparsed)
		 */
	CPacket *GetPacketHeader(UINT32 nPacket, bool bPurge = true, bool bLock = true);
		/**
		 * Get the sequential Packet Header for the specified packet number
		 * @param		nPacket		Packet number
		 * @return		CPacket	Packet Header struct (may be unparsed)
		 */
	CPacket *GetSequentialPacketHeader(UINT32 nPacket, UINT32 &nioPrevPLTnr, bool bPurge = true, bool bLock = true);
		/**
		 * Get the first packet number for this tile
		 * @return		UINT32	First packet number
		 */
	UINT32 GetFirstPacketNr();
		/**
		 * Get the number of packets for this tile
		 * @return		UINT32	Packet count
		 */
	UINT32 GetNrPackets();

		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);	

//	bool OpenEncoderPLTFile(bool bWrite);
//	bool CloseEncoderPLTFile(bool bDelete);

	bool SimpleStructure() {
		return(m_bSimpleStructure);
	}

	void ResetPacketInfo() {m_FirstPacketNr = CBuffer2D::MAXUINT32; m_NrPackets = CBuffer2D::MAXUINT32;}; // Against bug EMS-407, added 27/10/2011
	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets(bool bTilePartsAndPLTtoBeDeleted = false);	// Against bug EMS-407, added 02/11/2011
	bool ClearTilePartsAndPLTs();	// Against bug EMS-407, added 11/11/2011
	bool FreePLTsLength();	// Against bug EMS-407, added 22/11/2011

	void Lock(void);	// added on 04/01/2012
	void UnLock(void);	// added on 04/01/2012

private:
	bool SeekPacket(CJPC &JPC, CTilePartHeader *pMainTP, CTilePartHeader *pTP, UINT32 nPacket, CPLTMarker &inPLT, UINT32 &inPLTNr, UINT32 &inNr, std::vector<CPacketLengthType> *pLengths,
		CPacket *pioPH); // Added on 13/12/2011
	bool SeekPacket(UINT64 nOffset, CJPC &JPC, CTilePartHeader *pMainTP, CTilePartHeader *pTP, UINT32 nPacket, CPLTMarker &inPLT, UINT32 &inPLTNr, UINT32 &inNr, std::vector<CPacketLengthType> *pLengths,
		CPacket *pioPH);	// Added on 13/12/2011

protected:

	UINT32 m_FirstPacketNr;
	UINT32 m_NrPackets;

		// Dynamic precincts in progression Step()
	bool m_bDynPrecinct;
		// Stream is seekable
	bool m_bSeekable;
		// Parsing Dynamic precincts in progression Step()
	bool m_bParsingHeaders;
		// Simple structure, R/P/L equal in all C and no POC
	bool m_bSimpleStructure;

	//tbb::spin_mutex *m_pMutex_TP; // Against bug EMS-407, added 12/12/2011
	NCS::CMutex m_Mutex_TP;// Against bug EMS-407, changed 21/12/2011, as spin lock for short term

};
}
}


#endif // !NCSJPCTILEPARTHEADER_H
