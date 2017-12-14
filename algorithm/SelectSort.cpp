/**
 * 选择排序:从左到右依次选择最小的元素进行对比交换
 * 时间复杂度:最坏情况的时间复杂度是O(n^２)
 */
#include<stdio.h>

int main(int argc,char *argv[])
{
	int array[10]={15,225,34,42,52,6,7856,856,954,10};	
	for(int i=0;i<10;++i)
	{
		for(int j=i+1;j<10;j++)
		{
			if(array[i]>array[j])
			{
				int temp=array[i];
				array[i]=array[j];
				array[j]=temp;
			}
		}
	}

	for( int i=0;i<10;++i)
		printf("%d ", array[i]);

	return 0;
}
