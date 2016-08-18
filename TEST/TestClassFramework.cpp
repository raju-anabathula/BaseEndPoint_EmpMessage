#include "TestClassFramework.h"

namespace ITCM
{

TestClassFramework::~TestClassFramework()
{

}

void TestClassFramework::SetUp()
{
	m_pBackup = std::cerr.rdbuf();
	m_pSbuf = m_oss.rdbuf();
	std::cerr.rdbuf(m_pSbuf);
}

void TestClassFramework::TearDown()
{
	std::cerr.rdbuf(m_pBackup);
}

}
