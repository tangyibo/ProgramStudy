#include "logger.h"
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

namespace logging = boost::log;
using namespace logging::trivial;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

class test
{
	public:
		test(): scl(keywords::channel = "test_class")
	{
		BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ << " success log output";
	}

		src::severity_channel_logger<severity_level, std::string> scl;
};

int main()
{
	if(!init_log_environment("./log.config"))
		return -1;

	test t;		

	while(1)
	{
		BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
		BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
		BOOST_LOG_TRIVIAL(info) << "An informational severity message";
		BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
		BOOST_LOG_TRIVIAL(error) << "An error severity message";
		BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

		sleep(1);
	}

	return 0;
}

