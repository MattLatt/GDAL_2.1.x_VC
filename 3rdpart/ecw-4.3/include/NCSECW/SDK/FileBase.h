/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     FileBase.h $
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 03Aug04 tfl  Added integer member to NCSJP2PCSBox
**			 [02] 07Dec04 tfl  Moved GeodataUsage enum declaration to NCSECWClient.h
**			 [03] 17Dec04 tfl  Added static CFile::Shutdown() function called by CFile::Shutdown()
 *******************************************************/

#ifndef NCSFILEBASE_H
#define NCSFILEBASE_H

#ifndef NCSJP2DEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJP2DEFS_H

#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif // NCSECWCLIENT_H

// Our own 64bit IOSTREAM class
#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif //NCSJPCBuffer2D_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif //NCSJPCNode2D_H

#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif // NCSMUTEX_H

#include "NCSEcw/SDK/GlobalInfo.h"

namespace tbb {
	template <typename T> struct atomic;
}

#define ECW_COMPRESS_SDK_LIMIT_MASK			0xfe43218a

#pragma warning(push)
#pragma warning(disable:4251 4275)

namespace NCS {
class CView;

namespace SDK {
class CFileClient;

/**
 * CFileBase class - the JP2 file class.
 * Note, the JP2File class is a superbox!
 * The JP2File is the base shared file object used by multiple CFile classes to 
 * access a JP2 file.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: jshao $ $Date: 2011/12/05 $ 
 */
#ifndef SWIG
class NCSECW_IMPEX CFileBase: public CMutex, public NCS::CThreadPool::CJob {
public:
	/**
	 * CFileFactory class 
	 * The CFileFactory is a vector containing all the CFile object instances.
	 * There is only one instance of this class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: jshao $ $Date: 2011/12/05 $ 
	 */
#ifndef SWIG
	class NCSECW_IMPEX CFileFactory: public std::vector<CFileBase *>, public CThread {
	public:
			/** Default constructor, initialises ECW library */
		CFileFactory();
			/** Virtual destructor, clean up */
		virtual ~CFileFactory();

			/** 
			* Open method - returns existing instance if already open
			* @param		ppFile			Returned CFileBase instance pointer.
			* @param		pURLPath		Local/remote path/URL.
			* @return      CError		NCS_SUCCESS or error code on failure.
			*/
		CError Open(CFileClient *pClient, const CString &URLPath, bool bWrite, const CString &sECWPHttpHeaders);

			/** 
			* Open method - returns existing instance if already open
			* @param		ppFile			Returned CFileBase instance pointer.
			* @param		pStream			Stream to open file from.
			* @return      CError		NCS_SUCCESS or error code on failure.
			*/
		CError Open(CFileClient *pClient, CIOStream *pStream);

			/** Close */
		CError Close(CFileClient *pClient, bool bFreeCache = false);

		CError CloseAll();

		bool OverBudget(float factor = 1.0);
		void Purge(float factor = 1.0);
		NCS_INLINE CGlobalInfo &GlobalInfo() { return(m_EcwInfo); };
	private:
		virtual void Work(void *p);
		virtual CError RemoveFile(CFileBase *pFile);

		NCS::CThreadPool *m_pWorkThreadPool;

		CGlobalInfo m_EcwInfo;
		CEvent		m_Event;
		CMutex		m_PurgeMutex;

		NCSTimeStampMs m_tsLastCloseDelayTime;
	};
#endif //SWIG
		/** ECW FileInfo Structure */
	NCSFileViewFileInfoEx m_FileInfo;
		/** Number of refs to this file */
	INT32				m_nRefs;

	std::vector<class CFileClient *>	m_Clients;

		/** IOStream */
	CIOStream *m_pStream;
		/** Did we allocate the stream? */
	bool	m_bOurStream;
		/** Currently processing ReceiveBlock() */
	bool	m_bInReceiveBlock;

		/** 
		 * Get the memory image of the file.
		 * @param		pLength		Returned length of image
		 * @return      void *		NCSMalloc()'d pointer to image.
		 */
	virtual void *GetMemImage(UINT32 &nLength) = 0;
		/** 
		 * Get length of a packet in the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @return      UINT32		Packet length or 0 on failure.
		 */
	virtual UINT32 GetPacketLength(NCSBlockId nPacket) = 0;
		/** 
		 * Read a packet from the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @param		pLength		Returned length of packet (header+data)
		 * @return      void *		NCSMalloc()'d pointer to read packet (header+data).
		 */
	virtual void *GetPacket(NCSBlockId nPacket, UINT32 &nLength) = 0;
		/** 
		 * Get number of a packet in the file.
		 * @return      UINT32		Number of Packets or 0 on failure.
		 */
	virtual UINT32 GetNrPackets() = 0;

		/** 
		 * Set the compression parameters (dimensions, registration, bitdepth etc).
		 * @param		Info		FileInfo structure
		 * @return      CError	NCS_SUCCESS, or error on failure 
		 */
	virtual CError SetCompress(NCSFileViewFileInfoEx &Info) = 0;
		/** 
		 * Write the next line in BIL format into the file.
		 * @param		nCtx				Unique Context ID
		 * @param		pSrc				Source buffer
		 * @param		iComponent			Component index
		 * @return      bool				true on success, else false.
		 */
	virtual bool WriteLine(CNode2D::ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent) = 0;

