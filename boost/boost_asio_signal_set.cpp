//compile: g++ boost_asio_signal_set.cpp -I/home/tangyibo/local/boost/include -L/home/tangyibo/local/boost/lib -lboost_thread -lboost_system
//boost: verstion > 1.54.0
//runtime: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/tangyibo/local/boost/lib/
//
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio/signal_set.hpp>
#include <execinfo.h>

using namespace boost;

void signal_dump(int signo)
{
	void *array[256];
	size_t size;
	char **strings;

	std::cout <<"========================" << std::endl;
	size = backtrace(array, 256);
	strings = backtrace_symbols(array, size);
	std::cout<<"Obtained [" << size << "] stack frames, dump from " << signo <<std::endl;
	for (size_t i = 0; i < size; i++)
	{
		std::cout <<strings[i] << std::endl;
	}

	free(strings);
}

void signal_handler(const boost::system::error_code& err, int signo)
{
	switch (signo) 
	{
		case SIGINT:
			std::cout << "SIGNINT" << std::endl;
			exit(0);
		case SIGTERM:
			std::cout << "SIGNTERM" << std::endl;
			exit(0);
		case SIGHUP:
			std::cout << "SIGHUP" << std::endl;
			exit(0);	
		case SIGQUIT:
			std::cout << "SIGQUIT" << std::endl;
			exit(0);
		case SIGABRT:
			std::cout << "SIGABRT" << std::endl;
			break;
		case SIGSEGV:
			std::cout << "SIGSEGV" << std::endl;
			break;
		case SIGFPE:
			std::cout << "SIGFPE" << std::endl;
			break;
		case SIGPIPE:
			std::cout << "SIGPIPE" << std::endl;
			break;
		default:
			break;
	}
}

int main(int argc, const char * argv[]) {

	asio::io_service service;

	asio::signal_set sigset(service);
	sigset.add(SIGINT);
	sigset.add(SIGTERM);
	sigset.add(SIGHUP);
	sigset.add(SIGQUIT);
	sigset.add(SIGABRT);
	sigset.add(SIGSEGV);
	sigset.add(SIGFPE);
	sigset.add(SIGPIPE);	

	sigset.async_wait(signal_handler);

	boost::system::error_code ec;
	service.run(ec);
	if(ec)
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
	}
	std::cout << "End" << std::endl;

}

