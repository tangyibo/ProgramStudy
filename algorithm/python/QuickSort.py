# -*-encoding:utf-8-*-

def QuickSort(array,low,high):
	left=low
	right=high

	if left < right :
		key=array[left]
		
		while left<right :
		
			while array[right]>key and right>left :
				right-=1
			array[left]=array[right]

			while array[left]<key and left<right :
				left+=1
			array[right]=array[left]
		
		array[left]=key;
		QuickSort(array,low,left-1)
		QuickSort(array,right+1,high)


if __name__=='__main__':
	
	ls=[15,225,34,42,52,6,7856,856,954,10]
	print 'Before sort:'
	print ls
	QuickSort(ls,0,len(ls)-1)
	print 'After sort:'
	print ls
	
