/*
 * UnixDomainSocket.cpp
 *
 *  Created on: 03-Aug-2016
 *      Author: psajitha
 */

#include "LOG/Log.h"
#include "UnixDomainSocket.h"

#include "ITCM/common/ByteStream.h"
#include "ITCM/Configuration/ConfigConstants.h"

//char* socket_path = "\0hidden";
#define BACKLOG 10
#define UDS_CONFIG_GROUP "UDS"

using namespace ipc;
using namespace utils;
using namespace ITCM::Common;
using namespace ITCM::ConfigConstants;

//EndPoint::objConfigurationData;

UnixDomainSocket::UnixDomainSocket() {


}


UnixDomainSocket::UnixDomainSocket(EndPoint::ProtocolType protocolType)
{

	switch(protocolType)
	{
	case EndPoint::TCP:
		m_bIsTCP =true;
		break;
	case EndPoint::UDP:
		m_bIsTCP =false;
		break;

	}

	m_dSocketFD = -1;

	memset ( &m_socket_address, 0, sizeof ( m_socket_address ) );

}

UnixDomainSocket::~UnixDomainSocket()
{

}

/****************************************************************************************************
 * Function name	: initializeSocket
 * Description		: This function  specifies the protocol family and
 * 					the type of socket (SOCK_STREAM for stream sockets and SOCK_DGRAM for datagram sockets).
 * Parameters		: void
 * Return value		: int - return the file descriptor
 ******************************************************************************************************/
