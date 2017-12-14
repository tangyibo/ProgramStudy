#include <stdio.h>


int main(int argc,char *argv[])
{
	struct s1 {
		char ch;
		int i;
	};

	struct s2 {
		char a;
		double b;
		int c;
		char d;
	};

	printf("sizeof(struct s1)=%d\n",sizeof(struct s1));
	printf("sizeof(struct s2)=%d\n",sizeof(struct s2));
	
	return 0;
}
