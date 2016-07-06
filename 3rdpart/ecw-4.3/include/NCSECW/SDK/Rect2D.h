/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Rect2D.h $
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CNode2D class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRECT_H
#define NCSJPCRECT_H

#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H

#ifndef NCSMISC_H
#include "NCSMisc.h"
#endif // NCSMISC_H

const INT32 NCSJPC_MININT32 (-1 * (signed)0x7fffffff - 1);

namespace NCS {
namespace SDK {

	/**
	 * CRect class - the base JPC rectangle class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CRect2D {
public:
		/** Default constructor, initialises members */
	CRect2D() { 	
		m_X0 = NCSJPC_MININT32;
		m_Y0 = NCSJPC_MININT32;
		m_X1 = NCSJPC_MININT32;
		m_Y1 = NCSJPC_MININT32;
	};
		/** constructor, specify rect values */
	CRect2D(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1) { m_X0 = nX0; m_Y0 = nY0; m_X1 = nX1; m_Y1 = nY1; };
		/** constructor, specify rect values */
	CRect2D(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight) { m_X0 = nX0; m_Y0 = nY0; m_X1 = nX0 + nWidth; m_Y1 = nY0 + nHeight; };
		/** Virtual destructor */
	virtual ~CRect2D() {};
	
		/** 
		 * Get X0 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	NCS_INLINE INT32 GetX0() const { return(m_X0); };
		/** 
		 * Get Y0 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	NCS_INLINE INT32 GetY0() const { return(m_Y0); };
		/** 
		 * Get X1 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	NCS_INLINE INT32 GetX1() const { return(m_X1); };
		/** 
		 * Get Y1 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	NCS_INLINE INT32 GetY1() const { return(m_Y1); };
		/** 
		 * Get Width of this rect.
		 * @return      INT32		Width value.
		 */
	NCS_INLINE UINT32 GetWidth() const { return(m_X1 - m_X0); };
		/** 
		 * Get Height of this rect.
		 * @return      INT32		Height value.
		 */
	NCS_INLINE UINT32 GetHeight() const { return(m_Y1 - m_Y0);  };

	NCS_INLINE CRect2D &operator=(CRect2D& rect) {
			m_X0 = rect.GetX0();
			m_Y0 = rect.GetY0();
			m_X1 = rect.GetX1();
			m_Y1 = rect.GetY1();
			return(*this);
		} 

		/** 
		 * EQ operator.
		 * @return      bool		TF Rect is equal.
		 */
	NCS_INLINE bool operator==(const CRect2D& rect) const {
		return(m_X0 == rect.m_X0 && m_Y0 == rect.m_Y0 && m_X1 == rect.m_X1 && m_Y1 == rect.m_Y1);
	} 
		/** 
		 * NEQ operator.
		 * @return      bool		TF Rect is not equal.
		 */
	NCS_INLINE bool operator!=(const CRect2D& rect) const {
		return(m_X0 != rect.m_X0 || m_Y0 != rect.m_Y0 || m_X1 != rect.m_X1 || m_Y1 != rect.m_Y1);
	} 
		/** 
		 * LT operator.
		 * @return      bool		TF Rect is LT.
		 */
	NCS_INLINE bool operator<(const CRect2D& rect) const {
		return(m_X0 < rect.m_X0 || m_Y0 < rect.m_Y0);
	} 
		/** 
		 * Intersection operator.
		 * @return      CRect2D		Intersection of rects.
		 */
	CRect2D operator&(CRect2D& rect) const {
		return(CRect2D(NCSMax(GetX0(), rect.GetX0()),
					   NCSMax(GetY0(), rect.GetY0()),
					   NCSMin(GetX1(), rect.GetX1()),
					   NCSMin(GetY1(), rect.GetY1())));
	} 
		/** 
		 * Intersection Assignment operator.
		 * @return      CRect2D		Intersection of rects.
		 */
	CRect2D operator&=(CRect2D& rect) {
		CRect2D tmp(NCSMax(GetX0(), rect.GetX0()),
					NCSMax(GetY0(), rect.GetY0()),
					NCSMin(GetX1(), rect.GetX1()),
					NCSMin(GetY1(), rect.GetY1()));
		*this = tmp;
		return(tmp);
	} 

		/** 
		 * IsValid()
		 * @return		bool		TF Rect is valid (all points calculated)
		 */
	NCS_INLINE bool IsValid(void) {
		return(m_X0 != NCSJPC_MININT32 || m_Y0 != NCSJPC_MININT32 || m_X1 != NCSJPC_MININT32 || m_Y1 != NCSJPC_MININT32);
	}
protected:
		/** X0 of rectangle */
	INT32 m_X0;
		/** Y0 of rectangle */
	INT32 m_Y0;
		/** non-inclusive X1 of rectangle */
	INT32 m_X1;
		/** non-inclusive Y1 of rectangle */
	INT32 m_Y1;
};
}
}

#endif // !NCSJPCRECT_H
