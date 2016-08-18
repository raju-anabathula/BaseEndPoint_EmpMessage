#include <boost/cstdint.hpp>
#include <ITCM/common/PreconditionFailed.h>

#include "TestClassFramework.h"

#include "ITCM/Errors/InvalidMessage.h"

#include "ITCM/EmpFullMessage.h"

using namespace std;

namespace ITCM
{

class EmpFullMessageTests : public TestClassFramework
{
public:
	explicit EmpFullMessageTests() {}
	virtual ~EmpFullMessageTests() {}

	boost::uint8_t GetBits(boost::uint16_t bits, boost::uint8_t bitStart, boost::uint8_t bitEnd);

protected:
	static const boost::uint8_t m_protocolVersion;
	static const boost::uint16_t m_messageType;
	static const boost::uint8_t m_messageVersion;
	static const boost::uint8_t m_flags;
	static const boost::uint32_t m_dataLength;
	static const boost::uint32_t m_messageNumber;
	static const boost::uint32_t m_messageTime;
	static const boost::uint8_t m_varHeaderSize;
	static const boost::uint16_t m_ttl;
	static const boost::uint8_t m_qosClass;
	static const boost::uint8_t m_qosPriority;
	static const boost::uint8_t m_qosNP;
	static const boost::uint8_t m_qosSH;
	static const boost::uint8_t m_qosSvcReq;
	static const boost::uint16_t m_qos;
	static const string m_source;
	static const string m_destination;
	static const char* m_messageBody;
	static const boost::uint32_t m_dataIntegrity;

	static const boost::uint32_t m_totalBytesWithVarHeader;
	static const boost::uint32_t m_totalBytesWithoutVarHeader;

	static const boost::uint8_t m_bytesWithVarHeader[];
	static const boost::uint8_t m_bytesWithoutVarHeader[];
};

const boost::uint8_t EmpFullMessageTests::m_protocolVersion = 2;		// 0x02
const boost::uint16_t EmpFullMessageTests::m_messageType = 1;			// 0x00 0x01
const boost::uint8_t EmpFullMessageTests::m_messageVersion = 3;		// 0x03
const boost::uint8_t EmpFullMessageTests::m_flags = 9;				// 0x09
const boost::uint32_t EmpFullMessageTests::m_dataLength = 4;			// 0x00, 0x00, 0x04
const boost::uint32_t EmpFullMessageTests::m_messageNumber = 19;		// 0x00, 0x00, 0x00, 0x13
const boost::uint32_t EmpFullMessageTests::m_messageTime = 5000;		// 0x00, 0x00, 0x13, 0x88
const boost::uint8_t EmpFullMessageTests::m_varHeaderSize = 32;		// 0x20
const boost::uint16_t EmpFullMessageTests::m_ttl = 99;				// 0x00, 0x63

const boost::uint8_t EmpFullMessageTests::m_qosClass = 3;			// binary: 011
const boost::uint8_t EmpFullMessageTests::m_qosPriority = 2;		// binary: 010
const boost::uint8_t EmpFullMessageTests::m_qosNP = 1;				// binary: 001
const boost::uint8_t EmpFullMessageTests::m_qosSH = 9;				// binary: 1001
const boost::uint8_t EmpFullMessageTests::m_qosSvcReq = 7;			// binary: 111


// 0110 1000 1100 1111
// 0x68, 0xCF
//const boost::uint16_t EmpFullMessageTests::m_qos = 0x68CF;
const boost::uint16_t EmpFullMessageTests::m_qos = 0xF253;

const string EmpFullMessageTests::m_source = "SOURCE.ADDRESS";		// 0x53 4F 55 52 43 45 2E 41 44 44 52 45 53 53 00, size 15
const string EmpFullMessageTests::m_destination = "DEST.ADDRESS";	// 0x44 45 53 54 2E 41 44 44 52 45 53 53 00, size 13

const char* EmpFullMessageTests::m_messageBody = "ABC";				// 0x41 42 43 00

const boost::uint32_t EmpFullMessageTests::m_dataIntegrity = 0;		// 0x00, 0x00, 0x00, 0x00

const boost::uint32_t EmpFullMessageTests::m_totalBytesWithVarHeader = 57;
const boost::uint32_t EmpFullMessageTests::m_totalBytesWithoutVarHeader = 25;

const boost::uint8_t EmpFullMessageTests::m_bytesWithVarHeader[] = { 0x02,
																	0x00, 0x01,
																	0x03,
																	0x09,
																	0x00, 0x00, 0x04,
																	0x00, 0x00, 0x00, 0x13,
																	0x00, 0x00, 0x13, 0x88,
																	0x20,
																	0x00, 0x63,		// ttl
																	0xF2, 0x53,		// qos
																	0x53, 0x4F, 0x55, 0x52, 0x43, 0x45, 0x2E, 0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x00,
																	0x44, 0x45, 0x53, 0x54, 0x2E, 0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x00,
																	0x41, 0x42, 0x43, 0x00,
																	0x00, 0x00, 0x00, 0x00
																};

const boost::uint8_t EmpFullMessageTests::m_bytesWithoutVarHeader[] = { 0x02,
																		0x00, 0x01,
																		0x03,
																		0x09,
																		0x00, 0x00, 0x04,
																		0x00, 0x00, 0x00, 0x13,
																		0x00, 0x00, 0x13, 0x88,
																		0x00,
																		0x41, 0x42, 0x43, 0x00,
																		0x00, 0x00, 0x00, 0x00
																};

boost::uint8_t EmpFullMessageTests::GetBits(boost::uint16_t bits, boost::uint8_t bitStart, boost::uint8_t bitEnd)
{
	boost::uint8_t maskLength = bitEnd - bitStart + 1; // inclusive

	boost::uint8_t mask = 0;
	
	for (boost::uint8_t bit = 0; bit < maskLength; ++bit)
	{
		mask |= (1 << bit);
	}

	bits >>= bitStart;

	return bits & mask;
}

TEST_F(EmpFullMessageTests, BytesToFieldsNoVarHeader)
{
	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithoutVarHeader]);
	memcpy(messageBytes.get(), m_bytesWithoutVarHeader, m_totalBytesWithoutVarHeader);
	EmpFullMessage message(messageBytes.get(), m_totalBytesWithoutVarHeader);

	EXPECT_EQ(static_cast<int>(m_protocolVersion), static_cast<int>(message.GetProtocolVersion()));
	EXPECT_EQ(m_messageType, message.GetMessageType());
	EXPECT_EQ(static_cast<int>(m_messageVersion), static_cast<int>(message.GetMessageVersion()));
	EXPECT_EQ(static_cast<int>(m_flags), static_cast<int>(message.GetFlags()));
	EXPECT_EQ(m_dataLength, message.GetDataLength());
	EXPECT_EQ(m_messageNumber, message.GetMessageNumber());
	EXPECT_EQ(m_messageTime, message.GetMessageTime());
	EXPECT_EQ(0, message.GetVariableHeaderSize());
	EXPECT_STREQ(m_messageBody, reinterpret_cast<const char*>(message.GetBody().get()));
	EXPECT_EQ(m_dataIntegrity, message.GetDataIntegrity());
}

