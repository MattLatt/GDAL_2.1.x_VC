/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     View.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 07Sep04 tfl  Added support for configuring georeferencing use
**			 [02] 11Nov04 tfl  Added rotation member to ensure rotation data persists
**			 [03] 19Jan06 tfl  Changed tiled views vector to a single view-strip member
 *******************************************************/

#ifndef NCSVIEW_H
#define NCSVIEW_H

#pragma warning(push)
#pragma warning(disable: 4251)

#include "ECWJP2BuildNumber.h"

#ifndef NCSJPCDEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJPCDEFS_H

#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif // NCSECWCLIENT_H

#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif // NCSUTIL_H
#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif // NCSJPCBuffer2D_H
#ifndef NCSJP2BOX_H
#include "NCSEcw/JP2/Box.h"
#endif // NCSJP2BOX_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSFILEBASE_H
#include "NCSEcw/SDK/FileBase.h"
#endif // NCSJPCNode2D_H

#include "NCSEvent.h"

#ifdef ECW_COMPRESS_RW_SDK_VERSION
#include "NCSEcw/ECW/Compress.h"
#endif

namespace NCS {
	class CView;
	class CBlockIdQueue;
}

typedef NCSEcwReadStatus (*NCSRefreshCallback)( NCS::CView *pFile);

namespace NCS {
namespace ECW {
	class CReader;
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	class CWriter;
#endif
}
namespace JP2 {
	class CReader;
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	class CWriter;
#endif
}
namespace JPC {
	class CResampler;
}
namespace SDK {
	class CFileBase;
}

/**
 * CFile class - the JP2 file view class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #6 $ $Author: scope $ $Date: 2007/12/14 $ 
 */
class NCSECW_IMPEX CView: public SDK::CFileClient {
public:
		/** Statistics enum */
	typedef enum {
			/** Global T1 Decoder US total */
		ST_CODEBLOCK_DECODER_US			= 0,	
			/** Global T1 Decoder samples total */
		ST_CODEBLOCK_DECODER_SAMPLES	= 1,	
			/** Global Codeblock read US total */
		ST_CODEBLOCK_READ_US			= 2,	
			/** Global Codeblock read samples total */
		ST_CODEBLOCK_READ_SAMPLES		= 3,
			/** DC Shift US total */
		ST_DCSHIFT_US					= 10,
			/** MCT US total */
		ST_MCT_US						= 20,
			/** DWT ROW US total */
		ST_DWT_ROW_US					= 30,
			/** DWT COL US total */
		ST_DWT_COL_US					= 31,
			/** DWT Samples total */
		ST_DWT_SAMPLES					= 32,
			/** SetView US total */
		ST_VIEW_SETVIEW_US				= 40,
			/** ReadLine US total */
		ST_VIEW_READLINE_US				= 41,
			/** Resample US total */
		ST_VIEW_RESAMPLE_US				= 42,
			/** Raw IO Read US total */
		ST_IO_READ_US					= 50,
			/** Raw IO Write US total */
		ST_IO_WRITE_US					= 51,
			/** Raw IO Seek US total */
		ST_IO_SEEK_US					= 52,
			/** Global Buffer Alloc US total */
		ST_BUF_ALLOC_US					= 60,	
			/** Global Buffer Free US total */
		ST_BUF_FREE_US					= 61,	
			/** Global Buffer bytes allocated total */
		ST_BUF_ALLOC_BYTES				= 62,	
			/** Global Buffer Copy US total */
		ST_BUF_COPY_US					= 63,	
			/** Global Buffer bytes copied total */
		ST_BUF_COPY_BYTES				= 64,	
			/** Global Buffer Clear US total */
		ST_BUF_CLEAR_US					= 65,	
			/** Global Buffer bytes cleared total */
		ST_BUF_CLEAR_BYTES				= 66,	
			/** File Compression Total MS */
		ST_COMPRESS_TOTAL_MS			= 80,	
			/** File Compression output size bytes total */
		ST_COMPRESS_OUTPUT_SIZE			= 81	
	} Statistic;

