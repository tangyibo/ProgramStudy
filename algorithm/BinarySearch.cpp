/**
 *二分搜索：对一组有序的数字中进行查找，不断将数组进行对半分割，每次拿中间元素和要查找的数据进行比较
 *时间复杂度：O(lgN)
 *备注：Bentley在他的著作《Writing Correct Programs》中写道，90%的计算机专家不能在2小时内写出完全正确的二分搜索算法。
 */
#include<algorithm>
#include<stdio.h>

int BinarySearch(int *array,int len,int key)
{
	int low=0;
	int high=len-1;

	while(low<=high)
	{
		int mid=(low+high)/2;
		if(key<array[mid])
		{
			high=mid-1;
		}
		else if(array[mid]<key)
		{
			low=mid+1;
		}
		else
		{
			return mid;
		}
	}

	return -1;
}

int main(int argc,char *argv[])
{
	int array[10]={1,5,3,52,6,7,4,33,6,64};
	std::sort(array,array+10);
	for( int i=0;i<10;++i)
	{
		printf("%d=>%d ",i,array[i]);
	}
	printf("\n");

	int keyvalue=7;
	int index=BinarySearch(array,10,keyvalue);
	printf("Find value %d 's index is %d\n",keyvalue,index);

	return 0;
}
