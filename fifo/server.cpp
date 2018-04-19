#include "util.h"

int main(int argc, char **argv){
	FIFO fifo("/tmp/test_fifo");

	int num = 0;
	while(1){
		num ++;
		char buf[1024];
		int len = snprintf(buf, sizeof(buf), "hello world - %d\n", num);
		int ret = fifo.write(buf, len);
		if(ret == -1){
			log_debug("error");
			exit(0);
		}else if(ret == 0){
			log_debug("fifo not ready");
			sleep(1);
			continue;
		}
		
		log_debug("write %d byte(s) %d", ret, num);
		usleep(5 * 1000);
	}

	return 0;
}

