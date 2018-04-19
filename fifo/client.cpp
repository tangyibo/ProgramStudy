#include "util.h"

int main(int argc, char **argv){
	FIFO fifo("/tmp/test_fifo");

	while(1){
		char buf[32];
		int ret = fifo.read(buf, sizeof(buf));
		if(ret == -1){
			exit(0);
		}else if(ret == 0){
			log_debug("fifo empty");
			sleep(1);
			continue;
		}
		write(STDERR_FILENO, buf, ret);
		sleep(1);
	}


	return 0;
}

