/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Resolution.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CResolution class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRESOLUTION_H
#define NCSJPCRESOLUTION_H

#ifndef NCSJPCPRECINCT_H
#include "NCSEcw/JPC/Nodes/Precinct.h"
#endif // NCSJPCPRECINCT_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSJPCPLTMARKER_H
#include "NCSEcw/JPC/Markers/PLTMarker.h"
#endif // NCSJPCPLTMARKER_H

namespace NCS {
namespace JPC {
	/**
	 * CResolution class - the JPC Resolution level.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #3 $ $Author: jshao $ $Date: 2011/12/05 $ 
	 */	
class NCSECW_IMPEX CResolution: public SDK::CNodeTiler2D {
public:
		/** Component for this resolution */
	class CComponent *m_pComponent;
		/** Resolution level */
	UINT8	m_nResolution;
		/** Compression target ratio */
	UINT16	m_nCompressionRatio;

		/** Precincts */
	CPrecinctMap m_Precincts;
	//std::vector<CPrecinct*> m_Precincts;
		
//	CIOStreamVector m_EncoderFiles;
	//char			*m_pEncoderFileName;
	UINT32			m_nEncoderPLTStartIndex;
//	std::vector<UINT32>	m_EncoderPLTIndex;
//	CIOStreamVector m_EncoderPLTFiles;
	//char			*m_pEncoderPLTFileName;

		/** 
		 * constructor 
		 * @param pComponent	Parent component
		 * @param nResolution	Resolution level, 0-32
		 */
	CResolution(class CComponent *pComponent, UINT8 nResolution);
		/** Virtual destructor */
	virtual ~CResolution();

