#include "EndPointFactory.h"

#include "IPC_PIPE/Pipe.h"
#include "IPC_FIFO/Fifo.h"
//#include "IPC_SM/SharedMemory.h"

//#include "IPC_POSIX_MQ/PosixMessageQueue.h"
//#include "IPC_FL/FileLocking.h"

#include "IPC_UDS/UnixDomainSocket.h"
//#include "TCP_IP/TcpUdpSocket.h"

using namespace ipc;

EndPointFactory *EndPointFactory::m_EndPointFactoryInstance = NULL;

EndPointFactory::EndPointFactory()
{
}

EndPointFactory::~EndPointFactory()
{
}

EndPointFactory* EndPointFactory::getEndPointFactory()
{
	if(m_EndPointFactoryInstance == 0)
		m_EndPointFactoryInstance = new EndPointFactory();
	return m_EndPointFactoryInstance;
}

EndPoint* EndPointFactory::createEndPointInstance(EndPointFactory::EndPointType endPointType)
{
	switch (endPointType)
	{

	case IPC_PIPE:
		return new Pipe();
		break;
	case IPC_FIFO:
		return new Fifo();
		break;
		/*case IPC_FL:
		return new FileLocking();
		break;
	case TCP_IP:
		//			return new TcpIp();
		break;
	case EMP_CLASSD:
		//			return new ClassD();
		break;
	case IPC_SM:
		return new SharedMemory();
		break;
	case IPC_MQ:
		return new PosixMessageQueue();
		break;*/
	}
	return 0;
}

SocketEndPoint* EndPointFactory::createSocketEndPointInstance(EndPointFactory::EndPointType endPointType, EndPoint::ProtocolType protocolType)
{
	switch(endPointType)
	{
	case IPC_UDS:
		return new UnixDomainSocket(protocolType);
		break;
	/*case TCP_IP:
			return new TcpUdpSocket(protocolType);
			break;*/
	}
	return 0;
}