		/** JP2FileView parameters 
		* NOTE: THIS IS DEPRECATED.  USE THE STRING NAME VERSION INSTEAD
		*/
	typedef enum {
			/** Default, Baseline Profile 0 */
		JP2_COMPRESS_PROFILE_BASELINE_0		= 0, 
			/** Baseline Profile 1 */
		JP2_COMPRESS_PROFILE_BASELINE_1		= 1, 
			/** Profile 2 (unrestricted) */
		JP2_COMPRESS_PROFILE_BASELINE_2		= 2, 
			/** NITF NSIF BIIF NPJE Profile */
		JP2_COMPRESS_PROFILE_NITF_BIIF_NPJE = 3, 
			/** NITF NSIF BIIF EPJE Profile */
		JP2_COMPRESS_PROFILE_NITF_BIIF_EPJE = 4, 

			/** Resolution Levels.  UINT32 - Calculated so r=0 <= 64x64 */
		JP2_COMPRESS_LEVELS					= 20, 
			/** Quality Layers.  UINT32 - Default 1 */
		JP2_COMPRESS_LAYERS					= 21, 
			/** Precinct Width.  UINT32 - Default 64 or larger depending on file size */
		JP2_COMPRESS_PRECINCT_WIDTH			= 22, 
			/** Precinct Height.  UINT32 - Default 64 or larger depending on file size */
		JP2_COMPRESS_PRECINCT_HEIGHT		= 23, 
			/** Tile Width.  UINT32 - Default to image width given in SetFileInfo() */
		JP2_COMPRESS_TILE_WIDTH				= 24, 
			/** Tile Height.  UINT32 - Default to image height given in SetFileInfo() */
		JP2_COMPRESS_TILE_HEIGHT			= 25, 
			/** Output SOP.  bool - Default false */
		JP2_COMPRESS_INCLUDE_SOP			= 26, 
			/** Output EPH.  bool - Default true */
		JP2_COMPRESS_INCLUDE_EPH			= 27, 
			/** bool - Only write JPC codestream, no JP2 file wrapper */
		JP2_COMPRESS_CODESTREAM_ONLY		= 28, 
			/** bool - Enable/disable compression threaded read - Default true */
		JP2_COMPRESS_MT_READ				= 29, 

			/** LRCP progression */
		JP2_COMPRESS_PROGRESSION_LRCP		= 30, 
			/** RLCP progression */
		JP2_COMPRESS_PROGRESSION_RLCP		= 31, 
			/** v3.1 SDK Default: RPCL progression */
		JP2_COMPRESS_PROGRESSION_RPCL		= 32, 

			/** Control the precedence of georeferencing metadata from world files and embedded GML XML boxes and PCS UUID boxes */
		JP2_GEODATA_USAGE					= 40, 
			/** Control the precision employed by registration info (rounding errors can cause ridiculous values) */
		JP2_GEODATA_PRECISION_EPSILON       = 41,
			/** UINT32 - Defaults to all */
		JP2_DECOMPRESS_LAYERS				= 100, 
			/** IEEE4 - defaults to 0.0, range 0.0 <= r < 1.0 */
		JPC_DECOMPRESS_RECONSTRUCTION_PARAMETER = 101,
			/** Auto scale data up to read cell size, eg scale 1bit to 8bit when reading NCSCT_UINT8 */
		JPC_DECOMPRESS_AUTOSCALE_UP			= 102,

			/** Options for both formats */
			
		NCS_REFRESH_TIME_MS					= 200
	} Parameter;

#ifndef SWIG
	class NCSECW_IMPEX CReaderBase {//: public CThreadPool::CJob {
	public:
		CReaderBase(CView *pView);
		virtual ~CReaderBase();
		virtual CError SetView(UINT32 nBands, UINT32 *pBandList, 
								   UINT32 nDatasetTLX, UINT32 nDatasetTLY,
								   UINT32 nDatasetBRX, UINT32 nDatasetBRY,
								   UINT32 nWidth, UINT32 nHeight,
								   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
								   IEEE8 dWorldBRX, IEEE8 dWorldBRY) = 0; 
		virtual NCSEcwReadStatus Read(SDK::CBuffer2DVector &Buffers) = 0;
		virtual NCSEcwReadStatus ReadLine(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps) = 0;
		virtual void UpdateViewStats() = 0;

		virtual NCSFileType GetFileType() = 0;
				/** Copy the parameters from the m_Parameters vector to the File instance */
		virtual void SetFileParameters() {};

		virtual SDK::CFileBase *GetFile() { return(m_pView ? m_pView->GetFile() : NULL); };
		virtual CView *GetView() { return(m_pView); };

		virtual bool ReceivedBlock(NCSBlockId nBlock);

		NCS_INLINE UINT32 &NextLine() { return(m_nNextLine); };
		NCS_INLINE void NextLine(UINT32 n) { m_nNextLine = n; };

		virtual void Work();

		
		union UScaleValue {
			UINT8  u8;
			INT8   i8;
			UINT16 u16;
			INT16  i16;
			UINT32 u32;
			INT32  i32;
			UINT64 u64;
			INT64  i64;
			IEEE4  ie4;
			IEEE8  ie8;
		};
		UScaleValue m_uMinScale;
		UScaleValue m_uMaxScale;
		bool m_bScaleDataEnabled;
	protected:
		CView *m_pView;
			/** Scanline input buffer */
		SDK::CBuffer2D m_InputBuf;

		/** Next line in view to read */
		UINT32		m_nNextLine;

		CBlockIdQueue *m_pReceivedBlocks;

