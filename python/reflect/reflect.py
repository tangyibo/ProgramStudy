# -*-encoding:utf-8-*-

class Cat:

	def __init__(self,name):
		self._name=name;
	
	def say(self):
		print 'I\'m a cat ,my name is %s'%self._name	

if __name__=='__main__':
	cat=Cat('Jack')
	print 'Class Cat propery:'
	print dir(cat)
	
	if hasattr(cat,'_name'):
		print 'update name to lucy'
		setattr(cat,'_name','lucy')

	cat.say()	
