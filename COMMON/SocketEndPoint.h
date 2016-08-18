/*
 * SocketEndPoint.h
 *
 *  Created on: 12-Aug-2016
 *      Author: psajitha
 */

#ifndef COMMON_SOCKETENDPOINT_H_
#define COMMON_SOCKETENDPOINT_H_

#include <string>

#include "ITCM/EmpFullMessage.h"

using namespace std;
using namespace ITCM;

namespace ipc
{

class SocketEndPoint
{

public:
	SocketEndPoint();
	virtual ~SocketEndPoint();

	static void initConfigItems(string szFileName);

	// initialize the object
	virtual int initializeSocket()                 = 0;

	virtual bool connectToServer() = 0;

	virtual bool bindSocket() = 0;

	virtual bool listenSocket() = 0;

	virtual int acceptRequest() = 0;

	// functions used to read and write the data into/from the buffer
	virtual EmpFullMessage* readData() = 0;

	virtual void writeData(EmpFullMessage *message)     = 0;

	virtual void setNonBlocking(int socketFD) = 0;

	// function used to clear the data or shutdown the mechanism
	virtual void destroy()              = 0;

	// functions to get the current state of the object, true or false. Every thing is OK or not
	virtual bool getCurrentState() = 0;


};

}
#endif /* COMMON_SOCKETENDPOINT_H_ */