		NCSFileViewSetInfo m_LastRefreshedView;
		//void Work(void *pData);
	};
#endif //SWIG

#ifndef SWIG
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	class NCSECW_IMPEX CWriterBase: public CThread {
	public:
		CWriterBase(CView *pView) {
				m_pView = pView;
				m_bLowMemCompress = false;
				m_bCompressMTRead = true;//NCSGetNumPhysicalCPUs() > 1;
				m_nNextLine = 0;
			};
		virtual ~CWriterBase() {
				m_pView = NULL;
				FreeCompressBuffers();
			};
		virtual CError Open(const CString &URLPath, CIOStream *pStream) = 0;
		virtual NCSFileViewFileInfoEx *GetFileInfo() = 0;
		virtual CError WriteLineBIL(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL) = 0;
		virtual NCSFileType GetFileType() = 0;
				/** Copy the parameters from the m_Parameters vector to the File instance */
		virtual void SetFileParameters() {};

		virtual SDK::CFileBase *GetFile() { return(m_pView ? m_pView->GetFile() : NULL); };
		virtual CView *GetView() { return(m_pView); };

		NCS_INLINE UINT32 &NextLine() { return(m_nNextLine); };
		NCS_INLINE void NextLine(UINT32 n) { m_nNextLine = n; };
	protected:
		friend class CView;
		CView *m_pView;
			/** Compression buffer queues */
		SDK::CBuffer2DQueue m_CompressReadQueue;
		SDK::CBuffer2DQueue m_CompressFreeQueue;

			/** Multi-thread compression */
		bool m_bCompressMTRead;
			/** Performing low-memory compression */
		bool m_bLowMemCompress;

			/** Next line to write */
		UINT32		m_nNextLine;
	
		virtual void AllocCompressBuffers() {
				if(m_bCompressMTRead) {
					m_CompressFreeQueue.Alloc(m_bLowMemCompress ? 2 : MAX_THREAD_LINES, m_pView->m_nNumberOfBands);
				} else {
					m_CompressFreeQueue.Alloc(1, m_pView->m_nNumberOfBands);
				}
			};
		virtual void FreeCompressBuffers()	{
				Stop(true);
				m_CompressFreeQueue.Free();
				m_CompressReadQueue.Free();
			};
	};
#endif //ECW_COMPRESS_RW_SDK_VERSION
#endif //SWIG

		/** The number of cells across the dataset */
	INT32 m_nWidth;				
		/** The number of lines down the dataset */
	INT32 m_nHeight;			
		/** The number of bands in the dataset */
	INT32 m_nNumberOfBands;	
		/** The actual compression ratio */	
	IEEE8 m_dCompressionRate;	
		/** The target compression ratio */
	IEEE8 m_dTargetCompressionRate;
		/** Cell bits for MULTIBAND, RGB, YUV and GREYSCALE, currently support 8bit (default) and 16bit (progressing) */
	UINT8 m_nCellBitDepth;
		/** The X cell size in m_CellSizeUnits */
	IEEE8 m_dCellIncrementX;	
		/** The Y cell size in m_CellSizeUnits */
	IEEE8 m_dCellIncrementY;	
		/** The top left X origin of the dataset (world) */
	IEEE8 m_dOriginX;			
		/** The top left Y origin of the dataset (world) */
	IEEE8 m_dOriginY;			

		/** The GDT datum of the dataset */
	char *m_pDatum;				
		/** The GDT projection of the dataset */
	char *m_pProjection;		
		/** A pointer to the currently open filename in UNICODE*/
	wchar_t *m_pFilename;			
		/* Currently open URL */
	CString m_URL;
		/** Cell units, meters, degrees or feet */
	CellSizeUnits m_CellSizeUnits;
		/** Cell type image should be read as */
	NCSEcwCellType m_eCellType;		
		/** ColorSpace of image */
	NCSFileColorSpace m_eColorSpace;		
		/** BandInfo for each band */
	std::vector<NCSFileBandInfo> m_Bands;
		/** The clockwise rotation of the image in degrees [02] */
	IEEE8 m_dCWRotationDegrees; 
		/** Index of AllOpacity band */
	INT32 m_iAllOpacityBand;
		/** The number of bands in the current view */
	INT32 m_nSetViewNrBands;		
		/** A pointer to the band list for the current view */
	INT32 *m_pnSetViewBandList;		
		/** SetView() contains allopacity band */
	INT32 m_bSetViewHasAllOpacity;
		/** SetView() allopacity band number */
	INT32 m_nAllOpacityBandNumber;
		/** The current view width */
	INT32 m_nSetViewWidth;			
		/** The current view height */
	INT32 m_nSetViewHeight;			
		/** The current view world top left X */
	IEEE8 m_dSetViewWorldTLX;		
		/** The current view world top left Y */
	IEEE8 m_dSetViewWorldTLY;		
		/** The current view world bottom right X */
	IEEE8 m_dSetViewWorldBRX;		
		/** The current view world bottom right Y */
	IEEE8 m_dSetViewWorldBRY;		
		/** The current view dataset top left X */
	INT32 m_nSetViewDatasetTLX;		
		/** The current view dataset top left Y */
	INT32 m_nSetViewDatasetTLY;		
		/** The current view dataset bottom right X */
	INT32 m_nSetViewDatasetBRX;		
		/** The current view dataset bottom right Y */
	INT32 m_nSetViewDatasetBRY;		

	INT32 m_bUpdateOpacity;
		/** Default constructor, initialises members */
	CView();
	CView(const CView &s);
		/** Virtual Destructor */
	virtual ~CView();

	virtual CView &operator=(const CView &s);
#ifdef NCS_BUILD_UNICODE
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(wchar_t *pURLPath, bool bProgressiveDisplay = false, bool bWrite = false);
#endif
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(char *pURLPath, bool bProgressiveDisplay = false, bool bWrite = false);
			/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		URLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(const CString &URLPath, bool bProgressiveDisplay = false, bool bWrite = false);
		/** 
		 * Open the JP2 file for Parsing/UnParsing on the specified stream.  The stream object
		 * may be a custom subclass of CIOStream with overridden IO methods.  You should
		 * not usually maintain a reference or pointer to the stream object after calling this 
		 * method due to the way the SDK handles the destruction of NCSFile objects, which
		 * may lead to uncertainty as to the validity of the stream object over time.
		 * @param		pStream		Input stream on which to open the file
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */

