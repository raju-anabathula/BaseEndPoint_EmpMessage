/*
 * Pipe.h
 *
 * Created on: 19-Jul-2016
 * Author: Raju Anabathula
 * Purpose: This class implements the Unix Pipe mechanism by exploiting the default system calls.
 */

#ifndef PIPE_H_
#define PIPE_H_

#include <string>

#include "COMMON/EndPoint.h"
#include "ITCM/EmpFullMessage.h"

using namespace std;
using namespace ITCM;

namespace ipc
{

class Pipe : public EndPoint
{
public:
	Pipe();
	virtual ~Pipe();

	/****************************************************************************************************
	 * Function name	: initialize
	 * Description		: This function initializes the file descriptors needed for the read/write operations.
	 * Parameters		: void
	 * Return value		: void
	 ******************************************************************************************************/
	void initialize(string strName = "");

	/****************************************************************************************************
	 * Function name	: readData
	 * Description		: This function is used read the written data from the pipe
	 * Parameters		: void
	 * Return value		: string - data read from the pipe
	 ******************************************************************************************************/
	EmpFullMessage* readData();

	/****************************************************************************************************
	 * Function name	: writeData
	 * Description		: This function is used write the data to pipe
	 * Parameters		: string - data to write to the pipe
	 * Return value		: void
	 ******************************************************************************************************/
	void writeData(EmpFullMessage *message);

	/****************************************************************************************************
	 * Function name	: destroy
	 * Description		: This function is used to deallocate the resources used in the pipe
	 * Parameters		: void
	 * Return value		: void
	 ******************************************************************************************************/
	void destroy();

	/****************************************************************************************************
	 * Function name 	: getCurrentState
	 * Description		: This function is used to to get the current state of the object, true or false. Every thing is OK or not
	 * Parameters		: void
	 * Return value		: bool
 	 ****************************************************************************************************/
	bool getCurrentState();
private:

	/****************************************************************************************
	 * Variable name	: m_fds
	 * Type				: int
	 * Description  	: This is an integer array of file descriptors which are used while initializing the pipe
	 *****************************************************************************************/
	int m_fds[2];

	/****************************************************************************************
	 * Variable name	: m_readFD
	 * Type				: int
	 * Description  	: This is file descriptor to perform read operation on pipe
	 *****************************************************************************************/
	int m_readFD;

	/****************************************************************************************
	 * Variable name	: m_writeFD
	 * Type				: int
	 * Description  	: This is file descriptor to perform write operation on pipe
	 *****************************************************************************************/
	int m_writeFD;

	size_t gTotalBytesRead;
	size_t gBytesAllocated;
	size_t gBufferSize;

	ssize_t readFromFD(int fd, boost::uint8_t *buffer, size_t size);
};

}
#endif /* PIPE_H_ */
