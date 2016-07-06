/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     File.h 
** CREATED:  21Mar06 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWFILE_H
#define NCSECWFILE_H

#ifndef NCSJP2DEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJP2DEFS_H

#ifndef NCSMULTILOCK_H
#include "NCSMultiLock.h"
#endif //NCSMULTILOCK_H

#ifndef NCSECW_H
#include "NCSEcw/ECW/NCSEcw.h"
#endif // NCSECW_H

#ifndef NCSFILEBASE_H
#include "NCSEcw/SDK/FileBase.h"
#endif //NCSFILEBASE_H

#ifndef NCSGLOBALINFO_H
#include "NCSEcw/SDK/GlobalInfo.h"
#endif //NCSGLOBALINFO_H

#include "NCSThreadPool.h"
#include "NCSEcw/JPC/Nodes/SubBand.h"

#include <vector>
#include <map>
#include <deque>

#include "NCSEcw/SDK/NodeTiler2D_T.h"

#include "NCSEcw/API/HeaderEditor.h"

namespace tbb {
	template <typename T> struct atomic;
}


//#define USE_SCATTER_PRECINCT_READ_TASKS
//#define USE_CRATTER_SUBBAND_READ_TASKS

// NOTE - if you define COMP_WORKER you will need to muck around with 
// the tbb stuff in InputNode.h to make sure it is "hidden" like the rest of it
// - it was not done initially because COMP_WORKER was not defined so not used.
//#define COMP_WORKER // Use multiple threads to read the YUV bands

#pragma warning(push)
#pragma warning(disable: 4275)

namespace NCS {
	namespace SDK {
//		class CFileBase;
		class CFileBase::CFileFactory;
		class CBlockFile;
	}
namespace ECW {
	class CECWPIOStream;
/**
 * CFile class - the ECW file class.
 * The ECWFile is the base shared file object used by multiple CNCSFile classes to 
 * access an ECW file.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
 */
class NCSECW_IMPEX CFile: public SDK::CFileBase {
public:
	class CCachedBlock;
	class CSubBand;
	class CPrecinct;
	//class CPrecinctListReadAccessor;
	class CResolution;
	class CComponent;
	class CPrecinctList;
	class CPrecinctListWriteAccessor;
	class CCachedBlockList;
	class COffsetCache;
	/*
	** The task scheduler is not currently used and is causing an assert
	** in debug mode.  See the following bug log for details: 
	** http://swengjira/browse/EMS-242 - "in debug mode you can often see an assert failed on exit"
	**
	** The tbb::task_scheduler_init class usage below appears to be incorrect - a static 
	** instance of tbb::task_scheduler_init is used and initialized with 1 thread.  
	**
	** The tbb::task_scheduler_init class reference states "For production code, the default value 
	** of -1 (automatic) should be used." in relation to the number of threads the task scheduler 
	** should be initialized with.
	**
	** The tbb::task_scheduler_init class reference also states "A thread may construct multiple 
	** task_scheduler_inits. Doing so does no harm because the underlying scheduler is reference counted."
	** So there may be no need to use a static instance of tbb::task_scheduler_init.
	*/
	//static tbb::task_scheduler_init sm_ts;

	// The type of request for a block in a file cache list
	typedef enum {
		BLOCK_INVALID	= 0,	// invalid block request
		BLOCK_REQUEST	= 1,	// Post a Request for the block
		BLOCK_CANCEL		= 2,	// Post a Cancel for the block
		BLOCK_RETURN		= 3		// Return the block in the cache list
	} NCSEcwBlockRequestMethod;

		// 00 10 20
		// 01 XX 21
		// 02 12 22
	typedef enum {
		NEIGHBOUR_00 = 0,
		NEIGHBOUR_10 = 1,
		NEIGHBOUR_20 = 2,
		NEIGHBOUR_01 = 3,
		NEIGHBOUR_21 = 4,
		NEIGHBOUR_02 = 5,
		NEIGHBOUR_12 = 6,
		NEIGHBOUR_22 = 7,
		SELF		 = 8
	} Neighbour;

	class NCSECW_IMPEX CSubBand: public SDK::CBuffer2D {
	public:
		UINT8	LEFT_BORDER;
		UINT8	RIGHT_BORDER;
		UINT8	TOP_BORDER;
		UINT8	BOTTOM_BORDER;

		// Required for ECWP
		bool Incomplete() { return m_bIncomplete; };
		void Incomplete( bool bIncomplete ) { m_bIncomplete = bIncomplete; };

		CSubBand();
		virtual ~CSubBand();
		virtual CSubBand &operator=(const CSubBand &s);

