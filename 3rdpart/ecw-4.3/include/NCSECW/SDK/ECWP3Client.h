#ifndef ECWP3_CLIENT_H
#define ECWP3_CLIENT_H

#include "NCSUtil.h"
#include "NCSThread.h"
#include "NCSThreadPool.h"
#include "NCSMultiLock.h"

#include "NCScnet.h"
#include "NCSMd5.h"

#include "NCSMemoryIOStream.h"

#include <vector>
#include <list>

#include "NCSEcw/SDK/ecwp3packets.h"

//#define NCS_USE_CURL

#ifdef NCS_USE_CURL
typedef void CURL;
#endif

#define ECWP3_CLIENT_VERSION 1
#define MANUAL_LL

namespace NCS { namespace SDK {
	class CECWP3Client;
	class CBlockFile;
} }

namespace NCS {
namespace SDK {

class NCS_EXPORT CECWP3Client {
public:
	class NCS_EXPORT IECWP3ClientApp {
	public:
		virtual void BlockRecv(NCSBlockId nID, void *pBlock, UINT32 nBlockLength, NCSError eError) = 0;
		virtual void BlockFailed(NCSBlockId nID, NCSError eError) = 0;
	};

	class CPacket {
	public:
		CPacket() : m_ePacketType(), m_nPacketSize(0), m_nServerVersion(0) {};
		virtual ~CPacket() {};
		virtual bool Read(CMemoryIOStream &Stream) {
			UINT16 nPreamble = 0;
			return(Stream.Read((void*)&nPreamble, sizeof(UINT16)) && nPreamble == ECWP3_PACKET_PREAMBLE &&
					Stream.Read((void*)&m_ePacketType, sizeof(UINT8)) &&
					Stream.Read((void*)&m_nServerVersion, sizeof(UINT8)) &&
					Stream.Read((void*)&m_nPacketSize, sizeof(UINT32)));
		}
		NCSecwp3PacketType m_ePacketType;
		UINT8 m_nServerVersion;
		UINT32 m_nPacketSize;
		static UINT16 m_nPreamble;
	};

	class CErrorResponsePacket : public CPacket {
	public:
		CErrorResponsePacket() : CPacket(), m_eErrorType() {};
		virtual ~CErrorResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_ERROR_RESPONSE) {
				return(Stream.Read((void*)&m_eErrorType, sizeof(UINT8)) &&
						Stream.Read((void*)&m_nData, sizeof(UINT32)));
			}
			return false;
		}
		NCSecwp3ErrorType m_eErrorType;
		UINT32 m_nData;

		static CErrorResponsePacket *Parse(CMemoryIOStream &Stream) {
			CErrorResponsePacket *pPacket = new CErrorResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CGetResponsePacket : public CPacket {
	public:
		CGetResponsePacket() : CPacket(), m_pBlock(NULL), m_nBlockID(0), m_nBlockSize(0) {};
		virtual ~CGetResponsePacket() {
			if(m_pBlock) {
				delete m_pBlock;
			}
		}

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_GET_RESPONSE) {
				bool bRet = false;
				if(Stream.Read((void*)&m_nBlockID, sizeof(UINT32)) && 
					Stream.Read((void*)&m_nBlockSize, sizeof(UINT32))) {
					
					m_pBlock = NCSMalloc(m_nBlockSize, FALSE);
					bRet = Stream.Read((void*)m_pBlock, m_nBlockSize);
				}
				return bRet;
			}
			return false;
		}
		static CGetResponsePacket *Parse(CMemoryIOStream &Stream) {
			CGetResponsePacket *pPacket = new CGetResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
		void *m_pBlock;
		UINT32 m_nBlockID;
		UINT32 m_nBlockSize;
	};

	class COpenResponsePacket : public CPacket {
	public:
		COpenResponsePacket() : CPacket(), m_pHeader(NULL), m_nHeaderSize(0), m_nCID(0), m_eFileType(ECWP3_FILETYPE_UNKNOWN) {};
		virtual ~COpenResponsePacket() {
			// header memory is managed by parent class
		}

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_OPEN_RESPONSE) {
				if(Stream.Read((void*)&m_nCID, sizeof(UINT64))) {
					UINT8 eFileType = 0;
					if(Stream.Read((void*)&eFileType, sizeof(UINT8))) {
						m_eFileType = (NCSecwp3FileType)eFileType;
						if(Stream.Read((void*)&m_nHeaderSize, sizeof(UINT32))) {
							m_pHeader = NCSMalloc(m_nHeaderSize, FALSE);
							return(Stream.Read((void*)m_pHeader, m_nHeaderSize));
						}
					}
				}
			}
			return false;
		}
		UINT64 m_nCID;
		void *m_pHeader;
		UINT32 m_nHeaderSize;
		NCSecwp3FileType m_eFileType;

		static COpenResponsePacket *Parse(CMemoryIOStream &Stream) {
			COpenResponsePacket *pPacket = new COpenResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CReconnectResponsePacket : public CPacket {
	public:
		CReconnectResponsePacket() : CPacket(), m_nCID(0) {};
		virtual ~CReconnectResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_RECONNECT_RESPONSE) {
				return(Stream.Read((void*)&m_nCID, sizeof(UINT64)));
			}
			return false;
		}
		UINT64 m_nCID;

		static CReconnectResponsePacket *Parse(CMemoryIOStream &Stream) {
			CReconnectResponsePacket *pPacket = new CReconnectResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CCloseResponsePacket : public CPacket {
	public:
		CCloseResponsePacket() : CPacket(), m_nSuccess(0) {};
		virtual ~CCloseResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_CLOSE_RESPONSE) {
				return(Stream.Read((void*)&m_nSuccess, sizeof(UINT8)));
			}
			return false;
		}
		UINT8 m_nSuccess;

