# -*-encoding:utf-8-*-

def SelectedSort(array):
	length=len(array)
	for i in range(0,length,1):
		for j in range(i+1,length,1):
			if array[j]<array[i] :
				temp=array[j];
				array[j]=array[i];
				array[i]=temp;	



if __name__=='__main__':
	
	ls=[15,225,34,42,52,6,7856,856,954,10]
	print 'Before sort:'
	print ls
	SelectedSort(ls)
	print 'After sort:'
	print ls
	
