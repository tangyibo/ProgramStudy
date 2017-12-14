#include <stdio.h>

int main(int argc,char *argv[])
{
	union {int i;unsigned char ch[2];} var;
	var.i=0x1122;
	printf("x0=%d,x1=%d\n",var.ch[0],var.ch[1]);

	return 0;
}
