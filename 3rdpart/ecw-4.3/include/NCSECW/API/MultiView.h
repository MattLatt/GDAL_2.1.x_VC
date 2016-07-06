/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/


#ifndef NCSMULTIVIEW_H
#define NCSMULTIVIEW_H

#pragma warning(push)
#pragma warning(disable: 4251)

#include "NCSEcw/API/View.h"

namespace tbb {
	template <typename T> struct atomic;
}

namespace NCS {

class NCSECW_IMPEX CMultiView: public CView {
public:
	class CViewList;
	class Handle {
	public: 
		Handle() { m_id = (UINT64)-1; };
		virtual ~Handle() { release(); };
		void release() { m_pMV->ReleaseHandle(*this); };
	protected:
	friend class CMultiView;
		CMultiView *m_pMV;
		UINT64 m_id;
	};

	CMultiView();
	virtual ~CMultiView();

	virtual CError Close ( bool bFreeCache = false );

	virtual CError SetView(Handle &Id, UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0);
	virtual CError SetView (Handle &Id, INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
					   IEEE8 dWorldBRX, IEEE8 dWorldBRY );
	virtual CError SetView (Handle &Id,  INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   INT32 nDatasetTLX, INT32 nDatasetTLY,
					   INT32 nDatasetBRX, INT32 nDatasetBRY );
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, UINT8 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, UINT16 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, UINT32 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, UINT64 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, INT8 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, INT16 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, INT32 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, INT64 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, IEEE4 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, IEEE8 **ppOutputLine);
	virtual NCSEcwReadStatus ReadLineRGB(Handle Id, UINT8 *pRGBTriplet);
	virtual NCSEcwReadStatus ReadLineBGR(Handle Id, UINT8 *pBGRTriplet);
	virtual NCSEcwReadStatus ReadLineRGBA(Handle Id, UINT32 *pRGBA);
	virtual NCSEcwReadStatus ReadLineBGRA(Handle Id, UINT32 *pBGRA);
	virtual NCSEcwReadStatus ReadLineARGB(Handle Id, UINT32 *pARGB);
	virtual NCSEcwReadStatus ReadLineABGR(Handle Id, UINT32 *pABGR);
	virtual NCSEcwReadStatus ReadLineBIL(Handle Id, NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
	virtual NCSEcwReadStatus Read(Handle Id, SDK::CBuffer2DVector &Buffers);
protected:
	friend class Handle;
	virtual CError ReleaseHandle(Handle &Id);
private:
	CViewList *m_pViews;
	tbb::atomic<UINT64> *m_pNextId;
};

}

#pragma warning(pop)

#endif //NCSMULTIVIEW_H