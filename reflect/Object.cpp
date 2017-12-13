#include "Object.h"
#include <map>
#include <iostream>

static std::map<std::string,ClassContext*> *classMap=NULL;

//////////////////////////////////////////////

Object::Object()
{
}
Object::~Object()
{
}

void Object::RegisterClass(ClassContext *cls)
{
	if(NULL==classMap)
	{
		classMap=new std::map<std::string,ClassContext*>();
	}

	if(classMap->find(cls->GetClassName())==classMap->end())
	{
		//std::cout<<"Register Class :"<<cls->GetClassName()<<std::endl;
		classMap->insert(std::pair<std::string,ClassContext*>(cls->GetClassName(),cls));
	}
}

Object* Object::CreateInstance(std::string class_name)
{
	std::map<std::string,ClassContext*>::const_iterator it;
	it=classMap->find(class_name);
	if(it!=classMap->end())
		return it->second->CreateObject();

	return NULL;
}

//////////////////////////////////////////////////////////////

ClassContext::ClassContext(const std::string className,const CreateObjectFun f)
:m_className(className),m_objCreateFun(f)
{
	Object::RegisterClass(this);
}

ClassContext::~ClassContext()
{
	m_className.clear();
	m_objCreateFun=NULL;
}

Object* ClassContext::CreateObject() const
{
	if(m_objCreateFun)
		return m_objCreateFun();

	return NULL;
}

const std::string ClassContext::GetClassName()
{
	return m_className;
}