		NCS_INLINE virtual JPC::CSubBand::Type Type() { return(m_eSBType); };
		NCS_INLINE void Type(JPC::CSubBand::Type e) { 
				m_eSBType = e; 

				switch(e) {
					case JPC::CSubBand::LL:
							LEFT_BORDER = 0;
							TOP_BORDER = 0;
						break;
					case JPC::CSubBand::HL:
							RIGHT_BORDER = 0;
							TOP_BORDER = 0;
						break;
					case JPC::CSubBand::LH:
							LEFT_BORDER = 0;
							BOTTOM_BORDER = 0;
						break;
					case JPC::CSubBand::HH:
							RIGHT_BORDER = 0;
							BOTTOM_BORDER = 0;
						break;
				}
			};
		
		NCS_INLINE CPrecinct *Precinct() { return(m_pPrecinct); };
		NCS_INLINE void Precinct(CPrecinct *pPrecinct) { m_pPrecinct = pPrecinct; };
		NCS_INLINE CResolution *Resolution();// { return(Precinct()->Resolution()); };
		NCS_INLINE CComponent *Component();// { return(Resolution()->Component()); };
		NCS_INLINE CFile *File();// { return(Component()->File()); };

		NCS_INLINE bool Zero() { return(m_bIsZero); };//||GetPtr()==0); };
		NCS_INLINE void Zero(bool b) { m_bIsZero = b; };				

		NCS_INLINE bool HaveBorders() { return(m_bHaveBorders); };
		NCS_INLINE void HaveBorders(bool b) { m_bHaveBorders = b; };
		
		NCS_INLINE void SetupBorders(CPrecinct &P, CPrecinctListWriteAccessor acc_w[9]); 
		
		//void UnpackNeighbour(CPrecinct &P, Neighbour b); 
		//NCS_INLINE void SetupBorder(CPrecinct &P, Neighbour b); 
		NCS_INLINE bool CopyNeighbourOverlap(CPrecinct &P, Neighbour f, CPrecinctListWriteAccessor acc_w[9]); 
		NCS_INLINE bool CheckZeroNeighbourOverlap(CPrecinct &P, Neighbour f, CPrecinctListWriteAccessor acc_w[9]);  
		void _writePixel(bool bDeleting = false);

	protected:
		// ECWP: indicates that the required block was missing when the subband was unpacked
		bool m_bIncomplete;

		JPC::CSubBand::Type	m_eSBType;
		bool	m_bHaveBorders;
		bool	m_bIsZero;
		CPrecinct *m_pPrecinct;
	};

	class NCSECW_IMPEX CPrecinct: public SDK::CNode2D/*, public SDK::HashList_T<NCSBlockId, CPrecinct>::Item_T<NCSBlockId>*/ {
	public:

#ifdef USE_SCATTER_PRECINCT_READ_TASKS
		class CGetNeighbourTask: public tbb::task {
		public:
			CGetNeighbourTask(JPC::CSubBand::Type eSBType, CPrecinct *pPrecinct);
			virtual ~CGetNeighbourTask();
			virtual tbb::task *execute(void);
		protected:
			JPC::CSubBand::Type	m_eSBType;
			CPrecinct *m_pPrecinct;
		};
#endif
#ifdef USE_CRATTER_SUBBAND_READ_TASKS
		class CGetSubBandTask: public tbb::task {
		public:
			CGetSubBandTask(JPC::CSubBand::Type eSBType, CPrecinctListWriteAccessor acc_w[9]);
			virtual ~CGetSubBandTask();
			virtual tbb::task *execute(void);
		protected:
			JPC::CSubBand::Type	m_eSBType;
			CPrecinctListWriteAccessor *m_acc_w;
		};
#endif
		CPrecinct(CResolution *pResolution=NULL, CCachedBlock *pCB=NULL);
		CPrecinct(const CPrecinct &s);
		virtual ~CPrecinct();			

		virtual CPrecinct &operator=(const CPrecinct &s);

		bool Init(CResolution *pResolution, NCSBlockId nId);//CCachedBlock *pCB);
		bool HaveLL();
		bool HaveLL(bool b);
		bool InIDWT();
		bool InIDWT(bool b);

		NCS_INLINE CResolution *Resolution() { return(m_pResolution); };
		void NCS_INLINE Resolution(CResolution *pResolution) { m_pResolution = pResolution; };
		NCS_INLINE CComponent *Component();// { return(Resolution()->Component()); };
		NCS_INLINE CFile *File();// { return(Component()->File()); };

		void CachedBlock(CCachedBlock *p);

