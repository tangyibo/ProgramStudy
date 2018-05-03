//compile:g++ boost_unit_test.cpp -lboost_unit_test_framework
//
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>

BOOST_AUTO_TEST_CASE(test_func)
{
	int i=0;
	int j=2-2;

	BOOST_CHECK_EQUAL(i,j);

	std::string str("hello");
	BOOST_CHECK_EQUAL(str.size(),5);
}

