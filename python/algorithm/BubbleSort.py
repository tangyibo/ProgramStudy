# -*-encoding:utf-8-*-

def BubbleSort(array):
	length=len(array)
	for i in range(0,length,1):
		for j in range(length-1,i,-1):
			if array[j]<array[j-1] :
				temp=array[j];
				array[j]=array[j-1];
				array[j-1]=temp;	



if __name__=='__main__':
	
	ls=[15,225,34,42,52,6,7856,856,954,10]
	print 'Before sort:'
	print ls
	BubbleSort(ls)
	print 'After sort:'
	print ls
	
