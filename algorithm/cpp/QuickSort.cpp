/**
 * 快速排序：
 * 1.在待排序的元素任取一个元素作为基准(通常选第一个元素，但最的选择方法是从待排序元素中随机选取一个作为基准)，称为基准元素；
 * 2.将待排序的元素进行分区，比基准元素大的元素放在它的右边，比其小的放在它的左边；
 * 3.对左右两个分区重复以上步骤直到所有元素都是有序的。
 * 时间复杂度:冒泡排序最坏情况的时间复杂度是O(nlgn)
 */
#include<stdio.h>

void QuickSort(int *array,int low,int high)
{
	int left=low;
	int right=high;
	if(left<right)
	{
		int key=array[left];
		while(left<right)
		{
			while(array[right]>key && right>left)
				right--;

			array[left]=array[right];

			while(array[left]<key && left<right)
				left++;

			array[right]=array[left];			
		}	
	

		array[left]=key;

		QuickSort(array,low,left-1);
		QuickSort(array,right+1,high);
	}
}

int main(int argc,char *argv[])
{
	int array[10]={15,225,34,42,52,6,7856,856,954,10};	
	QuickSort(array,0,10-1);
	for( int i=0;i<10;++i)
		printf("%d ", array[i]);

	return 0;
}
