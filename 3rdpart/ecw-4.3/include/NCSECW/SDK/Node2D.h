/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Node2D.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CNode2D class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCNode2D_H
#define NCSJPCNode2D_H

#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif // NCSJPCBuffer2D_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace NCS {
namespace SDK {
	/**
	 * CNode2D class - the base JPC codestream marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CNode2D: public CBuffer2D, public CError {
public:
	class NCSECW_IMPEX CNode2DTracker {
	public:
		CNode2DTracker(char *pClassName, size_t nNodeSize);
		virtual ~CNode2DTracker();

		void Add();
		void Remove();
		void AddMem(UINT64 nSize);
		void RemoveMem(UINT64 nSize);

//		INT32 m_nNodesTotal;		// Total # nodes allocated
		INT64 m_nMemTotal;			// Total Mem allocated
		char	m_szNodeName[32];	// Name of this node
		INT32 m_nNodeSize;			// Size of a single node

//		static INT32 sm_nNodesTotal;	// Total # nodes allocated globally
		static INT64 sm_nMemTotal;		// Total # nodes allocated globally
//		static std::vector<CNode2DTracker*> *sm_pTrackers;
	};
		/** Default constructor, initialises members */
	CNode2D();
		/** Virtual destructor */
	virtual ~CNode2D();

	virtual CNode2D &operator=(const CNode2D&s);

	typedef UINT64 ContextID;

	class NCSECW_IMPEX Context {
	public:
		Context();
		virtual ~Context();
		virtual UINT32 NrInputs();
		virtual class CNode2D *GetInput(UINT32 n);
		virtual void SetInput(UINT32 n, CNode2D *pNode);
		virtual void SetInputBuffers(UINT32 nInput, UINT8 nBuffers);
		virtual CBuffer2D *GetInputBuffer(UINT32 nInput, UINT8 nBuffer);
		virtual CBuffer2D *FindInputBuffer(UINT32 nInput, UINT32 &nBuffer, CRect2D &r, bool &bFound, CBuffer2D::Type eType);
		virtual void AddInput(CNode2D *pNode);
		Context operator=(const Context &src);

		static ContextID AllocContextID();
	private:
		class NCSECW_IMPEX CNode2DInput {
		public:
			class CNode2D *m_pNode;
			CBuffer2DCache m_Buffers;

			virtual ~CNode2DInput();
		};
			/** Link to input nodes */
		std::vector<CNode2DInput>m_Inputs;

		static ContextID	sm_NextCtxID;
		static CMutex	sm_NextCtxIDMutex;
	};

	class NCSECW_IMPEX Contexts {
	public:
		typedef std::pair<ContextID, class Context*> Pair;
		typedef std::vector<Pair> ContextList;
		
		Contexts();
		virtual ~Contexts();

		Context *get(ContextID nCtx);
		void add(ContextID nCtx, Context *pCtx);
		void remove(ContextID nCtx);

	private:
		tbb::queuing_mutex *m_pMutex;
		ContextList m_Contexts;
	};

		/** 
		 * Link a node as the unput to this node.
		 * @param		nCtx		Read context
		 * @param		nInputs		Number of inputs to this node
		 * @param		pInput		First CNode2D to use as input to this node.
		 * @param		...			Additional inputs where required (eg, MCT)
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Link(ContextID nCtx, UINT16 nInputs, CNode2D *pInput, ...);
		/** 
		 * Add a node as an input to this node.
		 * @param		nCtx		Read context
		 * @param		pNode		Input node to add
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool AddInput(ContextID nCtx, CNode2D *pInput) { GetContext(nCtx)->AddInput(pInput); return(true); };
		/** 
		 * UnLink a node as the unput to this node.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs = 0);

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

	virtual bool ReadInputs(ContextID nCtx, UINT32 ninputs, ...);
		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent) { 
			nCtx;pSrc;iComponent;//Keep compiler happy
			return(false); 
		};

		/** 
		 * Get the input error 
		 * @param		nCtx		Context id for input
		 * @return		CError	CError value of the input to this node.
		 */
	CError GetError(ContextID nCtx);

protected:
/*	class ContextAutoPtr: public std::auto_ptr<Context> {
	public:
		ContextAutoPtr() {};
		ContextAutoPtr(Context *s): std::auto_ptr<Context>(s) {};
		ContextAutoPtr(const ContextAutoPtr &s) { 
			ContextAutoPtr P(s.get());
			*this = P;
		};
		~ContextAutoPtr() {};
	};
	std::map<ContextID, ContextAutoPtr> *m_pContext;
*/
	Contexts m_Contexts;

		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to retrive context
		 * @param		bAutoConstruct Autoconstruct a new context for this ContextID if one doesn't exist
		 * @return		Context*	Context for given ID.
		 */
	virtual Context *GetContext(ContextID nCtx, bool bAutoConstruct = true) {
	//template< class T > T *GetContext(ContextID nCtx, bool bAutoConstruct = true) {
				Context *pCtx = (Context*)m_Contexts.get(nCtx);
				if(NULL == pCtx && bAutoConstruct) {
					pCtx = (Context*)NewContext();
					m_Contexts.add(nCtx, pCtx);	
				}
				return(pCtx);
			};
	virtual Context *NewContext() {
				return(new Context());
			};
		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to set
		 * @param		pCtx		Context to set
		 * @return		Context*	Context for given ID.
		 */	
//	virtual void SetContext(ContextID nCtx, Context *pCtx);
private:
};

/**
 * CNode2DVector - vector of CNode2Ds.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
 */
typedef NCSECW_IMPEX std::vector<class CNode2D*> CNode2DVector;

}
}

#pragma warning(pop)

#endif // !NCSJPCNode2D_H

