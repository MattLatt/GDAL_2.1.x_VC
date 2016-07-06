/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     PacketLengthType.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPacketLengthType class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPACKETLENGTHTYPE_H
#define NCSJPCPACKETLENGTHTYPE_H

#include "NCSEcw/JPC/Markers/Marker.h"

	/**
	 * CPacketLengthType class - the JPC Packet Length Type.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CPacketLengthType {
public:
		/** Packet length - Header usually includes Data, except when using PPM or PPT markers
		  *
		  * m_nHeaderLength was changed to a UINT64 instead of UINT32 as it allows
		  * CNCSJPCPacketList::GenDynamicPLTs() to use "CumulativeLengths" which greatly
		  * increases the performance of CNCSJPCTilePartHeader::GetPacketHeader() and hence
		  * the JP2 index file generation for some JP2 files.
		  */
	UINT64	m_nHeaderLength;
		/** Packet data length - Only when using PPM or PPT markers */
	UINT32	m_nDataLength;

		/** Default constructor, initialises members */
	CPacketLengthType();

		/** 
		 * Parse the fields from the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      bool		true on success.
		 */
	NCS_INLINE bool Parse(NCS::CIOStream &Stream) {
			UINT32 nTmp = 0;
			bool bRet = false;
			UINT8 t8;

			while((bRet = Stream.ReadMSB(t8)) != FALSE) {
				nTmp = (nTmp << 7) | (t8 & 0x7f);
				if((t8 & 0x80) == 0) {
					break;
				}
			}
			m_nHeaderLength = nTmp;
			return(bRet);
		};
		/** 
		 * Parse the fields from a chunk of memory.
		 * @param		UINT8*		Memory buffer to parse length from.
		 * @param		&i			Index into memory buffer, incremented before return
		 * @return      bool		true on success.
		 */
#pragma warning(push)
#pragma warning( disable : 4127 )

	NCS_INLINE bool Parse(UINT8 *pBuf, INT32 &i) {
			UINT32 nTmp = 0;
			while(true) {
				UINT8 t8 = pBuf[i++];
				nTmp = (nTmp << 7) | (t8 & 0x7f);
				if((t8 & 0x80) == 0) {
					break;
				}
			}
			m_nHeaderLength = nTmp;
			return(true);
		};
#pragma warning(pop)
		/** 
		 * UnParse the fields to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      bool		true on success.
		 */
	NCS_INLINE bool UnParse(NCS::CIOStream &Stream) {
			UINT32 nMask = 0xffffffff;
			INT8 nShift = 0;
			bool bRet = true;

			while(nMask & m_nHeaderLength) {
				nMask <<= 7;
				nShift++;
			}
			if(nShift > 0) {
				nShift--;
				nMask >>= 7;
				nMask = 0x7f << (nShift * 7);
				while(nShift >= 0 && bRet) {
					bRet &= Stream.WriteMSB((UINT8)(((m_nHeaderLength & nMask) >> (nShift * 7)) | ((nShift > 0) ? 0x80 : 0x00)));
					nShift--;
					nMask >>= 7;
				}
			} else {
				// Zero length
				bRet &= Stream.WriteMSB((UINT8)0x00);
			}
			return(bRet);

		};
};


#endif // !NCSJPCPACKETLENGTHTYPE_H