		NCS_INLINE NCSBlockId GetNeighbourId(Neighbour f);
		virtual CSubBand &GetLLInternal();
		virtual CSubBand &GetSubBandInternal(JPC::CSubBand::Type e);

		NCS_INLINE INT32 GetX() { return(m_nX); };
		NCS_INLINE INT32 GetY() { return(m_nY); };
		NCS_INLINE NCSBlockId GetId() { return(m_nId); };

		virtual CSubBand &SubBand(JPC::CSubBand::Type e, CPrecinctListWriteAccessor acc_w[9]);
		virtual CSubBand &GetLL(CPrecinctListWriteAccessor acc_w[9]);
		virtual CSubBand &GetHL(CPrecinctListWriteAccessor acc_w[9]);
		virtual CSubBand &GetLH(CPrecinctListWriteAccessor acc_w[9]);
		virtual CSubBand &GetHH(CPrecinctListWriteAccessor acc_w[9]);

		virtual void PreErase(void);

		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
#ifdef NODETILER2D_READCONST
		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent) const;
#endif
		static SDK::CBuffer2D *s_pTextureBuf;
	protected:
		tbb::atomic<int>	*m_pbHaveLL;
		tbb::atomic<int>	*m_pbInIDWT;
		class CResolution *m_pResolution;

	friend class CCachedBlock;
		CSubBand m_LL;
		CSubBand m_LH;
		CSubBand m_HL;
		CSubBand m_HH;

		bool m_bBlockHasLL;

		INT32 m_nX;
		INT32 m_nY;
		NCSBlockId m_nId;

#ifdef USE_SCATTER_PRECINCT_READ_TASKS
		tbb::queuing_mutex	*m_pMutex; // not needed unless doing precinct_read_tasks
		tbb::empty_task *m_NeighbourSignalList[9];
#endif
#ifdef USE_CRATTER_SUBBAND_READ_TASKS
		int				m_nSubBandSignals[4];
		tbb::empty_task *m_SubBandSignalList[4][4];
#endif
		//tbb::concurrent_queue<tbb::empty_task*> m_SignalList;
	};

	//typedef CPrecinctList::ReadAccessor CPrecinctListReadAccessor;
	//class CPrecinctList::write_accessor: public CPrecinctList::WriteAccessor {};	

	class NCSECW_IMPEX CComponent: public SDK::CNode2D {
	public:
		CComponent(CFile *pFile, UINT16 c);
		virtual ~CComponent();

		void Index(UINT32 nComponent) {	m_nComponent = nComponent; };
		UINT32 Index(void) { return(m_nComponent); };
		NCS_INLINE CFile *File() { return(m_pFile); }
		NCS_INLINE CResolution *Resolution(UINT8 r) { return(m_Resolutions[r]); }
		NCS_INLINE UINT8 NrResolutions() { return((UINT8)m_Resolutions.size()); }

		virtual bool Link(ContextID nCtx, UINT32 nResolution);
		virtual bool UnLink(ContextID nCtx, UINT32 nResolution);

		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

#ifdef COMP_WORKER
		typedef NCS::CThreadPool::CJob CWorkerJob;
		virtual void QueueJob(CWorkerJob *p) { m_Worker.QueueJob(p); };
	protected:
		NCS::CThreadPool m_Worker;
#else
	protected:
#endif
		
		UINT32 m_nComponent;
		std::vector<CResolution*> m_Resolutions;
		CFile *m_pFile;
	};

	/*class NCSECW_IMPEX CCachedBlockTask: public tbb::task {
	public:
		CCachedBlockTask(CCachedBlock *pBlock);
		virtual ~CCachedBlockTask();
		virtual tbb::task *execute(void);
	protected:
		CCachedBlock *m_pBlock;
	};*/

	class NCSECW_IMPEX CCachedBlock: public SDK::CRect2D/*, public CNCSThreadPool::CNCSThreadPoolJob*/ {
	public:
		//CNCSEvent	m_Event;
		CFile *m_pFile;
		NCSBlockId	m_nBlockNumber;	// block number being cached

		UINT32 m_nBlockX;
		UINT32 m_nBlockY;
		UINT32 m_nSidebands;
		
		UINT8	*m_pPacked;	// the packed ECW block that is currently cached (or NULL)
		UINT32	m_nPackedLength;	// Length of the packed block
		
		// Cache the unpacked data for ECWP
		class CUnpackedCache {
		public:
			CUnpackedCache();
			~CUnpackedCache();

			void Add(CCachedBlock *pCachedBlock, INT32 nBand, INT32 nSubBand, UINT8 *pData, INT32 nLength );
			void Free();