		/** 
		 * Get Precinct Width.
		 * @return      UINT32		Un-clipped Width of precincts.
		 */
	UINT32 NCS_INLINE GetPrecinctWidth() {
				return(m_PrecinctWidth);
			};
		/** 
		 * Get Precinct Height.
		 * @return      UINT32		Un-clipped height of precincts.
		 */
	UINT32 NCS_INLINE GetPrecinctHeight() {
				return(m_PrecinctHeight);
			};
		/** 
		 * Get number of precincts wide.
		 * @return      INT32		Nr of precincts across.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsWide() {
				return(m_NumPrecinctsWide);
			};
		/** 
		 * Get number of precincts high.
		 * @return      UINT32		Nr of precincts high.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsHigh() {
				return(m_NumPrecinctsHigh);
			};
		/** 
		 * Get number of precincts area (wide x high).
		 * @return      UINT32		Nr of precincts area.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsArea() {
				return(m_NumPrecinctsArea);
			};

		/** 
		 * Get codeblock Width.
		 * @return      UINT32		Un-clipped Width of codeblocks.
		 */
	UINT32 NCS_INLINE GetCBWidth() {
				return(m_CBWidth);
			};
		/** 
		 * Get codeblock Height.
		 * @return      UINT32		Un-clipped height of codeblocks.
		 */
	UINT32 NCS_INLINE GetCBHeight() {
				return(m_CBHeight);
			};	
		/** 
		 * Get CPRL/PCRL/RPCL Progression resolution multiplier.
		 * @return      INT32		Progression resolution multiplier.
		 */
	INT32 NCS_INLINE GetResMultiplier() {
				return(m_ResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression XRsiz multiplier.
		 * @return      INT32		Progression XRsiz multiplier.
		 */
	INT32 NCS_INLINE GetXRsizResMultiplier() {
				return(m_XRsizResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression YRsiz multiplier.
		 * @return      INT32		Progression YRsiz multiplier.
		 */
	INT32 NCS_INLINE GetYRsizResMultiplier() {
				return(m_YRsizResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression DivX factor.
		 * @return      INT32		Progression DixX Factor.
		 */
	INT32 NCS_INLINE GetDivX() {
				return(m_DivX);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression DivY factor.
		 * @return      INT32		Progression DixY Factor.
		 */
	INT32 NCS_INLINE GetDivY() {
				return(m_DivY);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression X origin check OK test result.
		 * @return      INT32		Progression DixX Factor.
		 */
	bool NCS_INLINE XOriginCheckOk() {
				return(m_XOriginCheckOk);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression Y origin check OK test result.
		 * @return      INT32		Progression DixY Factor.
		 */
	bool NCS_INLINE YOriginCheckOk() {
				return(m_YOriginCheckOk);
			};

		/** 
		 * Get X0 of top left precinct for this resolution.
		 * @return      INT32		Coordinate value.
		 */
	INT32 NCS_INLINE GetPX0() {
				return(m_PX0);
			};
		/** 
		 * Get Y0 of top left precinct for this resolution.
		 * @return      INT32		Coordinate value.
		 */
	INT32 NCS_INLINE GetPY0() {
				return(m_PY0);
			};

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);

		/** 
		 * UnLink input nodes.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs = 0);

	virtual bool WritePrecinctLayerPLT(CJPC &JPC, CIOStream &Stream, CTilePartHeader &TilePart, CPLTMarker &PLT, CPacketLengthType &Len, INT64 &nSOTOffset, UINT32 &nBytes, UINT32 p, UINT32 l);
	virtual bool WriteLayerPLTs(CJPC &JPC, CIOStream &Stream, CTilePartHeader &TilePart, CPLTMarker &PLT, CPacketLengthType &Len, INT64 &nSOTOffset, UINT32 &nBytes, UINT32 l);
	virtual bool WritePrecinctLayerPacket(CJPC &JPC, CIOStream &Stream, UINT32 p, UINT32 l);
	virtual bool WriteLayerPackets(CJPC &JPC, CIOStream &Stream, UINT32 l);

	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets();	// Against bug EMS-407, added 02/11/2011
	bool PrecinctValidation();	// added on 03/12/2011

protected:

	CBuffer2D	m_EncoderLL;
	CBuffer2D	m_EncoderLH;
	CBuffer2D	m_EncoderHL;
	CBuffer2D	m_EncoderHH;

	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
			/** 2-line Output Buffer */
		CBuffer2D m_OutputBuffer;

		SDK::CBuffer2DCache	m_State;
		INT32			m_nStateU0;
		INT32			m_nStateU1;
		INT32			m_nStateV;

		CBuffer2D		m_IL2D_L;
		CBuffer2D		m_IL2D_H;
		Context();
		virtual ~Context();
	};

		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to retrive context
		 * @param		bAutoConstruct Autoconstruct a new context for this ContextID if one doesn't exist
		 * @return		Context*	Context for given ID.
		 */
	//virtual CNode2D::Context *GetContext(ContextID nCtx, bool bAutoConstruct = true);
	virtual CNode2D::Context *NewContext() {
				return((CNode2D::Context*)new Context());
			};

		/** 
		 * Read a BufferType line from the given subband.
		 * @param		nCtx		Read Context
		 * @param		nX			X coordinate of node within it's resolution rect.
		 * @param		nY			Y coordinate of node within it's resolution rect.
		 * @param		nWidth		Number of pixels to read.
		 * @param		eType		Buffer type
		 * @param		pBuffer		Pointer to destination buffer.
		 * @param		eSubBandType SubBand index enum.
		 * @return      bool		true on succes, else false.
		 */
	bool ReadSubBandLine(ContextID nCtx, CBuffer2D *pDst, CSubBand::Type eSubBandType);
		/** 
		 * Write a BufferType line to the given subband.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		eSubBandType SubBand index enum.
		 * @return      bool		true on succes, else false.
		 */
	bool WriteSubBandLine(ContextID nCtx, CBuffer2D *pSrc, CSubBand::Type eSubBandType);
		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetPrecinctWidth()); 
		};
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetPrecinctHeight()); 
		};
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetNumPrecinctsWide()); 
		};
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0) { 
			iComponent;//Keep compiler happy
			return(GetNumPrecinctsHigh()); 
		};
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D * Ptr to node.
		 */
	virtual __inline CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0) {
					iComponent;//Keep compiler happy
					return NULL;
					//return(m_Precincts.find(nNode));
				}
	virtual bool INTERLEAVE_2D(ContextID nCtx,
								INT32 u0,
								INT32 u1,
								INT32 v0,
								INT32 v1,
								CBuffer2D &a);
	virtual bool SR_2D(ContextID nCtx,
					  INT32 u0,
					  INT32 u1,
					  INT32 v0,
					  INT32 v1,
					  CBuffer2D &Dst);
	virtual bool HOR_SR(ContextID nCtx,
						CBuffer2D &a,
						INT32 u0,
						INT32 u1,
						INT32 v0,
						INT32 v1);
	virtual CBuffer2D *GET_STATE_BUFFER(ContextID nCtx,
											INT32 u0, 
											INT32 u1, 
											INT32 v,
											CBuffer2D::Type eType);
	virtual bool VER_SR_INPUT2(ContextID nCtx,
							  INT32 u0, 
							  INT32 u1,
							  CBuffer2D *pDst,
							  CBuffer2D::Type eType);
	virtual bool VER_SR(ContextID nCtx,
						INT32 u0,
						INT32 u1,
						INT32 v0,
						INT32 v1,
						CBuffer2D &Dst);

	virtual bool SD_2D_OUTPUT4(ContextID nCtx, 
							   INT32 u0, 
							   INT32 u1, 
							   INT32 v,
							   CBuffer2D::Type eType);
	virtual bool SD_2D(ContextID nCtx,
					   CBuffer2D *pSrc);
private:
	UINT32 m_CBWidth;
	UINT32 m_CBHeight;
	UINT32 m_PrecinctWidth;
	UINT32 m_PrecinctHeight;
	INT32 m_NumPrecinctsWide;
	INT32 m_NumPrecinctsHigh;
	INT32 m_NumPrecinctsArea; // added on 27/10/2011

	INT32 m_ResMultiplier;
	INT32 m_XRsizResMultiplier;
	INT32 m_YRsizResMultiplier;
	INT32 m_DivX;
	INT32 m_DivY;

	bool m_XOriginCheckOk;
	bool m_YOriginCheckOk;
	INT32 m_PX0;
	INT32 m_PY0;

	tbb::spin_mutex *m_pMutex_Res; // Against bug EMS-407, added 30/11/2011

};
}
}

#endif // !NCSJPCRESOLUTION_H
