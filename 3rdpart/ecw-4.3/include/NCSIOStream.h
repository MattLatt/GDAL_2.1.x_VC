/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     NCSIOStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSIOSTREAM_H
#define NCSIOSTREAM_H

// NCS 64bit FileIO routines
#ifndef NCSFILEIO_H
#include "NCSFileIO.h"
#endif // !NCSFILEIO_H

// Various NCS Headers
#include "NCSDefs.h"
#include "NCSError.h"

#ifndef NCSSTRING_H
#include "NCSString.h"
#endif

// STD vector template
#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

namespace NCS {

/**
 * CIOStream class - 64bit IO "Stream" class.
 * This class is the IO mechanism used to access JP2 files.
 * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: jjarvis $ $Date: 2012/06/27 $ 
 */
class NCSUTIL_IMPEX CIOStream: public CError {
public:
		/** Anonymous enum for Seek() origin. */
	typedef enum {
			/** Origin is start of file */
		START		= NCS_FILE_SEEK_START,
			/** Origin is from current position in file */
		CURRENT		= NCS_FILE_SEEK_CURRENT,
			/** Origin is from the end of the file */
		END			= NCS_FILE_SEEK_END
	} Origin;

		/** Default constructor, initialises members */
	CIOStream();
		/** Virtual destructor, releases members */
	virtual ~CIOStream();

	virtual CIOStream *Clone() = 0;

		/** 
		 * Get the current CError.
		 * @return      CError	CError object representing current error status of the Stream.
		 */
	virtual CError GetError() { return((CError)*this); };

		/** 
		 * Get the current filename for the stream.
		 * @return      _NCSTChar *	const pointer to filename for the stream if available, else NULL.
		 */
	virtual const CString &GetName() { return(m_Name); };

		/** 
		 * Open the stream on the specified file.
		 * @param		pName		Full Name of JP2 stream being parsed
		 * @param		bWrite		Open for writing flag.
		 * @return      CError	NCS_SUCCESS, or error code on failure.
		 */
	virtual CError Open(const CString &Name, bool bWrite = false);
		/** 
		 * Close the stream.
		 * @return      CError	NCS_SUCCESS, or error code on failure.
		 */
	virtual CError Close();

		/** 
		 * Seek on the file to the specified location.
		 * @param		offset		Signed 64bit offset to seek by
		 * @param		origin		Origin to calculate new position from.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
		/** 
		 * Is stream seekable.
		 * @return      bool		true if seekable, or false if not.
		 */
	virtual bool Seek();
		/** 
		 * Get the current file pointer position.
		 * @return      INT64		Current file pointer position, or -1 on error.
		 */
	virtual INT64 Tell();
		/** 
		 * Get the total current size of the file, in bytes.
		 * @return      INT64		Size of the file, or -1 on error.
		 */
	virtual INT64 Size();

		/** 
		 * Mark the current position in the file, so we can rewind it if required.
		 * Note: maintains a stack of marked positions.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Mark();
		/** 
		 * Rewind the file to the previously marked position.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Rewind();
		/** 
		 * UnMark the previously marked file position.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool UnMark();

		/** Is file open for writing 
		 * @return		bool		true is file os open for writing, otherwise false
		 */
	virtual bool IsWrite();

		/** Is IOStream buffered 
		 * @return		bool		true if IOStream is buffered (in application space)
		 */
	virtual bool IsBuffered() { return(false); }

		/** Is the stream a packet stream?  This means the SOD segment will be missing from
		 * the stream as parsed, and must be requested separately (ie, ECWP, JPIP)
		 * @return		bool		true this is a packet stream, otherwise false
		 */
	virtual bool IsPacketStream() { return(false); }

	virtual bool HasOpacityStreamAppended()  { return(m_bHasOpacityAppended); };
	virtual void SetOpacityStreamAppended(bool value)  {m_bHasOpacityAppended = value; };
	virtual bool IsOpacityStream()  { return(m_bIsOpacity); };
	virtual void SetOpacityStream(bool value)  {m_bIsOpacity = value; };