			UINT8 *Get(CCachedBlock *pCachedBlock, INT32 nBand, INT32 nSubBand );
		protected:
			typedef std::vector<UINT8 *> VecOfUINT8;
			typedef std::vector<VecOfUINT8> VecOfVecOfUINT8;
			
			VecOfVecOfUINT8 m_Unpacked;
			INT32 m_nLength; // always the same
		};
		CUnpackedCache m_Unpacked;
		tbb::atomic<INT64>	*m_pnUsageCount;		// number of times in use by FileViews
		tbb::atomic<INT64>	*m_pnHitCount;			// number of times block has been read during inverse DWT operations
		bool	m_bHasLL;
		bool	m_bUnPacking;
		bool	m_bAligned;			// Packed block is aligned
		bool	m_bRequested;			// only TRUE if block request HAS BEEN SENT to the server
									// (so FALSE in every other situation, even if block not loaded yet)

		void SetFile(CFile *pFile) { m_pFile = pFile; };
		bool Packed() { return(m_pPacked != NULL); };
		bool UnPacking() { return(m_bUnPacking); };
		void UnPacking(bool b) { m_bUnPacking = b; };

		CCachedBlock(CFile *pFile = NULL, NCSBlockId nBlockNumber = 0);
		CCachedBlock(const CCachedBlock &s);

		virtual CCachedBlock &operator=(const CCachedBlock &s);

		void Init(CFile *pFile, NCSBlockId nBlockNumber);
		virtual ~CCachedBlock();

		NCSError UnPack(CSubBand &S );
		void FreePacked();
		void FreeUnPacked();
		void SetPacked(UINT8*p, UINT32 l, bool bAlign);
		bool InUse();
	protected:
		//bool	m_bHaveUnpacked;		
	};

	typedef struct {
		UINT32		nLength;
		UINT64		nOffset;
	} NCSFileBlockOffsetEntry;

		/** 
		 * Get the memory image of the file.
		 * @param		pLength		Returned length of image
		 * @return      void *		NCSMalloc()'d pointer to image.
		 */
	virtual void *GetMemImage(UINT32 &nLength);
		/** 
		 * Get length of a packet in the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @return      UINT32		Packet length or 0 on failure.
		 */
	virtual UINT32 GetPacketLength(NCSBlockId nPacket);
		/** 
		 * Read a packet from the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @param		pLength		Returned length of packet (header+data)
		 * @return      void *		NCSMalloc()'d pointer to read packet (header+data).
		 */
	virtual void *GetPacket(NCSBlockId nPacket, UINT32 &nLength);
		/** 
		 * Get number of a packet in the file.
		 * @return      UINT32		Number of Packets or 0 on failure.
		 */
	virtual UINT32 GetNrPackets();

		/** 
		 * Set the compression parameters (dimensions, registration, bitdepth etc).
		 * @param		Info		FileInfo structure
		 * @return      CError	NCS_SUCCESS, or error on failure 
		 */
	virtual CError SetCompress(NCSFileViewFileInfoEx &Info);
		/** 
		 * Write the next line in BIL format into the file.
		 * @param		nCtx				Unique Context ID
		 * @param		pSrc				Source buffer
		 * @param		iComponent			Component index
		 * @return      bool				true on success, else false.
		 */
	virtual bool WriteLine(SDK::CNode2D::ContextID nCtx, SDK::CBuffer2D *pSrc, UINT16 iComponent);

		/** 
		 * Purge any unused buffers, codeblocks etc if memory useage exceeds acceptable limites
		 */
//	virtual void Purge();

	virtual NCSFileType GetFileType() { return(NCS_FILE_ECW); };

	virtual NCSFileViewFileInfoEx *GetFileInfo();

//	class CCachedBlockListHash {
//	public:
//		static bool equal( const NCSBlockId& j, const NCSBlockId& k ) { return(j == k); };
//		static size_t hash( const NCSBlockId& k ) { return((size_t)k); };
//	};
//	typedef tbb::concurrent_hash_map<NCSBlockId, CCachedBlock, CCachedBlockListHash> CCachedBlockList;

	QmfLevel	*m_pTopQmf;				// Pointer to the top level of the ECW QMF structure
	NCS::ECW::CFile *m_pOpacityFile;
	bool m_bReadOffsets;			// TRUE if the block offsets table has been read and is valid for the QMF
	bool m_bReadMemImage;			// TRUE if the NCSFile has a memory image of the header present
	bool m_bValid;					// File is currently valid (it has not changed on disk since open).
	// Client side information (not valid when file opened at the server end)
	NCSTimeStampMs	m_tLastSetViewTime;// Last time a SetView was done, used to decide when to purge files from cache
	bool		m_bAddTexture;		// TRUE if we need to add texture.
	CCachedBlockList	*m_pBlocks;