		/** 
		 * Enable unlimited compression - NOTE YOU MUST COMPLY WITH THE EULA.
		 */
	static void SetOEMKey(char *szCompanyName, char *szKey);
	static bool GetOEMKey(char **szCompanyName, char**szKey);

	virtual bool OverBudget(float factor = 1.0);
		/** 
		 * Purge any unused buffers, codeblocks etc if memory useage exceeds acceptable limites
		 */
	virtual void Purge();

	virtual NCSFileType GetFileType() = 0;

	virtual NCSFileViewFileInfoEx *GetFileInfo() = 0;

		/**
		 * Shutdown the resources associated with CFileBase objects.
		 * Called only by CFile::Shutdown().
		 */
//	static void sShutdown(bool bUnLock = false);
//	static void sStartup(bool bLock = false);

#ifndef SWIG
	NCS_INLINE static CFileFactory &sFiles() { return(sm_Files); };
	NCS_INLINE static SDK::CGlobalInfo &sGlobalInfo() {	return(sm_Files.GlobalInfo()); };
	NCS_INLINE static SDK::CConfig &sConfig() { return(sm_Files.GlobalInfo().Config()); };
	NCS_INLINE static SDK::CStats &sStats() { return(sm_Files.GlobalInfo().Stats()); };
#endif

		/** 
		* Open method - returns existing instance if already open
		* @param		ppFile			Returned CFileBase instance pointer.
		* @param		pURLPath		Local/remote path/URL.
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	static CError sOpen(CFileClient *pClient, const CString &URLPath, bool bWrite, const CString &sECWPHttpHeaders=NCS_T(""));

		/** 
		* Open method - returns existing instance if already open
		* @param		ppFile			Returned CFileBase instance pointer.
		* @param		pStream			Stream to open file from.
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	static CError sOpen(CFileClient *pClient, CIOStream *pStream);

		/** Close */
	static CError sClose(CFileClient *pClient, bool bFreeCache = false);

	virtual bool ReceivedBlock(NCSBlockId nBlock);
	
	virtual CError Reconnect();
	
// ONLY HERE UNTIL C++ify ECW
			/** Static list of files */
	CFileFactory &Files() { return(sm_Files); };
	void RemoveClient(CFileClient *pClient);
	void PushClient(CFileClient *pClient);
	void _LockWorker();
	bool _TryLockWorker();
	void _UnLockWorker();

protected:
#ifndef SWIG
	static CFileFactory sm_Files;
#endif //SWIG

	virtual void Work();
	//bool m_bWorking;
	NCS::CMutex m_mWorking;

		/** Unlimited compression key - see SetKeySize() */
	static char *sm_szOEMKey;
	static char *sm_szCompanyName;

	friend class CFileFactory;
	friend class NCS::CView;

			/** Default constructor, initialises members */
	CFileBase();
		/** Virtual Destructor */
	virtual ~CFileBase();

		/** 
		 * Open the JP2 file from the specified stream.
		 * @param		Stream		Stream of JP2 file to open
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Open(CIOStream *pStream);
		/** 
		 * Close the JP2 file.
		 * @param		bFreeCache	Free the cache & delete file object if last close
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Close();

		/** 
		 * Purge unused memory at specified purgestage
		 * @param		eStage		Purge stage to free memory at
		 */
	virtual void PurgeMemory(CGlobalInfo::Purge eStage, float factor = 1.0) = 0;

	virtual CIOStream *CreateBlockStream(const CString &URLPath, const CString &HttpHeaders) = 0;

public:
		/** Last access for this file */
	tbb::atomic<NCSTimeStampMs>	*m_ptsLastAccess;  // Used in FileBaseFactory to Purge older files first
};
#endif

class NCSECW_IMPEX CFileClient {
public:
	CFileClient() { m_pFile = NULL; };
	virtual ~CFileClient() { if(m_pFile) CFileBase::sClose(this, true); };

		/** 
		 * Get underlying CFileBase pointer.
		 * @return      CFileBase*	Return pointer to the CFileBase instance.
		 */
#ifndef SWIG
	CFileBase *GetFile() { return(m_pFile); };
	void SetFile(CFileBase *p) { m_pFile = p; };
#endif
	bool Set() { return(m_Event.Set()); };
	bool Reset() { return(m_Event.Reset()); };
	bool Wait(NCSTimeStampMs tsTimeout = -1) { return(m_Event.Wait(tsTimeout)); };

	virtual bool ReceivedBlock(NCSBlockId nBlock) {return(false);};

	virtual void Work() = 0;
protected:
#ifndef SWIG
		/** Event - fired when something happens */
	CEvent	m_Event;
	CFileBase *m_pFile;

#endif //SWIG
};
}
}

#pragma warning(pop)

#endif // !NCSJP2FILE_H
