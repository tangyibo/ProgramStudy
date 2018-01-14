#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
{
	public:
		session(boost::asio::io_service& io_service)
			: socket_(io_service)
		{
		}

		tcp::socket& socket()
		{
			return socket_;
		}

		void start()
		{
			socket_.async_read_some(boost::asio::buffer(data_, max_length),
					boost::bind(&session::handle_read, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
		}

		void handle_read(const boost::system::error_code& error,
				size_t bytes_transferred)
		{
			if (!error)
			{
				std::cout<<"Read data [ bytes:"<<bytes_transferred<<"]: "<<data_<<std::endl;

				boost::asio::async_write(socket_,
						boost::asio::buffer(data_, bytes_transferred),
						boost::bind(&session::handle_write, this,
							boost::asio::placeholders::error));
			}
			else
			{
				delete this;
			}
		}

		void handle_write(const boost::system::error_code& error)
		{
			if (!error)
			{
				socket_.async_read_some(boost::asio::buffer(data_, max_length),
						boost::bind(&session::handle_read, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
			}
			else
			{
				delete this;
			}
		}

	private:
		tcp::socket socket_;
		enum { max_length = 1024 };
		char data_[max_length];
};

class server
{
	public:
		server(boost::asio::io_service& io_service, short port)
			: io_service_(io_service),
			acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
	{
		std::cout<<"Server listen on "<<acceptor_.local_endpoint().address()<<":"<<port<<std::endl;

		session* new_session = new session(io_service_);
		acceptor_.async_accept(new_session->socket(),
				boost::bind(&server::handle_accept, this, new_session,
					boost::asio::placeholders::error));
	}

		void handle_accept(session* new_session,const boost::system::error_code& error)
		{
			if (!error)
			{
				new_session->start();
				new_session = new session(io_service_);
				acceptor_.async_accept(new_session->socket(),
						boost::bind(&server::handle_accept, this, new_session,
							boost::asio::placeholders::error));
			}
			else
			{
				delete new_session;
			}
		}

	private:
		boost::asio::io_service& io_service_;
		tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
	try
	{
		int port=3200;//atoi(argv[1]);
		boost::asio::io_service io_service;

		using namespace std; // For atoi.
		server s(io_service, port);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}