	UINT32				m_nPackets;
	SDK::CBuffer2D		m_ZeroBuffer;
	//CCachedBlockListIterator pLastBlock;
	NCSPacket			*m_pSendPacket;		// Pointer to Send Packet - only one packet allocated & kept for life of File open
	NCSClientUID	m_nClientUID;		// Unique UID for this client file opened
	NCSSequenceNr	m_nServerSequence; // current maximum sequence number read from back from the server
	NCSSequenceNr	m_nClientSequence; // current sequence number client has issued

//	CCachedBlock m_Level0ZeroBlock;// a level 0 zero block containing all zeros for all bands and sidebands
//	CCachedBlock m_LevelnZeroBlock;// a > level 0 zero block (has one less sideband than level 0) 
	//UINT8	*m_pLevel0ZeroBlock;		
	//UINT8	*m_pLevelnZeroBlock;		

	// Client network related information (not valid when file opened at the server end)
	pNCSnetClient	m_pClientNetID;		// client network layer file channel NetID
	//bool	m_bSendInProgress;			// if TRUE, a send pack (request and/or cancel) has been made, and callback will be made once complete
	//UINT32	m_nRequestsXmitPending;		// if non-zero, number of block read requests waiting to send
										// (NOT requests already sent that responses are waiting for)
	//UINT32	m_nCancelsXmitPending;		// if non-zero, number of block cancel requests waiting to sent
	UINT32	m_nUnpackedBlockBandLength;	// length of one band in an unpacked block (always unpacked out to max X&Y block size)
	bool m_bIsConnected;				// Are we still connected to the server serving this file /**[07]**/
	bool	m_bIsCorrupt;						// File is corrupt - displayed message for user
	COffsetCache *m_pOffsetCache;
	bool		m_bFileIOError; //[17]

	NCSTimeStampMs m_tsLastReconnectTry;  //[19] The time of the last attempt to reconnect to the IWS
	UINT32	m_nReconnectCount;
	NCS_INLINE INT32 NumComponents() { return (INT32)m_Components.size(); };
	NCS_INLINE CComponent *Component(UINT16 iComponent) { return(m_Components[iComponent]); };

	static NCSError EditReadInfo(char *pFilename, NCSEcwEditInfo **ppInfo);
	static NCSError EditWriteInfo(char *pFilename, NCSEcwEditInfo *pInfo, void (*pProgressFunc)(UINT64 nTotal, UINT64 nWritten, void *pClientData), BOOLEAN (*pCancelFunc)(void *pClientData), void *pClientData);

protected:
	friend NCS::SDK::CFileBase::CFileFactory;
	friend NCS::ECW::CECWPIOStream;
//	friend class JP2::CNCSJP2Reader;
//	friend class CNCSJP2Writer;
	friend NCS::SDK::CBlockFile;

	std::vector<CComponent*> m_Components;

		/** Default constructor, initialises members */
	CFile();
		/** Virtual Destructor */
	virtual ~CFile();

		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pFileName	Full path name of JP2 file to open
		 * @param		bWrite		If true, opens for writing.
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	//virtual CError Open(NCSTChar *pFileName, bool bWrite = false);
		
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
		 * How much memory in use for all ECW files
		 * @return      UINT64		Memory in use in bytes
		 */
	static UINT64 sMemoryInUse();

		/** 
		 * Purge unused memory at specified purgestage
		 * @param		eStage		Purge stage to free memory at
		 */
	virtual void PurgeMemory(SDK::CGlobalInfo::Purge eStage, float factor = 1.0);

	virtual UINT8 *ConstructZeroBlock(QmfLevel *pQmf,	UINT32 &nLength, bool bAligned);

	virtual bool GetFileBlockSize(NCSBlockId nBlock, UINT32 &nBlockLength, UINT64 &nBlockOffset );
	virtual UINT8 *ReadFileBlock(NCSBlockId nBlock, UINT32 &nBlockLength );

	CError CloseOpacityFile();
private:
	friend class CView;

	virtual CIOStream *CreateBlockStream(const CString &, const CString &);
};
}
}

#pragma warning(pop)

//FIXME
extern "C" NCS::ECW::CFile::CCachedBlock *NCScbmReadViewBlock(QmfRegionLevel	*pQmfRegionLevel, UINT32 nBlockX, UINT32 nBlockY);
extern "C" int unpack_start_line_block( QmfRegionLevel *p_level, UINT32 x_block,
							NCS::ECW::CFile::CCachedBlock *p_packed_block, UINT32	lines_to_skip);

#endif // !NCSECWFILE_H
