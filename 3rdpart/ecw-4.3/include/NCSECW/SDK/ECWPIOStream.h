/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/


#ifndef NCSSDKECWPIOSTREAM_H
#define NCSSDKECWPIOSTREAM_H

#include "NCSTypes.h"
#include "NCSPacketIOStream.h"
#include "NCScnet.h"
#include "NCSEcw/ECW/NCSEcw.h"


#ifdef NOT_DEBUG
#define NCSDEBUGLOG1(X, Y) NCS::CLog::Log(__FILE__, __LINE__, CLog::LOG_LEVEL0, X, Y)
#define NCSDEBUGLOG2(X, Y, Z) NCS::CLog::Log(__FILE__, __LINE__, CLog::LOG_LEVEL0, X, Y, Z)
#else
#define NCSDEBUGLOG1(X, Y)
#define NCSDEBUGLOG2(X, Y, Z)
#endif

#if defined(_DEBUG)||defined(NCS_USE_DEBUG_SERVER)
#define NCS_IIS_DLL_NAME	"/ecwp/ecwpd.dll"
#else
#define NCS_IIS_DLL_NAME	"/ecwp/ecwp.dll"
#endif

namespace NCS {
namespace SDK {

#ifdef ECWP_CACHE
class CECWPCache;
#endif

/**
 * CECWPIOStream class - 64bit ECWP IO "Stream" class.
 * This class is the IO mechanism used to access JP2 files via ECWP.
 * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: rrogers $ $Date: 2009/03/17 $ 
 */
class NCSECW_IMPEX CECWPIOStream: public CMutex , public SDK::PacketIOStream {
public:
		/** Constructor, initialises members */
	CECWPIOStream(const CString &URLPath, const CString &HttpHeaders);

		/** Virtual destructor, releases members */
	virtual ~CECWPIOStream();

	virtual CError CECWPIOStream::Open(const CString &URLPath, bool bWrite);
	virtual CError Close();

	virtual CError Reconnect();

	virtual void RequestPacket(CPacketId id);
	virtual void CancelPacket(CPacketId id);

	virtual CError SendRequests();

	virtual void SetPacketStatus(CPacketId nPacket, CPacketStatus::Status eStatus);

	virtual void ProcessReceivedPackets();

	virtual bool ValidateIfReallyReceived(CPacketId nPacket) { return false; };
	virtual void TouchInternalBlock(CPacketId nPacket) { };

	virtual bool IsConnected()
	{
		return m_bIsConnected;
	}

	virtual bool IsPolling()
	{
		return m_bPollingConnection;
	}
	
	virtual NCS::CString GetHttpHeaders() { return m_sHttpHeaders; };

protected:
	virtual void ReceivedBlock(CPacketId nPacket, UINT8* p, UINT32 l) = 0;

	virtual CError OpenInternal(UINT8 **ppHeaderMemImage, 
								   UINT32 *pnHeaderMemImageLen, 
								   pNCSnetClient *pClientID, 
								   NCSClientUID		*pClientUID,
								   NCSnetPacketSentCB *pSentCB,
								   NCSnetPacketRecvCB *pRecvCB,
								   void *pCBData, 
								   wchar_t *szUrlPath, const CString &HttpHeaders);

	pNCSnetClient	m_ClientNetID;
	NCSClientUID	m_ClientUID;
	bool			m_bIsConnected;
	bool			m_bSendInProgress;
	NCSSequenceNr	m_nClientSequence;
	NCSPacket		*m_pSendPacket;

	CString m_sHttpHeaders;
	bool m_bPollingConnection;
	int m_nReconnections;

#ifdef ECWP_CACHE
	CECWPCache *m_pCache;
#endif

	typedef struct {
		NCSPacket *pPacket;
		INT32 nLength;
	} ReceivedPacket;
	std::list<ReceivedPacket*> m_ReceivedPackets;
	// TODO: improve mutexing (started but commented out because not tested)
	//CMutex m_mReceivedPackets;

		/** Precincts we need to request packets for */
	std::vector<CPacketId> m_RequestPackets;
		/** Precincts we need to cancel packets for */
	std::vector<CPacketId> m_CancelPackets;
	CMutex m_mRequestPackets;

	void PacketRecvCB(NCSPacket *pPacket, INT32 nLength, pNCSnetClient pClientNetID, NCSError eError);	 
	void PacketSentCB(NCSError eError, UINT8 *pLastPacketSent);			

	static void sPacketRecvCB(NCSPacket *pPacket, INT32 nLength, pNCSnetClient pClientNetID,
		CECWPIOStream *pIOStream, NCSError eError);	 
	static void sPacketSentCB(NCSError eError, UINT8 *pLastPacketSent, CECWPIOStream *pIOStream);
};

}} // namespace

#endif
