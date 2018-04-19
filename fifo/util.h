#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#define log_debug(fmt, args...)	\
	fprintf(stderr, "%s(%d): " fmt "\n", __FILE__, __LINE__, ##args)

class FIFO
{
public:
	FIFO(const std::string &path);
	~FIFO();
	int read(char *buf, int len);
	int write(char *buf, int len);
private:
	int fd;
	std::string path;
	int open(mode_t mode);
};

FIFO::FIFO(const std::string &path){
	signal(SIGPIPE, SIG_IGN);
	fd = -1;
	this->path = path;
}

FIFO::~FIFO(){
	if(fd >= 0){
		close(fd);
		fd = -1;
	}
}

int FIFO::open(mode_t mode){
	const char *path = this->path.c_str();
	if(mkfifo(path, mode) == -1 && errno != EEXIST){
		return -1;
	}
	if(mode & O_WRONLY){
		if(chmod(path, 0666) == -1){
			return -1;
		}
	}

	fd = ::open(path, O_RDWR | O_NONBLOCK);
	if(fd == -1){
		if(errno == ENXIO){
			return 0;
		}
		return -1;
	}

	return 1;
}

int FIFO::read(char *buf, int len){
	if(fd < 0){
		this->open(O_RDONLY);
	}
	if(fd < 0){
		return 0;
	}
	while(1){
		int ret = ::read(fd, buf, len);
		if(ret == -1){
			if(errno == EAGAIN){
				return 0;
			}
			return -1;
		}
		return ret;
	}
}

int FIFO::write(char *buf, int len){
	if(fd < 0){
		this->open(O_WRONLY);
	}
	if(fd < 0){
		return 0;
	}

	while(1){
		int ret = ::write(fd, buf, len);
		if(ret == -1){
			if(errno == EINTR){
				continue;
			}
			if(errno == EAGAIN){
				return 0;
			}
			return -1;
		}
		return ret;
	}
}

