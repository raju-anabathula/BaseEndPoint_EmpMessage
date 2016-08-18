/*
 * Pipe.cpp
 * Created on: 19-Jul-2016
 * Author: Raju Anabathula
 * Purpose: This class implements the Unix Pipe mechanism by exploiting the default system calls.
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "Pipe.h"
#include "LOG/Log.h"
#include "ITCM/common/ByteStream.h"
#include "ITCM/Configuration/ConfigConstants.h"

using namespace ipc;
using namespace utils;
using namespace ITCM::Common;
using namespace ITCM::ConfigConstants;

Pipe::Pipe()
{
	m_readFD = -1;
	m_writeFD = -1;

	gTotalBytesRead = 0;
	gBytesAllocated = 0;
	gBufferSize = 1024;
}

Pipe::~Pipe()
{
}

/****************************************************************************************************
 * Function name	: init
 * Description		: This function initializes the file descriptors needed for the read/write operations.
 * Parameters		: void
 * Return value		: void
 ******************************************************************************************************/
void Pipe::initialize(string strName)
{
	if(pipe(m_fds))
	{
		Log::error(__FILE__, __LINE__, "Cannot create pipe::pipe() failed");
		exit(EXIT_FAILURE);
	}

	m_readFD = m_fds[0];
	m_writeFD = m_fds[1];
}

ssize_t Pipe::readFromFD(int fd, boost::uint8_t *buffer, size_t size)
{
	if((gBytesAllocated - gTotalBytesRead) < size)
	{
		boost::uint8_t *new_buffer = (boost::uint8_t*) realloc(buffer, (gBytesAllocated + gBufferSize) * sizeof(boost::uint8_t));

		if(new_buffer == NULL)
		{
			Log::trace(__FILE__, __LINE__, "buffer cannot be resized");
			return -1;
		}

		buffer = new_buffer;
		gBytesAllocated += gBufferSize;
	}
	if(gBytesAllocated >= MAX_EMP_MESSAGE_LENGTH)
	{
		Log::trace(__FILE__, __LINE__, "buffer size exceeds the permitted message length");
		return -1;
	}

	return read(fd, buffer + gTotalBytesRead, size);
}

/****************************************************************************************************
 * Function name	: readData
 * Description		: This function is used read the written data from the pipe
 * Parameters		: void
 * Return value		: string - data read from the pipe
 ******************************************************************************************************/
EmpFullMessage* Pipe::readData()
{
	Log::info(__FILE__, __LINE__, "in readData()");

	ssize_t bytesRead;

	boost::uint32_t m_dataLength;
	boost::uint8_t m_varHeaderSize;
	boost::uint32_t m_dataIntegrity;
	boost::uint8_t *buffer = (boost::uint8_t*) malloc(gBufferSize * sizeof(boost::uint8_t));

	gTotalBytesRead = 0;
	gBytesAllocated = gBufferSize;

	if(m_readFD == -1)
	{
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");
		return NULL;
	}

	bytesRead = readFromFD(m_readFD, buffer, sizeof(emp_message_part_before_length) - 1);
	gTotalBytesRead += bytesRead;

	bytesRead = readFromFD(m_readFD, buffer, DATA_LENGTH_BYTES);

	ByteStream lengthStream(buffer + gTotalBytesRead, bytesRead);
	lengthStream >> Common::UnsignedInteger<DATA_LENGTH_BYTES > (m_dataLength);

	cout << "data length:" << static_cast<int>(m_dataLength) << endl;
	gTotalBytesRead += bytesRead;

	bytesRead = readFromFD(m_readFD, buffer, sizeof(emp_message_part_after_length));
	gTotalBytesRead += bytesRead;

	bytesRead = readFromFD(m_readFD, buffer, sizeof(m_varHeaderSize));

	cout << "gTotalBytesRead before var header:" << gTotalBytesRead << "buffer value:" << (int)buffer[gTotalBytesRead] << endl;
	ByteStream varHeaderStream(buffer + gTotalBytesRead, bytesRead);
	varHeaderStream >> m_varHeaderSize;

	cout << "var header size:" << static_cast<int>(m_varHeaderSize) << endl;
	gTotalBytesRead += bytesRead;

	bytesRead = readFromFD(m_readFD, buffer, m_varHeaderSize + m_dataLength + sizeof(m_dataIntegrity));
	gTotalBytesRead += bytesRead;

	cout << "total bytes:" << gTotalBytesRead << endl;

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[gTotalBytesRead]);
	memcpy(messageBytes.get(), buffer, gTotalBytesRead);
	EmpFullMessage *message = new EmpFullMessage(messageBytes.get(), gTotalBytesRead);

	if(bytesRead != -1)
		return message;
	else
		return NULL;
}

/****************************************************************************************************
 * Function name	: writeData
 * Description		: This function is used write the data to pipe
 * Parameters		: string - data to write to the pipe
 * Return value		: void
 ******************************************************************************************************/
void Pipe::writeData(EmpFullMessage *message)
{

	if(m_writeFD == -1)
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message->GetMessageSize()]);
	message->AsBytes(bytes.get(), message->GetMessageSize());

	if(message->GetMessageSize() > MAX_EMP_MESSAGE_LENGTH)
	{
		Log::trace(__FILE__, __LINE__, "Cannot send message. Message size exceeds maximum permitted length");
		return;
	}

	if(write(m_writeFD, bytes.get(), message->GetMessageSize()) == -1)
	{
		Log::error(__FILE__, __LINE__, "cannot write to the fifo.");
	}
	else
	{
		Log::trace(__FILE__, __LINE__, "Written to fifo:");
	}
}

/****************************************************************************************************
 * Function name	: destroy
 * Description		: This function is used to deallocate the resources used in the pipe
 * Parameters		: void
 * Return value		: void
 ******************************************************************************************************/
void Pipe::destroy()
{
}

bool Pipe::getCurrentState()
{
	return true;
}
