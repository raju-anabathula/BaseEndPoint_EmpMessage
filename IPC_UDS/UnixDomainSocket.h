/*
 * UnixDomainSocket.h
 *
 *  Created on: 03-Aug-2016
 *      Author: psajitha
 */

#ifndef IPC_UDS_UNIXDOMAINSOCKET_H_
#define IPC_UDS_UNIXDOMAINSOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#include "COMMON/SocketEndPoint.h"
#include "COMMON/EndPoint.h"

namespace ipc
{
class UnixDomainSocket : public SocketEndPoint
{
private :

	int m_dSocketFD;
	struct sockaddr_un m_socket_address;

	bool m_bIsTCP;
	string m_strSocketName;
	int m_dMaxConnections;

public:


	UnixDomainSocket();
	UnixDomainSocket(EndPoint::ProtocolType protocolType);
	virtual ~UnixDomainSocket();


	/****************************************************************************************************
	 * Function name	: readData
	 * Description		: This function is used read the written data from the client/server
	 * Parameters		: void
	 * Return value		: struct message_struct
	 ******************************************************************************************************/
	EmpFullMessage* readData();


	/****************************************************************************************************
	 * Function name	: writeData
	 * Description		: This function is used write data into client/server
	 * Parameters		: struct message_struct
	 * Return value		: void
	 ******************************************************************************************************/
	void writeData(EmpFullMessage *message);

	/****************************************************************************************************
	 * Function name	: destroy
	 * Description		: This function is used to deallocate the resources used in the client/server
	 * Parameters		: void
	 * Return value		: void
	 ******************************************************************************************************/
	void destroy();


	/****************************************************************************************************
	 * Function name	: initializeSocket
	 * Description		: This function  specifies the protocol family and
	 * 					the type of socket (SOCK_STREAM for stream sockets and SOCK_DGRAM for datagram sockets).
	 * Parameters		: void
	 * Return value		: int - return the file descriptor
	 ******************************************************************************************************/
	int initializeSocket();

	/****************************************************************************************************
	 * Function name	: connectToServer
	 * Description		: This function is used to establish the connection to the server.
	 * Parameters		: void
	 * Return value		: bool
	 ******************************************************************************************************/
	bool connectToServer();


	/****************************************************************************************************
	 * Function name	: bindSocket
	 * Description		: This function assigns a local protocol address to a socket
	 * Parameters		: void
	 * Return value		: bool
	 ******************************************************************************************************/
	bool bindSocket();


	/****************************************************************************************************
	 * Function name	: acceptRequest
	 * Description		: This function is used to retrieve a connect request and convert that into a request
	 * Parameters		: void
	 * Return value		: int - return the file descriptor
	 ******************************************************************************************************/
	int acceptRequest();


	/****************************************************************************************************
	 * Function name	: listenSocket
	 * Description		: This function converts an unconnected socket into a passive socket, indicating
	 * 					that the kernel should accept incoming connection requests directed to this socket.
	 * Parameters		: void
	 * Return value		: bool
	 ******************************************************************************************************/
	bool listenSocket();


	/****************************************************************************************************
	 * Function name	: setNonBlocking
	 * Description		: This function is used to set the communication in non blocking mode using fcntl()
	 * Parameters		: int - the file descriptor
	 * Return value		: void
	 ******************************************************************************************************/
	void setNonBlocking(int socketFD);

	bool getCurrentState();

	size_t m_totalBytesRead;
	size_t m_bytesAllocated;
	size_t m_bufferSize;

	ssize_t readFromFD(int fd, boost::uint8_t *buffer, size_t size);

};

}
#endif /* IPC_UDS_UNIXDOMAINSOCKET_H_ */
