/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     QuantizationParameter.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CQuantizationParameter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCQUANTIZATIONPARAMETER_H
#define NCSJPCQUANTIZATIONPARAMETER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {

	/**
	 * CQuantizationParameter class - the JPC Quantization Parameters.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2011/11/18 $ 
	 */	
class NCSECW_IMPEX CQuantizationParameter {
public:
	typedef enum {
		NONE				= 0x0,
		SCALAR_DERIVED		= 0x1,
		SCALAR_EXPOUNDED	= 0x2
	} Style;

	typedef struct {
		Style	m_eStyle;	
		UINT8				m_nGuardBits;
	} Values;

	typedef struct {
		UINT16		m_nMantissa;
		UINT8		m_nExponent;
	} Irreversible;

	typedef union {
		UINT8			m_nReversible;
		Irreversible	m_Irreversible;
	} Parameters;

	Values		m_Sqc;
	std::vector<Parameters>	m_SPqc;

		/** Default constructor, initialises members */
	CQuantizationParameter();
		/** Virtual destructor */
	virtual ~CQuantizationParameter();

		/** 
		 * Parse the fields from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		nLength		# of bytes of the marker to parse
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, INT32 nLength);
		/** 
		 * UnParse the fields to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

	UINT64 GetMemorySize(); // Against bug EMS-407, added 02/11/2011

};
}
}

#endif // !NCSJPCQUANTIZATIONPARAMETER_H
