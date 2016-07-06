/*
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
*/
#ifndef NCSJPCECWPIOSTREAM_H
#define NCSJPCECWPIOSTREAM_H

#include "NCSEcw/SDK/ECWPIOStream.h"

namespace NCS {
namespace JP2 {
	class CFile;
}
namespace JPC {

/**
 * CECWPIOStream class - 64bit ECWP IO "Stream" class.
 * This class is the IO mechanism used to access JP2 files via ECWP.
 * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
 */
class NCSECW_IMPEX CECWPIOStream: public SDK::CECWPIOStream {
public:
		/** Constructor, initialises members */
	CECWPIOStream(JP2::CFile *pFile, const CString &URLPath, const CString &HttpHeaders);

	virtual void ReceivedBlock(CPacketId nBlock, UINT8* pImage, UINT32 nBlockLength);

protected:
	JP2::CFile *m_pFile;

};

}} // namespace

#endif