/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     MainHeader.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CMainHeader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCMAINHEADER_H
#define NCSJPCMAINHEADER_H

#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/SOCMarker.h"
#include "NCSEcw/JPC/Markers/SIZMarker.h"
#include "NCSEcw/JPC/Markers/CODMarker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Markers/EOCMarker.h"
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSEcw/JPC/Markers/QCCMarker.h"
#include "NCSEcw/JPC/Markers/RGNMarker.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSEcw/JPC/Markers/TLMMarker.h"
#include "NCSEcw/JPC/Markers/PLMMarker.h"
#include "NCSEcw/JPC/Markers/PPMMarker.h"
#include "NCSEcw/JPC/Markers/CRGMarker.h"
#include "NCSEcw/JPC/Markers/COMMarker.h"
#include "NCSEcw/JPC/Types/Types.h"

namespace NCS {
namespace JP2 {
	class CFile;
}
namespace JPC {
	/**
	 * CMainHeader class - the JPC codestream Main Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #4 $ $Author: jshao $ $Date: 2012/01/09 $ 
	 */	
class NCSECW_IMPEX CMainHeader {
public:
		/** Main header is valid */
	bool	m_bValid;

		/** SOC start of codestream Marker - required */
	CSOCMarker	m_SOC;
		/** SIZ image size Marker - required */
	CSIZMarker	m_SIZ;
		/** COD coding style default Marker - required */
	CCODMarker	m_COD;
		/** QCD quantization default Marker - required */
	CQCDMarker	m_QCD;
		/** EOC end of codestream Marker - required */
	CEOCMarker	m_EOC;
		/** RGN markers */
	std::vector<CRGNMarker> m_RGNs;

		/** Any progression order changes 
		 * NOTE: this over-rides any progression in a tile-part COD marker
		 */
	CPOCMarker	m_POC;
		/** TLM markers */
	std::vector<CTLMMarker> m_TLMs;
		/** PLM markers */
	std::vector<CPLMMarker> m_PLMs;
		/** PPM markers */
	std::vector<CPPMMarker> m_PPMs;
		/** Are the PPM's from the file or dynamically created? */
	bool		m_bFilePPMs;

		/** CRG component registration - optional */
	CCRGMarker	m_CRG;
		/** COM Marker - compression only */
	CCOMMarker	m_COM;

		/** Are tiles (SOTs) stored order of by Node Wide? */
	bool		m_bSOTsOrderNodeWide; // Against bug EMS-407, added 03/11/2011
		/** Start and end offsets of each Tile Node Wide, very useful when some tiles are read in many times */
	std::vector< std::pair<UINT64, UINT64> > m_nOffsetsSOTsList; // to be used when m_bSOTsOrderNodeWide = true. Against bug EMS-407, added 03/11/2011

		/** Start and end offsets of each Tile, very useful when some tiles are read in many times */
	std::vector< std::pair<UINT64, UINT64> > m_nOffsetsSOTsList_Tile; // Against bug EMS-407, added 03/01/2012

		/** Tile-row list, added on 22/12/2011 to reduce calculation for every line 
		 *  Each tile-row holds the number of references which are decoding
		 */
	std::vector<UINT32> m_vTileRowList;	

		/** Default constructor, initialises members */
	CMainHeader();
		/** Virtual destructor */
	virtual ~CMainHeader();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);

		/** 
		 * Parse the SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseSOTs_MemoryWise(class CJPC &JPC, CIOStream &Stream,
		UINT32 inTileNrStart, UINT32 inTileNrEnd);

		/** 
		 * Parse the SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseSOTs(class CJPC &JPC, CIOStream &Stream);

		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

		/**
		 * Get X0, the component top left in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentX0(INT32 iComponent);
		/**
		 * Get X1, the component bottom right in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentX1(INT32 iComponent);
		/**
		 * Get Y0, the component top left in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentY0(INT32 iComponent);
		/**
		 * Get Y1, the component bottom right in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentY1(INT32 iComponent);
		/**
		 * Get the component width in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component width
		 */
	INT32 GetComponentWidth(INT32 iComponent);
		/**
		 * Get the component height in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component height
		 */
	INT32 GetComponentHeight(INT32 iComponent);

		/**
		 * Get the number of tile across the image
		 * @return		INT32		Number of tiles across image
		 */
	INT32 GetNumXTiles();
		/**
		 * Get the number of tile down the image
		 * @return		INT32		Number of tiles down image
		 */
	INT32 GetNumYTiles();
		/**
		 * Get the Tile P index from the tile index
		 * @param		INT32		Tile Index, >= 0
		 * @return		INT32		X of the tile, >= 0
		 */
	INT32 GetTilePFromIndex(INT32 iIndex);
		/**
		 * Get the Tile Q index from the tile index
		 * @param		INT32		Tile Index, >= 0
		 * @return		INT32		Y of the tile, >= 0
		 */
	INT32 GetTileQFromIndex(INT32 iIndex);
		/**
		 * Get the Tile Index from the tile PQ index
		 * @param		INT32		Tile P Index
		 * @param		INT32		Tile Q Index
		 * @return		INT32		Tile Index
		 */
	INT32 GetTileIndexFromPQ(INT32 iPIndex, INT32 iQIndex);

		/** Tmp directory for encoding */
	wchar_t				*m_pTmpDir;

		/** Encoder PLT Stream - holds all packet lengths during encoding */
	CIOStream		*m_pEncoderPLTFile;
		/** Encoder Resolution Packet Offset temp files */
	CIOStream		*m_pEncoderOffsetFile;
		/** Encoder Resolution Packet temp files */
	CIOStreamVector m_EncoderFiles;

	bool OpenEncoderFiles(bool bWrite);
	bool CloseEncoderFiles(bool bDelete);

	/** start of the first SOT */
	UINT64		m_nFirstSOToffset; // Against bug EMS-407, added 27/10/2011

protected:
	friend class CTilePartHeader;
	friend class JP2::CFile;
			/** CodingStyle for each component */
	std::vector<CCODMarker> m_CodingStyles;
		/** QuantizationStyle for each component */
	std::vector<CQCDMarker> m_QuantizationStyles;

};
}
}


#endif // !NCSJPCMAINHEADER_H