	virtual CError Open(CIOStream *pStream, bool bProgressiveDisplay = false, NCSFileType eType = NCS_FILE_UNKNOWN);
		/** 
		 * Close the JP2 file
		 * @param		bFreeCache	Free the cached JP2 file data.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Close ( bool bFreeCache = false );

		/** 
		 * Set a view into the JP2 file for reading.
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
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(UINT8 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(UINT16 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(UINT32 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(UINT64 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(INT8 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(INT16 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(INT32 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(INT64 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(IEEE4 **ppOutputLine);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(IEEE8 **ppOutputLine);
		/** 
		 * Read the next line in RGB UINT8 triplet format from the current view into the file.
		 * @param		pRGBTripler			Pointer to UINT8 buffer to receive RGB data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineRGB(UINT8 *pRGBTriplet);
		/** 
		 * Read the next line in BGR UINT8 triplet format from the current view into the file.
		 * @param		pBGRTripler			Pointer to UINT8 buffer to receive BGR data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBGR(UINT8 *pBGRTriplet);
		/** 
		 * Read the next line in RGBA UINT32 format from the current view into the file.
		 * @param		pRGBA				Pointer to UINT32 buffer to receive RGBA data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineRGBA(UINT32 *pRGBA);
		/** 
		 * Read the next line in BGRA UINT32 format from the current view into the file.
		 * @param		pBGRA				Pointer to UINT32 buffer to receive BGRA data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBGRA(UINT32 *pBGRA);
		/** 
		 * Read the next line in ARGB UINT32 format from the current view into the file.
		 * @param		pARGB				Pointer to UINT32 buffer to receive ARGB data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineARGB(UINT32 *pARGB);
		/** 
		 * Read the next line in ABGR UINT32 format from the current view into the file.
		 * @param		pABGR				Pointer to UINT32 buffer to receive ABGR data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineABGR(UINT32 *pABGR);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBIL(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
		/** 
		 * Read the given buffers from the current view.
		 * @param		Buffers				Output buffers
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus Read(SDK::CBuffer2DVector &Buffers);
#ifdef ECW_COMPRESS_RW_SDK_VERSION
		/** 
		 * Write the next line in BIL format into the JP2 file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      CError			Write status code
		 */
	virtual CError WriteLineBIL(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
#endif
	/**
	 *	More data has become available and a refresh update should be done.  Deprecated, override RefreshUpdateEx() instead.
	 *
	 *	@param[in]	pViewSetInfo	Pointer to a SetViewInfo containing details on view the update is from.
	 */
	virtual void RefreshUpdate(NCSFileViewSetInfo *pViewSetInfo);
		/** 
		 * More data is available and a refresh update should be done.
		 * @param		pSetViewInfo		Pointer to SetViewInfo containing details on view the update is for
		 * @return      NCSEcwReadStatus	Return the Read status code from the ReadLine*() call.
		 */
	virtual NCSEcwReadStatus RefreshUpdateEx(NCSFileViewSetInfo *pViewSetInfo);

#ifdef ECW_COMPRESS_RW_SDK_VERSION
		/** 
		 * Start the compression.
		 * In progressive (pull) mode scanlines will be sequentially
		 * read by the overloaded WriteReadLine() method
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      CError			Write status code
		 */
	virtual CError Write();	
		/** 
		 * Read input line for compression.
		 * In progressive (pull) mode scanlines will be sequentially
		 * read by the overloaded WriteReadLine() method
		 * @param		nNextLine			Next input line to read
		 * @param		ppInputArray		Array of buffer pointers, one buffer for each band
		 * @return      CError			Write status code
		 */
	virtual CError WriteReadLine(UINT32 nNextLine, void **ppInputArray);	
		/** 
		 * Compression status callback.
		 * you overload this if you want a status callback to show progress during compression
		 * @param		nCurrentLine		Current input line processed
		 */
	virtual void WriteStatus(UINT32 nCurrentLine);
		/** 
		 * Compression cancel callback.
		 * you overload this if you want to be able to cancel during compression
		 * @return      bool				Return true to cancel, else false to continue
		 */
	virtual bool	WriteCancel(void);

		/** 
		 * Get current ViewSetinfo structure.
		 * @return      NCSFileViewSetInfo*	Return pointer to the current SetViewInfo.
		 */
#endif
	virtual /*const*/ NCSFileViewSetInfo *GetFileViewSetInfo();
		/** 
		 * Get FileInfo structure.
		 * @return      NCSFileViewFileInfo*	Return pointer to the FileInfo.
		 */
	virtual /*const*/ NCSFileViewFileInfoEx *GetFileInfo();
		/** 
		 * Set FileInfo structure.
		 * @param		Info		New fileinfo - used to specify file info for compression
		 * @return      CError	Return pointer to the FileInfo.
		 */
	virtual CError SetFileInfo(NCSFileViewFileInfoEx &Info);

		/**
		  * Get the next UUID box with the specified UUID from the file.
		  * @param		uuid			UUID of UUID box to find.
		  * @param		pLast			Last UUID box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual JP2::CBox *GetUUIDBox(UUID uuid, JP2::CBox *pLast = NULL);
		/**
		  * Get the next XML box from the file.
		  * @param		pLast			Last XML box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual JP2::CBox *GetXMLBox(JP2::CBox *pLast = NULL);
		/**
		  * Get the next box of the specified type from the file.
		  * @param		nTBox			Box type to find
		  * @param		pLast			Last box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual JP2::CBox *GetBox(UINT32 nTBox, JP2::CBox *pLast = NULL);
		/**
		  * Add a box to be written to the file on compression.
		  * The specified box's UnParse() method will be called to write the box into the file
		  * @param		pBox			Box to be written to file
		  * @return		CError		returned error or NCS_SUCCESS;
		  */
	virtual CError AddBox(JP2::CBox *pBox);

		/**
		  * Get a GDT (ER Mapper) Projection/Datum pair for the given EPSG code, if available
		  * @param		nEPSGCode		EPSG Code to find projection/datum for
		  * @param		ppProjection	Returned GDT Projection name - free with NCSFree()
		  * @param		ppDatum			Returned GDT Datum name - free with NCSFree()
		  * @return		CError		Error code;
		  */
	static CError GetProjectionAndDatum(const INT32 nEPSGCode, char **ppProjection, char **ppDatum, bool bUseEpsgCodeIfFails=false);
		/**
		  * Get an EPSG code from the given GDT (ER Mapper) Projection/Datum pair if available
		  * @param		pProjection		GDT Projection name
		  * @param		pDatum			GDT Datum name
		  * @param		nEPSGCode		Returned EPSG Code or 0 on error
		  * @return		CError		Error code;
		  */
	static CError GetEPSGCode(char *pProjection, char *pDatum, INT32 *nEPSGCode);
		/**
		  * Get the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
		  * @return the custom path being used, or NULL
		  */
	static char *GetGDTPath();
		/**
		  * Set the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
		  * @param szPath the new GDT data path
		  */
	static void SetGDTPath(const char *szPath);
		/**
		  * Refresh the custom GDT data path, doing a search and validity check on 
		  * the current path value and several common GDT data locations
		  */
	static void DetectGDTPath();

	static bool AddCustomEPSGCodes(char *pMapping);

		/** 
		  * Set refresh callback funnction 
		  * @param		pCallback		Refresh callback function to use
		  * @return		CError		Error code;
		  */
	CError SetRefreshCallback(NCSEcwReadStatus (*pCallback)(NCSFileView*));

		/** 
		 * Get a pointer to the underlying CIOStream.
		 * This can be used to read specific JP2 UUID/XML boxes etc.
		 * This may also be useful if the underlying stream object belongs to a 
		 * custom subclass of CIOStream that you have passed to the overloaded
		 * method CFile::Open(CIOStream *pStream, bool bProgressiveDisplay).
		 * @return      CIOStream*	Return pointer to the CIOStream, else NULL.
		 */
	CIOStream *GetStream();

		/**
		  * Get a statistic of NCSTimeStampMs type
		  * @param		eType				Statistic to find
		  * @param		Val					Statistic value
		  */		 
	virtual void GetStatistic(Statistic eType, NCSTimeStampMs &Val);
		/**
		  * Get a statistic of UINT64 type
		  * @param		eType				Statistic to find
		  * @param		Val					Statistic value
		  */		 
	virtual void GetStatistic(Statistic eType, UINT64 &Val);
		/**
		  * Reset the specified statistic to the default value
		  * @param		eType				Statistic to reset
		  */		 
	virtual void ResetStatistic(Statistic eType);
		/**
		  * Reset all statistics to their default values
		  */		 
	virtual void ResetStatistics();
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		eType			Parameter type to set.
		  * @param		...				value to set
		  */
	virtual void SetParameter(Parameter eType, ...);
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		...				value to set
		  */
	virtual void SetParameter(char *sName, ...);
		/**
		  * Get a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		eType			Parameter type to set.
		  * @param		...				value to set
		  */
	virtual void GetParameter(Parameter eType, ...);
		/**
		  * Get a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		...				value to set
		  */
	virtual void GetParameter(char *sName, ...);
		/**
		  * Set the usage of geographical metadata when reading and writing JPEG 2000 files.
		  * @param		nGeodataUsage	GeodataUsage enum value specifying the desired metadata types
		  */
	static void SetGeodataUsage(GeodataUsage nGeodataUsage);

		/**
		  * Get the percent complete (from time of setview == 0%)
		  * @return		INT32			Returned Percent complete
		  */		 
	INT32 GetPercentComplete();

		/**
		  * Get the percent complete as a percentage of total blocks in the current view.
		  * @return		INT32			Returned Percent complete
		  */		 
	INT32 GetPercentCompleteTotalBlocksInView();

		/**
		  * Get the EPSG code associated with this view's projection and datum, if any.
		  * @return		INT32			EPSG code, or 0 if none applies.
		  */
	INT32 GetEPSGCode();

		/**
		  * Call this function to enable compression.
		  * NOTE: Verify you are in compliance with the appropriate license agreements.
		  * Calling this function signifies you accept the terms of the appropriate license.
		  */		 
	static void SetOEMKey(char *szCompanyName, char *szKey);

		/**
		  * Get the type of the currently open file
		  * @return		NCSFileType			The file type.
		  */
	NCSFileType GetFileType();

		/**
		  * Get the Mime type of the currently open file
		  * @return		The Mime Type string or NULL if error.
		  */
	char *GetFileMimeType();
	
		/**
		  * Return the Mime type of a file given the file type.
		  * @param		NCSFileType			The file type.
		  * @return		The Mime Type string or NULL if error.
		  */
	static char *GetFileMimeType(NCSFileType fileType);

	void _Lock() { m_Mutex.Lock(); };
	bool _TryLock() { return(m_Mutex.TryLock()); };
	void _UnLock() { m_Mutex.UnLock(); };

	/**
		  * Set Compress Client - Internal func for "C" API support only
		  * @param		pCompressClient	ECW Compress Client struct
		  * @return		CError		Error code;
		  */		 
	CError SetCompressClient(struct NCSEcwCompressClient *pCompressClient);

//FIXME TEMP public till ECW in C++
				/** ECW SetViewInfo Structure */
	NCSFileViewSetInfo m_CurrentView;
				/** Display update is progressive for this view */
	bool m_bIsProgressive;
			/** Timestamp representing lat time a blocking read was called */
	NCSTimeStampMs	m_tsLastBlockTime;

	virtual void Work();

protected:
#ifndef SWIG
	friend ECW::CReader;
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	friend ECW::CWriter;
	friend JP2::CWriter;
#endif
	friend JP2::CReader;
	friend SDK::CFileBase;
	friend JPC::CResampler;

		/** Set view mode is world? */
	bool m_bSetViewModeIsWorld;

		/** 
		 * Internal method to set a view into the JP2 file for reading.
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
		 * @param		bFormWorker		Is this call from the worker thread or external?
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetViewInternal(UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0, bool bFromWorker = false);
		/** 
		 * Set a pending view on the JP2 file for reading.
		 * Actual SetView() will occur from within worker thread when it processes the pending view.
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
	virtual CError SetPendingView(UINT32 nBands, UINT32 *pBandList, 
									 UINT32 nDatasetTLX, UINT32 nDatasetTLY,
									 UINT32 nDatasetBRX, UINT32 nDatasetBRY,
									 UINT32 nWidth, UINT32 nHeight,
									 IEEE8 dWorldTLX, IEEE8 dWorldTLY,
									 IEEE8 dWorldBRX, IEEE8 dWorldBRY);
		/** 
		 * Validates the view parameters for correctness.
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
	virtual CError ValidateViewParams(UINT32 nBands, UINT32 *pBandList, 
										 UINT32 nDatasetTLX, UINT32 nDatasetTLY,
										 UINT32 nDatasetBRX, UINT32 nDatasetBRY,
										 UINT32 nWidth, UINT32 nHeight,
										 IEEE8 dWorldTLX, IEEE8 dWorldTLY,
										 IEEE8 dWorldBRX, IEEE8 dWorldBRY);
			/** 
		 * Read the next line in RGB UINT8 triplet format from the current view into the file.
		 * @param		pRGBTripler			Pointer to UINT8 buffer to receive RGB data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineRGBInternal(UINT8 *pRGBTriplet);
		/** 
		 * Read the next line in BGR UINT8 triplet format from the current view into the file.
		 * @param		pBGRTripler			Pointer to UINT8 buffer to receive BGR data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBGRInternal(UINT8 *pBGRTriplet);
		/** 
		 * Read the next line in RGBA UINT32 format from the current view into the file.
		 * @param		pRGBA				Pointer to UINT32 buffer to receive RGBA data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineRGBAInternal(UINT32 *pRGBA);
		/** 
		 * Read the next line in BGRA UINT32 format from the current view into the file.
		 * @param		pBGRA				Pointer to UINT32 buffer to receive BGRA data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBGRAInternal(UINT32 *pBGRA);
		/** 
		 * Read the next line in ARGB UINT32 format from the current view into the file.
		 * @param		pARGB				Pointer to UINT32 buffer to receive ARGB data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineARGBInternal(UINT32 *pARGB);
		/** 
		 * Read the next line in ABGR UINT32 format from the current view into the file.
		 * @param		pABGR				Pointer to UINT32 buffer to receive ABGR data
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineABGRInternal(UINT32 *pABGR);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      NCSEcwReadStatus	Read status code
		 */
		/** 
		 * Internal non-locking Read the next line in BIL format from the current view into the file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      NCSEcwReadStatus	Read status code
		 */
	virtual NCSEcwReadStatus ReadLineBILInternal(NCSEcwCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);

		/**
		  * Fire a refresh update,
		  * @return		NCSEcwReadStatus	RefreshUpdate Read status
		  */
	NCSEcwReadStatus FireRefreshUpdate();
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		pStream		IO Stream to open on
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError OpenForRead(const CString &URLPath, CIOStream *pStream, bool bProgressiveDisplay);
#ifdef ECW_COMPRESS_RW_SDK_VERSION
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		pStream		IO Stream to open on
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError OpenForWrite(const CString &URLPath, CIOStream *pStream, bool bProgressiveDisplay, NCSFileType eType);	
#endif

	CError NCS::CView::OpenOpacityFileView(SDK::CFileBase *pFile, bool bProgressiveDisplay, NCSFileType eType);

		/** 
		 * Set FileInfo structure.
		 * @param		Info		New fileinfo - used to specify file info for compression
		 * @return      CError	Return pointer to the FileInfo.
		 */
	virtual CError SetFileInfoInternal(NCSFileViewFileInfoEx &Info);

		/** Add a dynamically allocated node to the dynamic node list so it is released */
	void AddDynamicNode(SDK::CNode2D *pNode) { m_DynamicNodes.push_back(pNode); };
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		va				varargs list
		  */
	virtual void SetParameterInternal(char *sName, va_list va);

		/**
		  * Get a parameter on the view.  Get the default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		va				varargs list
		  */
	virtual void GetParameterInternal(char *sName, va_list va);
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	NCS_INLINE UINT32 &NextLine() { return(m_pReader ? m_pReader->NextLine() : m_pWriter->NextLine()); };
	NCS_INLINE void NextLine(UINT32 n) { m_pReader ? m_pReader->NextLine(n) : m_pWriter->NextLine(n); };
#else
	NCS_INLINE UINT32 &NextLine() { return(m_pReader->NextLine()); };
	NCS_INLINE void NextLine(UINT32 n) {m_pReader->NextLine(n); };
#endif
	SDK::CNode2D::ContextID	m_CtxID;
	
		/** Connection has been lost to server (ECWP) */
	bool m_bConnectionLost;
		/** SetView is valid */
	bool m_bHaveValidSetView;
		/** File is open */
	bool m_bIsOpen;
		/** File is open */
	bool m_bIsWrite;
		/** Autoscale data up on read */
	bool m_bAutoScaleUp;
#ifdef AUTO_SCALING
	bool m_bAutoScaleDown;
#endif
		/** Do we have a pending setview */
	CMutex m_PendingViewMutex;
	INT32	m_nPendingViews;
	INT32	m_nCancelledViews;
		/** Progressive pending View */
	NCSFileViewSetInfo m_PendingView;

	/**
	 * ParameterValuePair class - holds compression parameters prior to CFile instance being created on the Open()
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #6 $ $Author: scope $ $Date: 2007/12/14 $ 
	 */
#ifndef SWIG
	class NCSECW_IMPEX ParameterValuePairBase {
	public:
		ParameterValuePairBase(char *pName) { m_pName = NCSStrDup(pName); };
		virtual ~ParameterValuePairBase() { NCSFree(m_pName); };
		virtual bool Is(char *pName) { return(!_stricmp(pName, m_pName)); };
		virtual void GetValue(void *pValue) = 0;
	protected:
			/** Parameter name */
		char	*m_pName;
	};
	template<class T> class NCSECW_IMPEX ParameterValuePair: public ParameterValuePairBase {
	public:
			/** No-parameter constructor */
		ParameterValuePair(char *pName):ParameterValuePairBase(pName) {};
			/** bool-parameter constructor */
		ParameterValuePair(char *pName, T Value):ParameterValuePairBase(pName) { m_Value = Value; };
			/** Virtual destructor */
		virtual ~ParameterValuePair() {};
		
		virtual void GetValue(void *pValue) { *((T*)pValue) = m_Value; };
	protected:
			/** Parameter value */
		T		m_Value;
	};
#endif

		/** Number of blocks in cache at the last SetView() */
	UINT32	m_nBlocksAtLastRefreshUpdate;
		/** Current in RefreshUpdate() callback */
	bool	m_bInRefreshUpdate;
		/** Cancel the read */
	bool	m_bCancelRead;

		/** Timestamp of last refreshupdate for this view */
	NCSTimeStampMs m_tsLastRefreshUpdate;
		/** MS blocking delay, defaults to global NCSCFG_REFRESH_TIME_MS */
	NCSTimeStampMs	m_nBlockingTimeMS;

		/** parameters held until open() */
	std::vector<ParameterValuePairBase*> m_Parameters;
		/** Copy the parameters from the m_Parameters vector to the CFile instance */
	void SetFileParameters();
		/** List of other JP2 box(es) to be included in the output file */
	JP2::CBox::CBoxList m_OtherBoxes;	

		/** Used when SetView is >4000 in either dimension - too large to cache */
	bool	m_bTiledView;
		/** This member stores a view "tile" (horizontal strip) when view > 4K x 4K */
	CView *m_pTiledView;
		/** Set the tiled views */
	CError SetTiledView(NCSFileViewSetInfo &View, UINT32 nReadLine);

		/** Used when reading extra opacity file */
	bool	m_bIsAppendedOpacityView;
		/** This member stores an opacity view */
	CView *m_pOpacityView;
		/** Set the opacity view */
	CError SetOpacityView(NCSFileViewSetInfo &View);

		/** Dynamic Nodes to free */
	SDK::CNode2DVector m_DynamicNodes;

		// The following private members are here to support the "C" API only.
		/** Progressive read callback for ECW decompression */
	NCSEcwReadStatus (*m_pRefreshCallback)(NCSFileView *pNCSFileView);
		/** ECW Compress Client for "C" compression API */
	struct NCSEcwCompressClient *m_pCompressClient;

		/** Compression Start timestamp */
	NCSTimeStampMs	m_tsCompressStart;
		/** Compression End timestamp */
	NCSTimeStampMs	m_tsCompressEnd;
		/** Compression Error status */
	CError		m_CompressError;

	CReaderBase *m_pReader;
#ifdef ECW_COMPRESS_RW_SDK_VERSION
	CWriterBase *m_pWriter;
#endif
		/** Do Blocking mode wait */
	virtual void DoBlockingWait();
		/** Update the public file members */
	virtual bool UpdateFileInfo(void);
		/** Update the public view members */
	virtual bool UpdateFileViewInfo(void);

		/** Delete all dynamic nodes */
	virtual void DeleteDynamicNodes(void);

		/** Update the view statistics */
	virtual void UpdateViewStats();

		/** Http Headers that are passed to cnet */
	NCS::CString m_sECWPHttpHeaders;

#endif //SWIG

public:
		/** Check if the ECWP connection is using streaming or polling mode */
	virtual bool IsConnected();
	virtual bool Reconnect();
	virtual bool IsConnectionPolling();

	/**	
	 *	Set the view on the open file.  This version takes world coordinates as input.
	 *
	 *	@param[in]	nBands				The number of bands to include in the view being set.
	 *	@param[in]	pBandList			An array of band indices specifying which bands to include and in which order.
	 *	@param[in]	nWidth				The width of the view to construct in dataset cells.
	 *	@param[in]	nHeight				The height of the view to construct in dataset cells.
	 *	@param[in]	dWorldTLX			The left of the view to construct in world coordinates.
	 *	@param[in]	dWorldTLY			The top of the view to construct in world coordinates.
	 *	@param[in]	dWorldBRX			The right of the view to construct in world coordinates.
	 *	@param[in]	dWorldBRY			The bottom of the view to construct in world coordinates.
	 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
	 */
	virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
					   IEEE8 dWorldBRX, IEEE8 dWorldBRY );
	/**	
	 *	Set the view on the open file.  This version takes dataset coordinates as input.
	 *
	 *	@param[in]	nBands				The number of bands to include in the view being set.
	 *	@param[in]	pBandList			An array of band indices specifying which bands to include and in which order.
	 *	@param[in]	nWidth				The width of the view to construct in dataset cells.
	 *	@param[in]	nHeight				The height of the view to construct in dataset cells.
	 *	@param[in]	nDatasetTLX			The left of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetTLY			The top of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetBRX			The right of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetBRY			The bottom of the view to construct in dataset coordinates.
	 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
	 */
	virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   INT32 nDatasetTLX, INT32 nDatasetTLY,
					   INT32 nDatasetBRX, INT32 nDatasetBRY );

