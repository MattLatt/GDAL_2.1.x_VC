/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/


#ifndef NCS_CHEADER_EDITOR
#define NCS_CHEADER_EDITOR

#include "NCSECWClient.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	UINT8				nVersion;			// ECW file version == ERSWAVE_VERSION
	CellSizeUnits		eCellSizeUnits;		// Units used for pixel size
	IEEE8				fCellIncrementX;	// Increment in CellSizeUnits in X direction. May be negative. Will never be zero
	IEEE8				fCellIncrementY;	// Increment in CellSizeUnits in Y direction. May be negative. Will never be zero
	IEEE8				fOriginX;			// World X origin for top-left corner of top-left cell, in CellSizeUnits
	IEEE8				fOriginY;			// World Y origin for top-left corner of top-left cell, in CellSizeUnits
	char				*szDatum;			// ER Mapper style Datum name string, e.g. "RAW" or "NAD27". Will never be NULL
	char				*szProjection;		// ER Mapper style Projection name string, e.g. "RAW" or "WGS84". Will never be NULL
	int					EPSGcode;           // EPSG code. 

	BOOLEAN				bCompressedOffsetTable; // Is the block table compressed
} NCSEcwEditInfo;

NCSError NCS_CALL NCSEcwEditReadInfo(char *pFilename, NCSEcwEditInfo **ppInfo);
void NCS_CALL NCSEcwEditCopyInfo(NCSEcwEditInfo *pSrc, NCSEcwEditInfo **pDst);
NCSError NCS_CALL NCSEcwEditWriteInfo(char *pFilename, NCSEcwEditInfo *pInfo, void (*pProgressFunc)(UINT64 nTotal, UINT64 nWritten, void *pClientData), BOOLEAN (*pCancelFunc)(void *pClientData), void *pClientData);
NCSError NCS_CALL NCSEcwEditFreeInfo(NCSEcwEditInfo *pInfo);

namespace NCS {

class NCSECW_IMPEX CHeaderEditor {
public:
	CHeaderEditor();
	virtual ~CHeaderEditor();

	NCSError SetFile(char *pFileName);

	NCSEcwEditInfo *GetEditInfo() { return m_pEditInfo; };

	bool SetEditInfo(NCSEcwEditInfo *pEditInfo);

	// Get and Set members for UI convenience
	bool SetVersion(UINT8 nVersion);							// ECW file version == ERSWAVE_VERSION
	UINT8 GetVersion();

	bool SetCellSizeUnits(CellSizeUnits eCellSizeUnits);		// Units used for pixel size
	CellSizeUnits GetCellSizeUnits();

	bool SetCellIncrementX(IEEE8 fCellIncrementX);				// Increment in CellSizeUnits in X direction. May be negative. Will never be zero
	IEEE8 SetCellIncrementX();

	bool SetCellIncrementY(IEEE8 fCellIncrementY);				// Increment in CellSizeUnits in Y direction. May be negative. Will never be zero
	IEEE8 GetCellIncrementY();

	bool SetOriginX(IEEE8 fOriginX);							// World X origin for top-left corner of top-left cell, in CellSizeUnits
	IEEE8 GetOriginX();
	
	bool SetOriginY(IEEE8 fOriginY);							// World Y origin for top-left corner of top-left cell, in CellSizeUnits
	IEEE8 GetOriginY();

	bool SetDatum(char *szDatum);								// ER Mapper style Datum name string, e.g. "RAW" or "NAD27". Will never be NULL
	char *GetDatum();

	bool SetProjection(char *szProjection);						// ER Mapper style Projection name string, e.g. "RAW" or "WGS84". Will never be NULL
	char *GetProjection();

	bool SetEPSGCode(int EPSGcode);								// EPSG code. 
	int GetEPSGCode();

	bool SetCompressedOffsetTable(bool bCompressedOffsetTable);	// Is the block table compressed
	bool GetCompressedOffsetTable();

	NCSError ApplyHeaderChanges();

	void Cancel() { m_bCancelHeaderChanges = true; };
	bool IsCancelled() { return m_bCancelHeaderChanges; };
	static BOOLEAN WriteCancel(void *pData);

	UINT64 m_nTotal;
	UINT64 m_nCurrentProgress;
	static 	void WriteProgress(UINT64 nTotal, UINT64 nCurrent, void *pData);
	

private:
	char *m_szFileName;
	NCSEcwEditInfo *m_pEditInfo;

	bool m_bCancelHeaderChanges;
};

} //namespace NCS

#ifdef __cplusplus
}
#endif

#endif