/*
 * Fifo.h
 * Created on: 19-Jul-2016
 * Author: Raju Anabathula
 * Purpose: This class implements the Unix named pipe (FIFO) mechanism by exploiting the default system calls.
 */

#ifndef INCLUDE_FIFO_H_
#define INCLUDE_FIFO_H_

#include <ITCM/Configuration/ConfigConstants.h>
#include "COMMON/EndPoint.h"

namespace ipc
{

class Fifo: public EndPoint
{
public:
	Fifo();
	virtual ~Fifo();

	/****************************************************************************************************
	 * Function name	: initialize
	 * Description		: This function initializes the file name needed for the read/write operations.
	 * Parameters		: string - the name of the FIFO
	 * Return value		: void
	 ******************************************************************************************************/
	void initialize(string strName);

	/****************************************************************************************************
	 * Function name	: readData
	 * Description		: This function is used read the written data from the FIFO
	 * Parameters		: void
	 * Return value		: EmpFullMessage - data read from the FIFO
	 ******************************************************************************************************/
	EmpFullMessage* readData();

	/****************************************************************************************************
	 * Function name	: writeData
	 * Description		: This function is used write the data to FIFO
	 * Parameters		: EmpFullMessage - data to write to the FIFO
	 * Return value		: void
	 ******************************************************************************************************/
	void writeData(EmpFullMessage *message);

	/****************************************************************************************************
	 * Function name	: destroy
	 * Description		: This function is used to deallocate the resources used in the FIFO
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
	 * Variable name	: m_strFileName
	 * Type				: string
	 * Description  	: This is the named pipe file name which is used while initializing the named pipe
	 *****************************************************************************************/
	string m_strFifoName;

	/****************************************************************************************
	 * Variable name	: m_RWFD
	 * Type				: int
	 * Description  	: This is file descriptor to perform read/write operations on named pipe
	 *****************************************************************************************/
	int m_RWFD;

	size_t m_totalBytesRead;
	size_t m_bytesAllocated;
	size_t m_bufferSize;

	ssize_t readFromFD(int fd, boost::uint8_t *buffer, size_t size);
};

}
#endif /* INCLUDE_FIFO_H_ */
