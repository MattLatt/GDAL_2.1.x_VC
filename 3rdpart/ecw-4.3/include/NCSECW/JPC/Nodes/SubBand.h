/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     SubBand.h 
** CREATED:  13/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSubBand class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCSUBBAND_H
#define NCSJPCSUBBAND_H

#include "NCSEcw/JPC/TagTree.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/SDK/NodeTiler2D.h"
//#include "NCSEcw/JPC/Nodes/CodeBlock.h"

namespace NCS {
namespace JPC {

	/**
	 * CSubBand class - the JPC codestream SubBand.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2011/11/18 $ 
	 */	
class NCSECW_IMPEX CSubBand: public SDK::CNodeTiler2D {
public:
			/** SubBand enumerated type */
	typedef enum {
			/** LowLow subband */
		LL		= 0,
			/** HighLow - horizontally high-pass subband */
		HL		= 1,
		/** LowHigh - vertically high-pass subband */
		LH		= 2,
			/** HighHigh */
		HH		= 3
	} Type;
		/** Precincts for this SubBand */
	class CPrecinct	*m_pPrecinct;

		/** Type of subband */
	Type	m_eType;

		/** Layer # each codeblock is first included in */
	CTagTree	m_LayerInclusion;
		/** # of zero bit planes for each codeblock */
	CTagTree	m_ZeroPlanes;
	
		/** CodeBlock data */
	class CCodeBlock *m_pCodeBlocks;
//	std::vector<class CCodeBlock> *m_pCodeBlocks;

		/** 
		 * Constructor 
		 * @param pPrecinct		Parent precinct
		 * @param eType			SubBand type LL,LH,HL,HH
		 */
	CSubBand(class CPrecinct *pPrecinct, CSubBand::Type eType);
		/** Virtual destructor */
	virtual ~CSubBand();

		/** 
		 * Get X0 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX0(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
		/** 
		 * Get X0 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX0(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

		/** 
		 * Get Y0 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY0(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
		/** 
		 * Get Y0 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY0(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

		/** 
		 * Get X1 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX1(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
		/** 
		 * Get X1 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX1(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

		/** 
		 * Get Y1 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY1(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
		/** 
		 * Get Y1 of this node.
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY1(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

		/** 
		 * Get codeblock Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	UINT32 GetCBWidth();
		/** 
		 * Get codeblock Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	UINT32 GetCBHeight();

		/** 
		 * Get number of codeblocks wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	UINT32 NCS_INLINE GetNumCBWide() {
				return(m_NumCBWide);
			};
		/** 
		 * Get number of codeblocks high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	UINT32 NCS_INLINE GetNumCBHigh() {
				return(m_NumCBHigh);
			};

		/** 
		 * Get the orientation value from the given subband type
		 * @param		etype		NCS SubBand type.
		 * @return      UINT8		SubBand orientation value.
		 */
	static NCS_INLINE UINT8 GetOrient(CSubBand::Type eType) {
		switch(eType) {
			default:
			case CSubBand::LL: return(0);
			case CSubBand::LH: return(1);
			case CSubBand::HL: return(2);
			case CSubBand::HH: return(3);
		}
	};

	void DecodeCodeBlocks(NCS::SDK::CBuffer2D::Type eType);

	UINT64 GetMemorySize(); // Against bug EMS-407, added 02/11/2011

protected:

		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetCBWidth()); 
		};
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetCBHeight()); 
		};
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetNumCBWide()); 
		};
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetNumCBHigh()); 
		};
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D * Ptr to node.
		 */
	virtual CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0);

private:
	UINT32 m_NumCBWide;
	UINT32 m_NumCBHigh;
};

}
}

#endif // !NCSJPCSUBBAND_H