TEST_F(EmpFullMessageTests, FieldsToBytesNoVarHeader)
{
	boost::shared_array<boost::uint8_t> messageBody(new boost::uint8_t[m_dataLength]);
	memcpy(messageBody.get(), m_messageBody, m_dataLength);

	EmpFullMessage message;
	message.SetProtocolVersion(m_protocolVersion);
	message.SetMessageType(m_messageType);
	message.SetMessageVersion(m_messageVersion);
	message.SetFlags(m_flags);
	message.SetMessageNumber(m_messageNumber);
	message.SetMessageTime(m_messageTime);
	message.SetBody(messageBody, m_dataLength);

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message.GetMessageSize()]);
        message.AsBytes(bytes.get(), message.GetMessageSize());

	EXPECT_EQ(m_totalBytesWithoutVarHeader, message.GetMessageSize());

	for (boost::uint32_t i = 0; i < message.GetMessageSize(); ++i)
	{
		EXPECT_EQ(static_cast<int>(m_bytesWithoutVarHeader[i]), static_cast<int>(bytes[i])) << "Mismatch at: " << i;
	}
}

TEST_F(EmpFullMessageTests, BytesToFieldWithVarHeader)
{
	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithVarHeader]);
	memcpy(messageBytes.get(), m_bytesWithVarHeader, m_totalBytesWithVarHeader);
	EmpFullMessage message(messageBytes.get(), m_totalBytesWithVarHeader);

	EXPECT_EQ(static_cast<int>(m_protocolVersion), static_cast<int>(message.GetProtocolVersion()));
	EXPECT_EQ(m_messageType, message.GetMessageType());
	EXPECT_EQ(static_cast<int>(m_messageVersion), static_cast<int>(message.GetMessageVersion()));
	EXPECT_EQ(static_cast<int>(m_flags), static_cast<int>(message.GetFlags()));
	EXPECT_EQ(m_dataLength, message.GetDataLength());
	EXPECT_EQ(m_messageNumber, message.GetMessageNumber());
	EXPECT_EQ(m_messageTime, message.GetMessageTime());
	EXPECT_EQ(m_varHeaderSize, message.GetVariableHeaderSize());
	EXPECT_EQ(m_ttl, message.GetTtl());
	EXPECT_EQ(static_cast<int>(m_qosClass), static_cast<int>(message.GetQoSClass()));
	EXPECT_EQ(static_cast<int>(m_qosPriority), static_cast<int>(message.GetQoSPriority()));
	EXPECT_EQ(static_cast<int>(m_qosNP), static_cast<int>(message.GetQoSNetworkPreference()));
	EXPECT_EQ(static_cast<int>(m_qosSH), static_cast<int>(message.GetQoSSpecialHandling()));
	EXPECT_EQ(static_cast<int>(m_qosSvcReq), static_cast<int>(message.GetQoSServiceRequest()));
	EXPECT_EQ(m_source, message.GetSourceAddress());
	EXPECT_EQ(m_destination, message.GetDestinationAddress());
	EXPECT_STREQ(m_messageBody, reinterpret_cast<const char*>(message.GetBody().get()));
	EXPECT_EQ(m_dataIntegrity, message.GetDataIntegrity());
}

