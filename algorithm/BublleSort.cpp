/**
 * 冒泡排序:最重要的思想是两两比较，将两者较少的升上去
 * 时间复杂度:冒泡排序最坏情况的时间复杂度是O(n²)
 */
#include<stdio.h>

int main(int argc,char *argv[])
{
	int array[10]={15,225,34,42,52,6,7856,856,954,10};	
	for(int i=0;i<10;++i)
	{
		for(int j=10-1;j>i;j--)
		{
			//相邻的两个元素比较,较小的元素上升一位
			if(array[j]<array[j-1])
			{
				int temp=array[j];
				array[j]=array[j-1];
				array[j-1]=temp;
			}
		}
	}

	for( int i=0;i<10;++i)
		printf("%d ", array[i]);

	return 0;
}
