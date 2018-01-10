#include "TestSample.h"
#include "TestSampleImpl.h"
#include<boost/make_shared.hpp>

TestSample::TestSample()
{
	m_ptrImpl=boost::make_shared<TestSampleImpl>();
}

TestSample::~TestSample()
{

}

void TestSample::Display()
{
	m_ptrImpl->Display();

}
