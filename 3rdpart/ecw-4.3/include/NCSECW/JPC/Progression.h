/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Progression.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CProgression class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPROGRESSION_H
#define NCSJPCPROGRESSION_H

#include "NCSDefs.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"

namespace NCS {
namespace JPC {
	/**
	 * CProgression class - Implements JPC progressions.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #3 $ $Author: ctapley $ $Date: 2011/12/09 $ 
	 */	
class NCSECW_IMPEX CProgression {
public:
		/** Current Layer being decoded */
	UINT16	m_nCurTile;
		/** Current Layer being decoded */
	UINT16	m_nCurLayer;
		/** Current Resolution being decoded */
	UINT8	m_nCurResolution;
		/** Current Component being decoded */
	UINT16	m_nCurComponent;
		/** Current Precinct X being decoded */
	UINT32	m_nCurPrecinctX;
		/** Current Precinct Y being decoded */
	UINT32	m_nCurPrecinctY;

		/** Default constructor, initialises members */
	CProgression();
		/** Virtual destructor */
	virtual ~CProgression();
	
		/** 
		 * Get the current POC marker PO if present.
		 * @return      CPOCMarker::ProgressionOrder	Current PO.
		 */
	CPOCMarker::ProgressionOrder *CurrentPO(class CTilePartHeader *pMainTP);
		/** 
		 * Increment the current POC marker PO if present and current PO is complete.
		 * @param		bComplete	Flag indicating if current PO is complete
		 * @return      CError	NCS_SUCCESS or error on failure.
		 */
	CError IncrementPO(CTilePartHeader *pMainTP, bool &bComplete);

		/** 
		 * Start the progression order from the beginning.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Start(CTilePartHeader *pMainTP);
	virtual CError Start(CTilePartHeader *pMainTP, INT32 nComponent, INT32 nResolution);

		/** 
		 * Increment the progression order from the current location, parsing packet headers in the process.
		 * @param		pData		Data for Step function
		 * @param		bComplete	Progression order is complete.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Iterate(CTilePartHeader *pMainTP, void *pData, bool &bComplete);

		/** 
		 * Perform the "Step" for this progression.
		 * @return      bool		true, or false on error & m_Error is set to actual error code.
		 */
	virtual bool Step(class CComponent *pComponent,
					  class CResolution *pResolution,
					  class CPrecinct *pPrecinct);
		/** 
		 * Perform the "StepRow" for this progression.
		 * The progression stepper can return true here to skip the entire row
		 * 
		 * @return      bool		true, or false on row not skipped.
		 */
	virtual bool StepRow(class CComponent *pComponent,
						 class CResolution *pResolution);

		/**
		 * Calculate the Precinct 'k' X from the current progression
		 * @return		INT32		Precinct 'k' X
		 */
	UINT32 CalculatePrecinctX(CTilePartHeader *pMainTP, class CComponent *pComponent, class CResolution *pResolution);
		/**
		 * Calculate the Precinct 'k' Y from the current progression
		 * @return		INT32		Precinct 'k' Y
		 */
	UINT32 CalculatePrecinctY(CTilePartHeader *pMainTP, class CComponent *pComponent, class CResolution *pResolution);
		
	PacketId CurrentPacket() { return(m_nCurPacket); };
	PacketId NextPacket() { return(m_nCurPacket++); };
	void SetPacket(PacketId n) { m_nCurPacket = n; }; 

	// Against bug EMS-407, added 21/10/2011
	//static bool AddResolutions(NCS::JPC::CJPC &JPC, int nBands,
	//	INT32 nDatasetTLX, INT32 nDatasetTLY, INT32 nDatasetBRX, INT32 nDatasetBRY, INT32 nSizeX, INT32 nSizeY);

	// Against bug EMS-407, added 27/10/2011
	//static void MarkTileVisibility(NCS::JPC::CJPC &JPC,  
	//	INT32 nDatasetTLX, INT32 nDatasetTLY, INT32 nDatasetBRX, INT32 nDatasetBRY, INT32 nSizeX, INT32 nSizeY);

protected:
		/** Current TX being decoded */
	INT32	m_nCurTX;
		/** Current TY being decoded */
	INT32	m_nCurTY;
		/** Current TX increment amount */
	INT32	m_nIncTX;
		/** Current TY increment amount */
	INT32	m_nIncTY;
	
		/** Current Packet Number */
	PacketId m_nCurPacket;

	// Data for step function
	void *m_pData;
	// Progression order is complete
	bool m_bDone;
	// Error code when Step() returns false
	CError m_Error;
};

}
}

#endif // !NCSJPCPROGRESSION_H