int UnixDomainSocket::initializeSocket()
{

	m_totalBytesRead = 0;
	m_bytesAllocated = 0;
	m_bufferSize = 1024;

	string strSocketName = "sock";

	m_strSocketName = EndPoint::objConfigurationData->getStringTypeItemValue("UDS", "SOCKET_NAME", strSocketName);
	if(m_bIsTCP)
	{
		if ( (m_dSocketFD = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			perror("socket error");
			exit(-1);
		}

		Log::trace(__FILE__, __LINE__," created stream sockets " );
	}
	else
	{
		if ( (m_dSocketFD = socket(AF_UNIX,  SOCK_DGRAM, 0)) == -1) {
			perror("socket error");
			exit(-1);
		}

		Log::trace(__FILE__, __LINE__, " created datagram sockets ");
	}

	memset(&m_socket_address, 0, sizeof(m_socket_address));
	m_socket_address.sun_family = AF_UNIX;


	strncpy(m_socket_address.sun_path,m_strSocketName.c_str(),sizeof(m_socket_address.sun_path)-1);

	//fcntl(m_dSocketFD, F_SETFL, O_NONBLOCK);

	return m_dSocketFD;


}


/****************************************************************************************************
 * Function name	: connectToServer
 * Description		: This function is used to establish the connection to the server.
 * Parameters		: void
 * Return value		: bool
 ******************************************************************************************************/
bool UnixDomainSocket::connectToServer()
{

	if (connect(m_dSocketFD, (struct sockaddr *)&m_socket_address, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		return false;
	}
	return true;
}

/****************************************************************************************************
 * Function name	: bindSocket
 * Description		: This function assigns a local protocol address to a socket
 * Parameters		: void
 * Return value		: bool
 ******************************************************************************************************/
bool UnixDomainSocket::bindSocket()
{

	if (bind(m_dSocketFD, (struct sockaddr *)&m_socket_address, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		return false;
	}

	return true;
}


/****************************************************************************************************
 * Function name	: acceptRequest
 * Description		: This function is used to retrieve a connect request and convert that into a request
 * Parameters		: void
 * Return value		: int - return the file descriptor
 ******************************************************************************************************/
int UnixDomainSocket::acceptRequest() {

	socklen_t addr_length = sizeof ( m_socket_address );
	m_dSocketFD = accept(m_dSocketFD, (struct sockaddr *)&m_socket_address, &addr_length);

	if(m_dSocketFD == -1)
	{
		perror("accept1");
	}

	return m_dSocketFD;

}


/****************************************************************************************************
 * Function name	: listenSocket
 * Description		: This function converts an unconnected socket into a passive socket, indicating
 * 					that the kernel should accept incoming connection requests directed to this socket.
 * Parameters		: void
 * Return value		: bool
 ******************************************************************************************************/
bool UnixDomainSocket::listenSocket() {

	int nMaxConnections = 5;
	m_dMaxConnections = EndPoint::objConfigurationData->getIntTypeItemValue("UDS", "MAX_CONNECTIONS", nMaxConnections);

	if(m_bIsTCP)
	{
		if (listen(m_dSocketFD, m_dMaxConnections) == -1) {
			perror("listen");
			return false;
		}
	}

	return true;
}



/****************************************************************************************************
 * Function name	: setNonBlocking
 * Description		: This function is used to set the communication in non blocking mode using fcntl()
 * Parameters		: int - the file descriptor
 * Return value		: void
 ******************************************************************************************************/
void UnixDomainSocket::setNonBlocking(int socketFD)
{
	fcntl (socketFD, F_SETFL,O_NONBLOCK);
}

ssize_t UnixDomainSocket::readFromFD(int fd, boost::uint8_t *buffer, size_t size)
{
	if((m_bytesAllocated - m_totalBytesRead) < size)
	{
		boost::uint8_t *new_buffer = (boost::uint8_t*) realloc(buffer, (m_bytesAllocated + m_bufferSize) * sizeof(boost::uint8_t));

		if(new_buffer == NULL)
		{
			Log::trace(__FILE__, __LINE__, "buffer cannot be resized");
			return -1;
		}

		buffer = new_buffer;
		m_bytesAllocated += m_bufferSize;
	}
	if(m_bytesAllocated >= MAX_EMP_MESSAGE_LENGTH)
	{
		Log::trace(__FILE__, __LINE__, "buffer size exceeds the permitted message length");
		return -1;
	}

	return read(fd, buffer + m_totalBytesRead, size);
}


/****************************************************************************************************
 * Function name	: readData
 * Description		: This function is used read the written data from the client/server
 * Parameters		: void
 * Return value		: struct message_struct
 ******************************************************************************************************/
EmpFullMessage* UnixDomainSocket::readData()
{
	ssize_t bytesRead;

	boost::uint32_t m_dataLength;
	boost::uint8_t m_varHeaderSize;
	boost::uint32_t m_dataIntegrity;
	boost::uint8_t *buffer = (boost::uint8_t*) malloc(m_bufferSize * sizeof(boost::uint8_t));

	m_totalBytesRead = 0;
	m_bytesAllocated = m_bufferSize;

	if(m_dSocketFD == -1)
	{
		Log::error(__FILE__, __LINE__, "Socket is not opened in read mode.");
		return NULL;
	}

	bytesRead = readFromFD(m_dSocketFD, buffer, sizeof(emp_message_part_before_length) - 1);
	m_totalBytesRead += bytesRead;

	bytesRead = readFromFD(m_dSocketFD, buffer, DATA_LENGTH_BYTES);

	ByteStream lengthStream(buffer + m_totalBytesRead, bytesRead);
	lengthStream >> Common::UnsignedInteger<DATA_LENGTH_BYTES > (m_dataLength);

	cout << "data length:" << static_cast<int>(m_dataLength) << endl;
	m_totalBytesRead += bytesRead;

	bytesRead = readFromFD(m_dSocketFD, buffer, sizeof(emp_message_part_after_length));
	m_totalBytesRead += bytesRead;

	bytesRead = readFromFD(m_dSocketFD, buffer, sizeof(m_varHeaderSize));

	cout << "gTotalBytesRead before var header:" << m_totalBytesRead << "buffer value:" << (int)buffer[m_totalBytesRead] << endl;
	ByteStream varHeaderStream(buffer + m_totalBytesRead, bytesRead);
	varHeaderStream >> m_varHeaderSize;

	cout << "var header size:" << static_cast<int>(m_varHeaderSize) << endl;
	m_totalBytesRead += bytesRead;

	bytesRead = readFromFD(m_dSocketFD, buffer, m_varHeaderSize + m_dataLength + sizeof(m_dataIntegrity));
	m_totalBytesRead += bytesRead;

	cout << "total bytes:" << m_totalBytesRead << endl;

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesRead]);
	memcpy(messageBytes.get(), buffer, m_totalBytesRead);
	EmpFullMessage *message = new EmpFullMessage(messageBytes.get(), m_totalBytesRead);

//	free(buffer);

	if(bytesRead != -1)
		return message;
	else
		return NULL;


//	char string_read[255];
//
//	struct message_struct *message_t;
//	message_t = (message_struct*) malloc(sizeof(message_struct));
//
//	int rcFD = read(m_dSocketFD,&message_t,sizeof(message_struct));
//
//	if(rcFD < 0)
//		perror("reading stream message");
//	else if(rcFD == 0)
//		Log::trace(__FILE__, __LINE__, "Ending connection.");


//	return message_t;

}


/****************************************************************************************************
 * Function name	: writeData
 * Description		: This function is used write data into client/server
 * Parameters		: struct message_struct
 * Return value		: void
 ******************************************************************************************************/
void UnixDomainSocket::writeData(EmpFullMessage *message)
{
	if(m_dSocketFD == -1)
		Log::error(__FILE__, __LINE__, "Socket is not opened in write mode.");

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message->GetMessageSize()]);
	message->AsBytes(bytes.get(), message->GetMessageSize());

	if(message->GetMessageSize() > MAX_EMP_MESSAGE_LENGTH)
	{
		Log::trace(__FILE__, __LINE__, "Cannot send message. Message size exceeds maximum permitted length");
		return;
	}

	if(m_bIsTCP)
	{
		if (write(m_dSocketFD, bytes.get(), message->GetMessageSize()) < 0)
			perror("writing on sockets");
	}
	else
	{
		int addr_length = sizeof(m_socket_address);
		//if (sendto(m_dSocketFD, &message, sizeof(message_struct), 0, (sockaddr *)&m_socket_address, (socklen_t *) &addr_length ) < 0)
		{
			perror("sending datagram message");
		}
	}
}


/****************************************************************************************************
 * Function name	: destroy
 * Description		: This function is used to deallocate the resources used in the client/server
 * Parameters		: void
 * Return value		: void
 ******************************************************************************************************/
void UnixDomainSocket::destroy()
{
	close(m_dSocketFD);
	unlink(m_strSocketName.c_str());
}


bool UnixDomainSocket::getCurrentState()
{
	return true;
}

