#ifndef ITCM_TEST_CLASS_FRAMEWORK_H
#define ITCM_TEST_CLASS_FRAMEWORK_H

#include <gtest/gtest.h>

#include <sstream>
#include <iostream>

namespace ITCM
{

class TestClassFramework : public ::testing::Test
{
public:
	virtual ~TestClassFramework();

protected:
	virtual void SetUp();
	virtual void TearDown();

private:
	std::stringstream m_oss;
	std::streambuf* m_pSbuf;
	std::streambuf* m_pBackup;
};

}

#endif /* ITCM_TEST_CLASS_FRAMEWORK_H */
