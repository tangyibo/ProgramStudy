#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>

using namespace boost::asio;

void handle_client(boost::shared_ptr<ip::tcp::socket> sock)
{
	char buf[1024];
	boost::system::error_code ec;
	std::size_t ret;

	std::cout << "client:" << sock->remote_endpoint().address() << std::endl;

	while(1)
	{
		ret=sock->read_some(buffer(buf,sizeof(buf)),ec);
		if(!ec)
		{
			std::cout<<"Recv data[bytes:"<<ret<<"]:"<<buf<<std::endl;
		}
		else
		{
			break;
		}

		ret=sock->write_some(buffer(buf,strlen(buf)+1),ec);
		if(ec)
		{
			std::cout<<"Send data failed:"<<ec.message()<<std::endl;
		}
	}
}

int main(int argc,char *argv[])
{
	std::vector< boost::shared_ptr<boost::thread> > clients;
	try
	{
		io_service io;
		short port=3200;
		ip::tcp::endpoint ep(ip::tcp::v4(), port);
		ip::tcp::acceptor acceptor(io, ep);

		std::cout<<"Server listen on "<<acceptor.local_endpoint().address()<<":"<<port<<std::endl;

		while (1)
		{
			//boost::shared_ptr<ip::tcp::socket> client=boost::make_shared<ip::tcp::socket>(io);//failed to compile!!
			boost::shared_ptr<ip::tcp::socket> client(new ip::tcp::socket(io));
			acceptor.accept(*client);

			boost::shared_ptr<boost::thread> t=boost::make_shared<boost::thread> (boost::bind(handle_client,client));
			clients.push_back(t);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
