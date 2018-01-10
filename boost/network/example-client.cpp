#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
        try
        {
                boost::asio::io_service io_service;
                tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 3200);

                tcp::socket socket(io_service);
                socket.connect(end_point);

                for (;;)
                {
                        char buf[128];
                        boost::system::error_code error;

			sprintf(buf,"hello world!");
			socket.write_some(boost::asio::buffer(buf,strlen(buf)+1), error);			

                        size_t len = socket.read_some(boost::asio::buffer(buf), error);
                        if (error == boost::asio::error::eof)
			{
				std::cout<<"Connection closed cleanly by peer."<<std::endl;
                                break;
			}
                        else if (error)
                                throw boost::system::system_error(error); // Some other error.
		
			buf[len]='\0';
                        std::cout<<buf<<std::endl;
                }
        }
        catch (std::exception& e)
        {
                std::cerr << e.what() << std::endl;
        }

        return 0;
}
