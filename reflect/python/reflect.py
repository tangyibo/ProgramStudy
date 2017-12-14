# -*-encoding:utf-8-*-

class Cat:
	print 'this is a cat'
	def __init__(self,name):
		self._name=name;
		print 'Construct function'
	
	def say(self):
		print 'I\'m a cat ,my name is %s'%self._name	

if __name__=='__main__':
	cat=eval('Cat("Jack")')
	print 'object class name is :%s'%cat.__class__  
	print 'Class Cat propery:'
	print dir(cat)
	
	if hasattr(cat,'_name'):
		print 'update name to lucy'
		setattr(cat,'_name','lucy')

	cat.say()

	c=Cat('twin')	
