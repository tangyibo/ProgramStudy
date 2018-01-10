#ifndef _TEST_H_
#define _TEST_H_
#include<boost/smart_ptr.hpp>

class TestSample
{
private:
	class TestSampleImpl;
	boost::shared_ptr<TestSampleImpl> m_ptrImpl;

public:
	TestSample();
	~TestSample();

	void Display();	
};

#endif