TEST_F(EmpFullMessageTests, FieldsToBytesWithVarHeader)
{
	boost::shared_array<boost::uint8_t> messageBody(new boost::uint8_t[m_dataLength]);
	memcpy(messageBody.get(), m_messageBody, m_dataLength);

	EmpFullMessage message;
	message.SetProtocolVersion(m_protocolVersion);
	message.SetMessageType(m_messageType);
	message.SetMessageVersion(m_messageVersion);
	message.SetFlags(m_flags);
	message.SetMessageNumber(m_messageNumber);
	message.SetMessageTime(m_messageTime);
	message.SetTtl(m_ttl);
	message.SetQoSClass(m_qosClass);
	message.SetQoSPriority(m_qosPriority);
	message.SetQoSNetworkPreference(m_qosNP);
	message.SetQoSSpecialHandling(m_qosSH);
	message.SetQoSServiceRequest(m_qosSvcReq);
	message.SetSourceAddress(m_source);
	message.SetDestinationAddress(m_destination);
	message.SetBody(messageBody, m_dataLength);

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message.GetMessageSize()]);
        message.AsBytes(bytes.get(), message.GetMessageSize());

	EXPECT_EQ(m_totalBytesWithVarHeader, message.GetMessageSize());

	for (boost::uint32_t i = 0; i < message.GetMessageSize(); ++i)
	{
		EXPECT_EQ(static_cast<int>(m_bytesWithVarHeader[i]), static_cast<int>(bytes[i])) << "Mismatch at byte: " << i;
	}
}

TEST_F(EmpFullMessageTests, VarLengthHeaderWithBadLengthValue)
{
	const boost::uint8_t bytesWithBadVarHeader[] = { 0x02,
													0x00, 0x01,
													0x03,
													0x09,
													0x00, 0x00, 0x04,
													0x00, 0x00, 0x00, 0x13,
													0x00, 0x00, 0x13, 0x88,
													0x00,			// varLength, should be 0x20, not 0
													0x00, 0x63,		// ttl
													0xF2, 0x53,		// qos
													0x53, 0x4F, 0x55, 0x52, 0x43, 0x45, 0x2E, 0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x00,
													0x44, 0x45, 0x53, 0x54, 0x2E, 0x41, 0x44, 0x44, 0x52, 0x45, 0x53, 0x53, 0x00,
													0x41, 0x42, 0x43, 0x00,
													0x00, 0x00, 0x00, 0x00
												};

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithVarHeader]);
	memcpy(messageBytes.get(), bytesWithBadVarHeader, m_totalBytesWithVarHeader);

	// This will only throw because too many bytes were specified (i.e. InvalidMessage)
	EXPECT_THROW(EmpFullMessage message(messageBytes.get(), m_totalBytesWithVarHeader), InvalidMessage);
}

TEST_F(EmpFullMessageTests, IncorrectVarLengthForNoHeader)
{
	const boost::uint8_t bytesWithBadVarHeader[] = { 0x02,
													0x00, 0x01,
													0x03,
													0x09,
													0x00, 0x00, 0x04,
													0x00, 0x00, 0x00, 0x13,
													0x00, 0x00, 0x13, 0x88,
													0x20,		// varLength, should be 0
													0x41, 0x42, 0x43, 0x00,
													0x00, 0x00, 0x00, 0x00
													};

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithoutVarHeader]);
	memcpy(messageBytes.get(), bytesWithBadVarHeader, m_totalBytesWithoutVarHeader);

	// This could throw for different reasons, depending on the bytes in the fake ttl, qos, and address strings
	EXPECT_ANY_THROW(EmpFullMessage message(messageBytes.get(), m_totalBytesWithoutVarHeader));
}

