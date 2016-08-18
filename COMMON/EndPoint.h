#ifndef EndPointTECHNIQUE_H_
#define EndPointTECHNIQUE_H_

#include <string>
#include <boost/cstdint.hpp>
#include "DYN_CONFIG/include/ConfigurationData.h"
#include "ITCM/EmpFullMessage.h"

using namespace std;
using namespace ITCM;

namespace ipc
{

struct emp_message_part_before_length
{
	boost::uint8_t protocolVersion;
	boost::uint16_t messageType;
	boost::uint8_t messageVersion;
	boost::uint8_t flags;
};

struct emp_message_part_after_length
{
	boost::uint32_t messageNumber;
	boost::uint32_t messageTime;
};

#define MAX_EMP_MESSAGE_LENGTH 1 * 1024 * 1024 //1 MB

class EndPoint
{

public:

	enum ProtocolType
		{
			TCP,
			UDP
		};
		enum SocketType
		{
			CLIENT,
			SERVER
		};
	virtual ~EndPoint();

	// read the configuration items
	static void initConfigItems(string szFileName);

	// initialize the Logging mechanism
	virtual bool InitializeLogMechanism();

	// initialize the object
	virtual void initialize(string strName)                 = 0;

	// functions used to read and write the data into/from the buffer
	virtual EmpFullMessage* readData()               = 0;

	virtual void writeData(EmpFullMessage *data)     = 0;

	// function used to clear the data or shutdown the mechanism
	virtual void destroy()              = 0;

	// functions to get the current state of the object, true or false. Every thing is OK or not
	virtual bool getCurrentState() = 0;

	static ConfigurationData *objConfigurationData;
};
}
#endif