		/** Packet stream subclasses overload this to process received packets on a 
		 * regular basis
		 */
	virtual void ProcessReceivedPackets() {};
		/** 
		 * Read some data from the stream into the supplied buffer.
		 * @param		buffer		Buffer to read the data into
		 * @param		count		How many bytes of data to read.
		 * @param		pRead		How many bytes of data were actually read.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Read(void* buffer, UINT32 count);
		/** 
		 * Read some data from the stream into the supplied buffer.  Does NOT update file pointer.
		 * @param		buffer		Buffer to read the data into
		 * @param		count		How many bytes of data to read.
		 * @param		pRead		How many bytes of data were actually read.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Read(INT64 offset, void* buffer, UINT32 count);
		/** 
		 * Write some data to the stream.
		 * @param		buffer		Buffer of data to write to the stream
		 * @param		count		How many bytes of data to write to the stream.
		 * @param		pWrote		How many bytes of data were actually written to the stream.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Write(void* buffer, UINT32 count);
		
	virtual void FlushFileBuffers() { ; }
	
		/** 
		 * Read aMSB  UINT8 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(UINT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
		/** 
		 * Read a MSB UINT16 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(UINT16 &Buffer);
		/** 
		 * Read a MSB UINT32 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(UINT32 &Buffer);
		/** 
		 * Read a MSB UINT64 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(UINT64 &Buffer);
		/** 
		 * Read aMSB  INT8 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(INT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
		/** 
		 * Read a MSB INT16 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(INT16 &Buffer);
		/** 
		 * Read a MSB INT32 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(INT32 &Buffer);
		/** 
		 * Read a MSB INT64 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(INT64 &Buffer);
		/** 
		 * Read a MSB IEEE4 float from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(IEEE4 &Buffer);
		/** 
		 * Read a MSB IEEE8 double from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadMSB(IEEE8 &Buffer);
		/** 
		 * Write aMSB  UINT8 to the stream.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	NCS_INLINE virtual bool WriteMSB(UINT8 nValue) { return(Write(&nValue, sizeof(nValue))); };
		/** 
		 * Write a MSB UINT16 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(UINT16 nValue);
		/** 
		 * Write a MSB UINT32 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(UINT32 nValue);
		/** 
		 * Write a MSB UINT64 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(UINT64 nValue);
		/** 
		 * Write aMSB  INT8 to the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	NCS_INLINE virtual bool WriteMSB(INT8 nValue) { return(Write(&nValue, sizeof(nValue))); };
		/** 
		 * Write a MSB INT16 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(INT16 nValue);
		/** 
		 * Write a MSB INT32 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(INT32 nValue);
		/** 
		 * Write a MSB INT64 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(INT64 nValue);
		/** 
		 * Write a MSB IEEE4 float to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(IEEE4 fValue);
		/** 
		 * Write a MSB IEEE8 double to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteMSB(IEEE8 dValue);
		/** 
		 * Read aMSB  UINT8 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(UINT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
		/** 
		 * Read aMSB  UINT16 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(UINT16 &Buffer);
		/** 
		 * Read a LSB UINT32 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(UINT32 &Buffer);
		/** 
		 * Read a LSB UINT64 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(UINT64 &Buffer);
		/** 
		 * Read aMSB  INT8 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(INT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
		/** 
		 * Read a LSB INT16 from the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(INT16 &Buffer);
		/** 
		 * Read a LSB INT32 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(INT32 &Buffer);
		/** 
		 * Read a LSB INT64 from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(INT64 &Buffer);
		/** 
		 * Read a LSB IEEE4 float from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(IEEE4 &Buffer);
		/** 
		 * Read a LSB IEEE8 double from the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ReadLSB(IEEE8 &Buffer);
		/** 
		 * Write aMSB  UINT8 to the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(UINT8 &Buffer) { return(Write(&Buffer, sizeof(Buffer))); };
		/** 
		 * Write a LSB UINT16 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(UINT16 nValue);
		/** 
		 * Write a LSB UINT32 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(UINT32 nValue);
		/** 
		 * Write a LSB UINT64 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(UINT64 nValue);
		/** 
		 * Write aMSB  INT8 to the stream.
		 * @param		buffer		Buffer to receive the data
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(INT8 &Buffer) { return(Write(&Buffer, sizeof(Buffer))); };
		/** 
		 * Write a LSB INT16 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(INT16 nValue);
		/** 
		 * Write a LSB INT32 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(INT32 nValue);
		/** 
		 * Write a LSB INT64 to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(INT64 nValue);
		/** 
		 * Write a LSB IEEE4 float to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(IEEE4 fValue);
		/** 
		 * Write a LSB IEEE8 double to the stream, byteswapped if necessary.
		 * @param		buffer		Buffer to write to the stream
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool WriteLSB(IEEE8 dValue);	

		/** 
		 * Reset values for bit (Un)stuffing.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool ResetBitStuff();
		/** 
		 * Flush values for bit (Un)stuffing.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool FlushBitStuff();
		/** 
		 * Stuff a bit to the JPC bitdata stream.
		 * @param		bool		Buffer to write bit value to stream from.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool Stuff(bool bBit);
		/** 
		 * UnStuff a bit from the JPC bitdata stream.
		 * @param		bool		Buffer to read bit from stream to.
		 * @return      bool		true, or false on failure.  Instance inherits CError object containing error value.
		 */
	virtual bool UnStuff(bool &bBit);
	
#ifndef NCS_NO_COMPAT_NAMES
	bool WriteUINT32(UINT32 nValue) {return WriteMSB(nValue);};
	bool WriteIEEE4(IEEE4 fValue) {return WriteMSB(fValue);};
	bool WriteUINT16(UINT16 nValue) {return WriteMSB(nValue);};
	bool WriteUINT64(UINT64 nValue) {return WriteMSB(nValue);};
	bool WriteIEEE8(IEEE8 dValue)  {return WriteMSB(dValue);};
	bool WriteUINT8(UINT8 nValue) {return WriteMSB(nValue);};
	bool ReadUINT32(UINT32& buffer) {return ReadMSB(buffer);};
	bool ReadIEEE4(IEEE4& buffer) {return ReadMSB(buffer);};
	bool ReadUINT16(UINT16& buffer) {return ReadMSB(buffer);};
	bool ReadUINT64(UINT64& buffer) {return ReadMSB(buffer);};
	bool ReadIEEE8(IEEE8& buffer) {return ReadMSB(buffer);};
	bool ReadUINT8(UINT8& buffer) {return ReadMSB(buffer);};
#endif

protected:
		/** name stream is open on */
	CString			m_Name;