TEST_F(EmpFullMessageTests, DataLengthValidity)
{
	static const boost::uint32_t MAX_LENGTH_PLUS_ONE = 16777216;

	boost::shared_array<boost::uint8_t> body(new boost::uint8_t[MAX_LENGTH_PLUS_ONE]);

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetBody(body, MAX_LENGTH_PLUS_ONE - 1));

	EXPECT_THROW(message.SetBody(body, MAX_LENGTH_PLUS_ONE), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, QoSClassValidity)
{
	const boost::uint8_t MAX = 7;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetQoSClass(MAX));

	EXPECT_THROW(message.SetQoSClass(MAX + 1), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, QoSPriorityValidity)
{
	const boost::uint8_t MAX = 7;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetQoSPriority(MAX));

	EXPECT_THROW(message.SetQoSPriority(MAX + 1), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, QoSNetworkPreferenceValidity)
{
	const boost::uint8_t MAX = 7;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetQoSNetworkPreference(MAX));

	EXPECT_THROW(message.SetQoSNetworkPreference(MAX + 1), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, QoSSpecialHandlingValidity)
{
	const boost::uint8_t MAX = 15;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetQoSSpecialHandling(MAX));

	EXPECT_THROW(message.SetQoSSpecialHandling(MAX + 1), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, QoSServiceRequestValidity)
{
	const boost::uint8_t MAX = 7;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetQoSServiceRequest(MAX));

	EXPECT_THROW(message.SetQoSServiceRequest(MAX + 1), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, SourceAddressValidity)
{
	const size_t MAX_LENGTH = 63;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetSourceAddress(string(MAX_LENGTH, 'a')));

	EXPECT_THROW(message.SetSourceAddress(string(MAX_LENGTH + 1, 'a')), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, DestinationAddressValidity)
{
	const size_t MAX_LENGTH = 63;

	EmpFullMessage message;
	EXPECT_NO_THROW(message.SetDestinationAddress(string(MAX_LENGTH, 'a')));

	EXPECT_THROW(message.SetSourceAddress(string(MAX_LENGTH + 1, 'a')), Common::PreconditionFailed);
}

TEST_F(EmpFullMessageTests, AsString)
{
	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithVarHeader]);
	memcpy(messageBytes.get(), m_bytesWithVarHeader, m_totalBytesWithVarHeader);
	EmpFullMessage message(messageBytes.get(), m_totalBytesWithVarHeader);

	string expectedMessage =
			"Emp Message: \n"
				"\tProtocol Version: 2\n"
				"\tMessage Type: 1\n"
				"\tMessage Version: 3\n"
				"\tFlags: 0x9\n"
				"\tData Length: 4\n"
				"\tMessage Number: 19\n"
				"\tMessage Time: 5000\n"
				"\tVariable Header Size: 32\n"
				"\tTTL: 99\n"
				"\tQoS Class: 3\n"
				"\tQoS Priority: 2\n"
				"\tQoS Network Preference: 1\n"
				"\tQoS Special Handling: 9\n"
				"\tQoS Service Request: 7\n"
				"\tSource Address: SOURCE.ADDRESS\n"
				"\tDestination Address: DEST.ADDRESS\n"
				"\tData Integrity: 0";

	EXPECT_EQ(expectedMessage, message.AsString());
}

TEST_F(EmpFullMessageTests, QoSBits)
{
	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[m_totalBytesWithVarHeader]);
	memcpy(messageBytes.get(), m_bytesWithVarHeader, m_totalBytesWithVarHeader);
	EmpFullMessage message(messageBytes.get(), m_totalBytesWithVarHeader);

	boost::uint16_t qosBits = message.GetQoSBits();

	EXPECT_EQ(static_cast<int>(m_qosSvcReq), static_cast<int>(GetBits(qosBits, 13, 15)));
	EXPECT_EQ(static_cast<int>(m_qosSH), static_cast<int>(GetBits(qosBits, 9, 12)));
	EXPECT_EQ(static_cast<int>(m_qosNP), static_cast<int>(GetBits(qosBits, 6, 8)));
	EXPECT_EQ(static_cast<int>(m_qosPriority), static_cast<int>(GetBits(qosBits, 3, 5)));
	EXPECT_EQ(static_cast<int>(m_qosClass), static_cast<int>(GetBits(qosBits, 0, 2)));
}

}
