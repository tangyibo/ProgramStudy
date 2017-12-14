# -*-encoding:utf-8-*-

def BinarySearch(array,key):
	low=0
	high=len(array)-1

	while low<=high :
		mid=(low+high)/2
		if key<array[mid] :
			high=mid-1
		elif array[mid]<key :
			low=mid+1
		else:
			return mid
	
	return -1

if __name__=='__main__':
	ls=[1,5,3,52,6,7,4,33,6,64]
	ls.sort()
	for i in range(0,len(ls)):
		print '%d=>%d'%(i,ls[i])

	key=7
	idx=BinarySearch(ls,key)
	print 'Search value %d \'s index is %d'%(key,idx)