		static CCloseResponsePacket *Parse(CMemoryIOStream &Stream) {
			CCloseResponsePacket *pPacket = new CCloseResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	CECWP3Client(IECWP3ClientApp *pApp);
	CECWP3Client(const CECWP3Client &rhs);
	virtual ~CECWP3Client();

	bool Open(NCS::CString sURL);
	bool Reconnect();
	bool IsConnected() { return m_bIsConnected; };
	bool Close();

	bool Request(std::vector<CPacketId> *pBlocks);
	bool Request(CPacketId nID);
	bool Cancel(CPacketId nID);
	bool SendRequests();

	void *GetHeader() { return m_pHeader; };
	int GetHeaderSize() { return m_nHeaderSize; };

	static INT32 GetQueueSize();

	NCSecwp3FileType GetFileType() { return m_eFileType; };

	static void Init();
	static void Fini();

	static bool SetMaxConcurrentDownloads(INT32 nThreadCount);

protected:

	class NCS_EXPORT CConnectionSwarm : public NCS::CThread {
	public:

		class NCS_EXPORT CConnection : public NCS::CThreadPool::CJob {
		public:
			CConnection();
			CConnection(NCS::CString &sServer);
			CConnection(NCS::CHashKey &Key);
			CConnection(const CConnection &s);
			virtual ~CConnection();

			virtual CConnection &operator=(const CConnection &s);

			void Enqueue(NCS::CString &sURL, CECWP3Client *pClient, NCSecwp3ServerCommand eCurrentRequestType);
			NCS::CHashKey Key() { return m_Key; };
			void Work();
			static int GetRequestCB(void *szNew, unsigned long nSize, void *szAllData, void *pUserData);
			bool IsAlive() { return m_bIsAlive; };			

			typedef std::map<NCS::CPacketId, bool> CGetBlockChecklist;
			CGetBlockChecklist *m_pGetBlockReceived;
			CGetBlockChecklist *m_pGetBlockCancel;
			
			CECWP3Client *m_pClient;
			NCS::CString m_sURL;

			NCSecwp3ServerCommand m_eCurrentRequestType;

			bool CancelBlock(NCS::CPacketId ID);
			bool AbortDownloadCheck();
			bool Alive() { return m_bIsAlive; };

			void Touch() { m_tsLastAccess = NCSGetTimeStampMs(); };
			NCSTimeStampMs LastAccess() { return m_tsLastAccess; }; 
		protected:
			HINTERNET m_hSession;
			HINTERNET m_hConnection;
			NCS::CHashKey m_Key;

			NCS::CMutex m_BlockMutex;
			
			bool m_bIsAlive;
			NCSTimeStampMs m_tsLastAccess;
		};

		CConnectionSwarm(int nSwarmSizePerServer);
		virtual ~CConnectionSwarm();

		CConnection *GetConnection(NCS::CString &sServer);
		void ReturnConnection(CConnection *Connection);

		void Work(void *pData);

	protected:
		int m_nSwarmSizePerServer;
		class CConnectionQueue;
		class CConnectionMap;
		CConnectionMap *m_pConnections;
	};
	CConnectionSwarm::CConnection *GetNextPacketsJob(size_t nMaxSize);
	std::vector<CConnectionSwarm::CConnection *> m_CurrentDownloadJobs;
	NCS::CMutex m_mCurrentDownloadJobs;

	class CDownloadScheduler;

	static wchar_t *s_pAcceptTypes[];

	class CDownloadScheduler : public NCS::CThreadPool
	{
	public:
		CDownloadScheduler( UINT32 nMaxThreads = 10, bool bStartThreads = false, bool bPoolIsJobOwner = true,
			bool bCheckIfAlreadyQueued=false, bool bCheckIfRunning=false );
		virtual ~CDownloadScheduler();

		bool UpdateThreadCount(INT32 nThreadCount);
		void RegisterClient(CECWP3Client *pClient);
		void UnRegisterClient(CECWP3Client *pClient);

		virtual inline void WorkFinished( CJob *pJob );
	protected:
		virtual inline void QueueJobInt( CJob *pJob, bool bHighPriority );
		virtual inline CJob *GetNextJobInt();

		std::vector<CECWP3Client *> m_Clients;
		NCS::CMultiLock m_Mutex;
		size_t m_nClient;
		size_t m_nBlocksToRequest;
	};

	static CDownloadScheduler *s_pDownloadSchedular;
	static CConnectionSwarm *s_pConnectionSwarm;
	static bool s_bWininetInitialised;

	bool ParseURL();
	void SetConnected(bool bIsConnected) { m_bIsConnected = bIsConnected; };
	bool m_bIsConnected;

	void *m_pHeader;
	UINT32 m_nHeaderSize;

	UINT64 m_nClientID;
	UINT32 m_nServerVersion;
	
	NCS::CMultiLock m_mMutex;
	NCS::CMultiLock m_mReconnectMutex;

	std::list<CPacketId> m_RequestBlocks;
	NCS::CMutex m_mRequestBlocks;

	CString m_sImageURL;
	CString m_sServerName;
	CString m_sServerURL;
	CString m_sLayerPath;

	CString m_sUsername;
	CString m_sPassword;
	CString m_sCredentials;

	bool m_bAnonymous;
	bool m_bUseSSL;

	static NCS::CMutex s_mMutex;

	NCSecwp3FileType m_eFileType;

	IECWP3ClientApp *m_pApp;
#ifdef USE_LOCAL_BLOCK_FILE
	NCS::SDK::CBlockFile *m_pBlockFile;
#endif
};

} // SDK
} // NCS

#endif