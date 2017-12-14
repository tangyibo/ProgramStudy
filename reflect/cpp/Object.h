#ifndef _CLASS_OBJECT_H_
#define _CLASS_OBJECT_H_
#include <string>

class ClassContext;
class Object
{
public:
	Object();
	virtual ~Object();

	static void RegisterClass(ClassContext *cls);

	static Object* CreateInstance(std::string class_name);
};


typedef Object* (*CreateObjectFun)(void);

class ClassContext
{
public:
	ClassContext(const std::string className,const CreateObjectFun f);
	~ClassContext();

	Object* CreateObject() const;

	const std::string GetClassName();

private:
	std::string m_className;
	CreateObjectFun m_objCreateFun;
};

#define DECLARE_DYNCREATE(class_name)\
	protected:	\
	static ClassContext m_classContext##class_name; \
	public:	\
	virtual ClassContext* GetClassContext() const; \
	static Object* CreateObject(); 


#define IMPLEMENT_DYNCREATE(class_name)  \
	ClassContext class_name::m_classContext##class_name(#class_name,class_name::CreateObject);	\
	ClassContext* class_name::GetClassContext() const { \
		return &class_name::m_classContext##class_name;	\
	}	\
	Object* class_name::CreateObject(){ \
		return  new class_name(); \
	}


#endif
