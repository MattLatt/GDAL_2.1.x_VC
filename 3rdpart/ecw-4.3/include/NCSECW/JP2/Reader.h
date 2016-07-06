/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Reader.h 
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CReader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJP2READER_H
#define NCSJP2READER_H

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H

#include "NCSEcw/JP2/File.h"

namespace NCS {
namespace JPC {
	class CResampler;
}
namespace JP2 {
/**
 * CReader class - the JP2 file view class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: jshao $ $Date: 2011/12/05 $ 
 */
class NCSECW_IMPEX CReader: public NCS::CView::CReaderBase {
public:
		/** Default constructor, initialises members */
	CReader(NCS::CView *pView);
		/** Virtual Destructor */
	virtual ~CReader();

		/** 
		 * Set a view into the ECW file for reading.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetView(UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0);

	virtual NCSEcwReadStatus Read(SDK::CBuffer2DVector &Buffers);
	virtual NCSEcwReadStatus ReadLine(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps);
	//virtual NCSEcwReadStatus ReadLine(NCSEcwCellType *pType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps); // temporarily disabled
	virtual bool ReceivedBlock(NCSBlockId nBlock);

	virtual void UpdateViewStats();
	virtual NCSFileType GetFileType() { return(NCS_FILE_JP2); };

		/** Copy the parameters from the m_Parameters vector to the File instance */
	virtual void SetFileParameters();

	virtual void SetParameterInternal(NCS::CView::Parameter eType, ...);
protected:
		/** JP2 resampler */
	JPC::CResampler *m_pInput;
};
}
}

#endif // !NCSJP2READER_H
