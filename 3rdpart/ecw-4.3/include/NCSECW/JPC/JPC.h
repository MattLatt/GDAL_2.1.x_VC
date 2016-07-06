/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:    JPC.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CJPCclass header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPC_H
#define NCSJPC_H

#ifndef NCSJPCMAINHEADER_H
#include "NCSEcw/JPC/MainHeader.h"
#endif // NCSJPCMAINHEADER_H
#ifndef NCSJPCTILEPARTHEADER_H
#include "NCSEcw/JPC/Nodes/TilePartHeader.h"
#endif // NCSJPCTILEPARTHEADER_H
#ifndef NCSFILEBASE_H
#include "NCSEcw/SDK/FileBase.h"
#endif // NCSFILEBASE_H

#ifndef NCSJPCFILEIOSTREAM_H
#include "NCSFileIOStream.h"
#endif // NCSJPSFILEIOSTREAM_H

#ifndef NCSBUFFEREDIOSTREAM_H
#include "NCSBufferedIOStream.h"
#endif // NCSBUFFEREDIOSTREAM_H

#ifndef NCSJP2EXTINDEXBOX_H
#include "NCSEcw/JP2/ExtIndexBox.h"
#endif //NCSJP2EXTINDEXBOX_H

#include <vector>
#include <map>

namespace NCS {
namespace JPC {

	/**
	 * CJPCclass - the JPC codestream class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2011/11/18 $ 
	 */	
class NCSECW_IMPEX CJPC: public CMainHeader, public SDK::CFileBase {
public:
	typedef enum {
			/** Profile 0 */
		BASELINE_0		= 0,
			/** Profile 1 */
		BASELINE_1		= 1,
			/** Profile 2 - unrestricted */
		BASELINE_2		= 2,
			/** NITF NPJE profile */
		NITF_BIIF_NPJE	= 3,
			/** NITF EPJE Profile */
		NITF_BIIF_EPJE	= 4
	} Profile;

	/** A vector of the first Tile Part for each tile */
	std::vector<CTilePartHeader *> m_Tiles;

	/** Current Tile-Part being decoded */
	CTilePartHeader	*m_pCurrentTilePart;
		/** Next tilepart index */
	UINT32	m_nNextZtp;

		/** performing low-memory compression */
	bool m_bLowMemCompression;

		/** Use external packet index */
	bool m_bExtPacketIndex;
		/** Currently building external packet index */
	bool m_bBuildExtPacketIndex;

	bool m_bOptimizeDecodingForMultiTile;
	bool m_bOptimizeDecodingForSinglePrecinct;
		/** External packet index stream */
	CFileIOStream	m_ExtPacketIndex;
	JP2::CExtIndexBox	m_ExtPacketIndexBox;

		/** Reconstruction parameter */
	IEEE4 m_fReconstructionParameter;

		/** Default constructor, initialises members */
	CJPC();
		/** Virtual destructor */
	virtual ~CJPC();

		/** 
		 * Parse the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(CIOStream &Stream);
		/** 
		 * UnParse the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(CIOStream &Stream);

		/**
		 * Get the Tile from the index and part index
		 * @param		INT32		Tile Index, >= 0, -1 == current tile main tile header
		 * @param		INT32		Tile Part Index, >= 0
		 * @return		CTilePartHeader*		Pointer to tile part
		 */
	CTilePartHeader *GetTile(INT32 iIndex = -1, INT32 iPart = 0);
		/** 
		 * Get length of a packet in the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @return      UINT32		Packet length or 0 on failure.
		 */
	UINT32 GetPacketLength(UINT32 nPacket);
	// Get number of a packet in the file.
	UINT32 GetNrPackets();

		/** 
		 * Read a packet from the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @param		pLength		Returned length of packet (header+data)
		 * @return      void *		NCSMalloc()'d pointer to read packet (header+data).
		 */
	void *GetPacket(UINT32 nPacket, UINT32 *pLength);
		/** 
		 * Get packet header for the specified absolute packet index.
		 * @param		nPacket		Absolute Packet number 
		 * @return      class CPacket*		Packet header
		 */
	class CPacket *GetPacketHeader(UINT32 nPacket, bool bPurge = true);

		/** 
		 * Get sequential packet header for the specified absolute and relative packet index.
		 * @param		nPacket		Absolute Packet number 
		 * @return      class CPacket*		Packet header
		 */
	class CPacket *GetSequentialPacketHeader(UINT32 &ioStartTileNr, UINT32 &ioRelativePacketNr, bool bPurge = true); // Against bug EMS-407, added 26/10/2011

		/** 
		 * Fnid the RCPL for the specified packet index 
		 * @param		nPacket		Absolute Packet number 
		 * @param		nTile		Tile index 
		 * @param		nResolution Resolution number
		 * @param		nComponent	Component index 
		 * @param		nPrecinctX	Precinct X index 
		 * @param		nPrecinctY  Precinct Y index 
		 * @param		nLayer		Quality Layer
		 * @return      bool 		true, or false on failure
		 */
	bool FindPacketRCPL(UINT32 nPacket, UINT16 &nTile, UINT8 &nResolution, UINT16 &nComponent, UINT32 &nPrecinctX, UINT32 &nPrecinctY, UINT16 &nLayer);

	// TODO: This could probably be better implemented as a CIOStream subclass that has a 
	// bool member that with an assignment operator that sets it to true and a destructor
	// that sets it to false.  See tbb hashlist items
	class CStreamPool
	{
	public:
		CIOStream *m_pStream;
		bool m_bInUse;
		CStreamPool();
		~CStreamPool();
	};
	CStreamPool *GetStream();
	CStreamPool *GetExtPacketIndexStream();
	virtual void QueueJob(NCS::CThreadPool::CJob *p) { m_Worker.QueueJob(p); };

private:
	CMutex m_StreamPoolMutex;
	CMutex m_ExtPacketIndexStreamPoolMutex;

	std::vector<CStreamPool> m_StreamPool;
	std::vector<CStreamPool> m_ExtPacketIndexStreamPool;
	NCS::CThreadPool m_Worker;
};
}
}

#endif // !NCSJPC_H
