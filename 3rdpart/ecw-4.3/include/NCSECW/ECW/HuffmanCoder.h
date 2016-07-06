/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
** 
** FILE:   	HuffmanCoder.h
** CREATED:	30 Jun 2004
** AUTHOR: 	Simon Cope
** PURPOSE:	CHuffmanCoder class header
** EDITS:
*******************************************************/

#ifndef NCSHUFFMANCODER_H
#define NCSHUFFMANCODER_H

#pragma warning(push)
#pragma warning(disable : 4786)

#include "NCSDefs.h"

#ifdef __cplusplus

#include <map>
#include <vector>

extern "C" {
#endif // __cplusplus

typedef struct _NCSHuffmanSymbol {
	UINT16 nValue;
	BOOLEAN bZeroRun;
} NCSHuffmanSymbol;

#ifdef __cplusplus
};
#include "NCSError.h"

namespace NCS {
namespace ECW {
/**
 * CHuffmanCoder class - ECW Huffman Coder/Decoder.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
 */
class CHuffmanCoder {
public:
	const static UINT32 _NUM_SYMBOL_VALUES;
	const static UINT16 _SIGN_MASK;
	const static UINT16 _RUN_MASK;
	const static INT16 _MAX_BIN_VALUE;
	const static INT16 _MIN_BIN_VALUE;
	const static UINT16 _VALUE_MASK;
	const static UINT16 _MAX_RUN_LENGTH;
	const static UINT8 _SMALL_SYMBOL;
	const static UINT8 _SMALL_SHIFT;

	/*
	**	Huffman structures and definitions
	*/
	class CTree;

	class CCodeNode {  
	public:
		typedef struct {
			class CCodeNode *m_p0Child;
			class CCodeNode *m_p1Child;
		} A;
		typedef union {
			A				m_P;
			class CCodeNode *m_Children[2];
		} U;
		U m_Children;

		NCSHuffmanSymbol m_Symbol;
		UINT32	m_nFrequency;
		class CCodeNode	*m_pPrev;  
		class CCodeNode	*m_pNext;  
		UINT32 	m_nCode;
		UINT8	m_nCodeBits;
		bool	m_bInHistogram;

		CCodeNode();
		CCodeNode(const CCodeNode&);
		//CCodeNode(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState);
		virtual ~CCodeNode();

		void Pack(UINT8 **ppPacked, UINT32 &nNodes);
		//CCodeNode *Unpack(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState);
		UINT8 SetCode(UINT32 nCode, UINT8 nCodeBits);
		void BuildLut(CTree &tree);
	};
	class CTree: public CCodeNode {
	public:
		CCodeNode *m_pRoot;
		CCodeNode *m_Histogram[65536];
		
		CTree();
		virtual ~CTree();

		CError Pack(UINT8 **ppPacked, INT16 *pUnPacked, UINT32 nSymbols);
	protected:
		CCodeNode *UnpackNode(UINT8 **ppPacked, UINT32 &nNodes);
	};

//	typedef struct {
//		UINT16			m_Children;
//		UINT16			m_nValue;
//		UINT32 nPAD;
//	} State;
	class State {
	public:
		UINT16			m_Children;
		UINT16			m_nValue;
		UINT32 nPAD;
		NCS_INLINE State() { nPAD = 0; };
	};

	UINT16 m_nStates;
	State *m_pTable;
	UINT32 m_nNodes; // Shao added to prevent heap (m_pTable) corruption when file corrupted

		/** Default constructor. */
	CHuffmanCoder();
		/** virtual default destructor. */
	virtual ~CHuffmanCoder();

	CError Pack(UINT8 *pPacked, UINT32 *pPackedLength, INT16 *pUnPacked, UINT32 nRawLength);
	CError UnPack(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);
private:
	CTree *m_pTree;

	CError Unpack(UINT8 **ppPacked);
	void Unpack(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT8 nCodeBits);
};
}
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NCS_HUFFMAN_MAX_RUN_LENGTH 0x7fff
#define NCS_HUFFMAN_SIGN_MASK	0x4000
#define NCS_HUFFMAN_VALUE_MASK	0x3fff
#define NCS_HUFFMAN_RUN_MASK	0x8000

typedef struct {
	void	*pTree;
	UINT32	nBitsUsed;		
} NCSHuffmanState;

NCSError unpack_huffman(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);
void unpack_huffman_init_state(NCSHuffmanState *pState, UINT8 **ppPacked);
void unpack_huffman_fini_state(NCSHuffmanState *pState);
NCSHuffmanSymbol *unpack_huffman_symbol(UINT8 **ppPacked, NCSHuffmanState *pState);
static NCS_INLINE BOOLEAN unpack_huffman_symbol_zero_run(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->bZeroRun);
			}
static NCS_INLINE UINT16 unpack_huffman_symbol_zero_length(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->nValue);
			};
static NCS_INLINE INT16 unpack_huffman_symbol_value(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->nValue);
			};

UINT32	pack_huffman(UINT8 *pPacked, INT16 *pUnPacked, UINT32 symbol_stream_length);
#ifdef __cplusplus
};
#endif

#pragma warning(pop)

#endif /* NCSHUFFMANCODER_H */