		/** Stream is open for writing */
	bool			m_bWrite;
	bool			m_bIsOpacity;
	bool			m_bHasOpacityAppended;
		/** Stack of marks in the stream */
	std::vector<INT64>	m_Marks;
		/** File pointer */
	INT64			m_nOffset;

		/** Current byte(s) when bit stuffing/unstuffing */
	UINT16	m_nThisBytes;
		/** # of bits left */
	UINT8	m_nBitsLeft;
};

typedef std::vector<CIOStream *> CIOStreamVector;
}

#pragma warning(pop)


#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CIOStream CNCSJPCIOStream;
#define WriteUINT32(X) WriteMSB(X)
#define WriteIEEE4(X) WriteMSB(X)
#define WriteUINT16(X) WriteMSB(X)
#define WriteUINT64(X) WriteMSB(X)
#define WriteIEEE8(X) WriteMSB(X)
#define WriteUINT8(X) WriteMSB(X)
#define ReadUINT32(X) ReadMSB(X)
#define ReadIEEE4(X) ReadMSB(X)
#define ReadUINT16(X) ReadMSB(X)
#define ReadUINT64(X) ReadMSB(X)
#define ReadIEEE8(X) ReadMSB(X)
#define ReadUINT8(X) ReadMSB(X)
#endif

/**
 * Some time-saving CHECKIO Macros.
 * Ideally we'd use exceptions instead, but they aren't portable to all compilers/platforms,
 * so revert to a low-tech solution.
 * 
 * @param        Error		Reference to error to construct from
 */
#define NCSJP2_CHECKIO_BEGIN(e, s)	while(e == NCS_SUCCESS) { \
												CError *pErrorNCSJP2_CHECKIO = &e;\
												CIOStream *pStreamNCSJP2_CHECKIO = &s
#define NCSJP2_CHECKIO_ERROR_BEGIN(e)	while(e == NCS_SUCCESS) { \
												CError *pErrorNCSJP2_CHECKIO = &e
												
#define NCSJP2_CHECKIO(a)		if(pStreamNCSJP2_CHECKIO->a == false) { *pErrorNCSJP2_CHECKIO = *pStreamNCSJP2_CHECKIO; break; }
#define NCSJP2_CHECKIO_ERROR(a)	*pErrorNCSJP2_CHECKIO = a; if(*pErrorNCSJP2_CHECKIO != NCS_SUCCESS) break
#define NCSJP2_CHECKIO_END()		break; }

#endif // !NCSIOSTREAM_H