	/**
	 *	Rectilinear conversion from world coordinates to dataset coordinates.
	 *
	 *	@param[in]	dWorldX				The world X coordinate.
	 *	@param[in]	dWorldY				The world Y coordinate.
	 *	@param[out]	pnDatasetX			A buffer for the output dataset X coordinate.
	 *	@param[out]	pnDatasetY			A buffer for the output dataset Y coordinate.
	 */
	NCSError ConvertWorldToDataset(IEEE8 dWorldX, IEEE8 dWorldY, INT32 *pnDatasetX, INT32 *pnDatasetY);
	/**
	 *	Rectilinear conversion from dataset coordinates to world coordinates.
	 *
	 *	@param[in]	nDatasetX			The dataset X coordinate.
	 *	@param[in]	nDatasetY			The dataset Y coordinate.
	 *	@param[out]	pdWorldX			A buffer for the output world X coordinate.
	 *	@param[out]	pdWorldY			A buffer for the output world Y coordinate.
	 */
	NCSError ConvertDatasetToWorld(INT32 nDatasetX, INT32 nDatasetY, IEEE8 *pdWorldX, IEEE8 *pdWorldY);
	/**
	 *	Set a (void *) to a data structure containing any client data that must be accessed in the read callback.
	 *
	 *	@param[in]	pClientData			(void *) pointer to client data.
	 */
	void SetClientData(void *pClientData);
	/**	
	 *	Obtain any client data that has been established.
	 *
	 *	@return							(void *) pointer to client data.
	 */
	void *GetClientData();

	/** 
	 *	Utility function.  Breaks down a URL string into protocol, hostname, and filename components.
	 *
	 *	@param[in]	pURLPath		The URL to be broken down and analyzed.
	 *	@param[out]	ppProtocol		A pointer to the protocol string resulting from the URL decomposition.
	 *	@param[out]	ppHost			A pointer to the hostname resulting from the URL decomposition.
	 *	@param[out]	ppFilename		A pointer to the filename resulting from the URL decomposition.
	 *	@return						BOOLEAN value, whether the input URL is a remote file.
	 */
	static BOOLEAN BreakdownURL(  char *pURLPath, 
							char **ppProtocol,
							char **ppHost, 
							char **ppFilename);
	/**	
	 *	Obtain meaningful error text from a returned error code.
	 *
	 *	@param[in]	nErrorNum		Error code
	 *	@return						(char *) value, an explanatory ASCII string for the error code
	 */
	static const char *FormatErrorText ( NCSError nErrorNum );

	virtual bool ReceivedBlock(NCSBlockId nBlock);

#ifndef SWIG
protected:
	CMutex m_Mutex;
private:
	void *m_pClientData;
#endif
};
}

#pragma warning(pop)

#endif // !NCSVIEW_H
