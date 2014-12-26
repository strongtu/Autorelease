#include "stdafx.h"
#include "classinfo.h"  
      
GClassInfo::ClassTable* GClassInfo::_classTable = NULL;  

GClassInfo::GClassInfo(const char* className, const GClassInfo* baseInfo, int size, FNGObjectCreateCallback fun)  
: _className(className)  
, _baseInfo(baseInfo)  
, _classSize(size)  
, _createFun(fun)  
{  
    registerClass();  
}  
      
GClassInfo::~GClassInfo()  
{  
    unregisterClass();  
}  
      
GObject* GClassInfo::createObject(const char* name)  
{  
    ClassTable::iterator pos = _classTable->find(name);  
    if (pos == _classTable->end())  
    {  
        throw("Not found the class of this name in rtti table");  
    }  
    return pos->second->createObject();  
}  

GObject* GClassInfo::createObject()  
{  
    return _createFun ? (*_createFun)() : NULL;  
}  

void GClassInfo::registerClass()  
{  
	if (NULL == _classTable)
	{
		_classTable = new ClassTable;
	}
		
	ClassTable::iterator pos = _classTable->find(_className);  
    if (pos != _classTable->end())  
    {  
        throw("The class of this name is already in rtti table");  
    }  
    (*_classTable)[_className] = this;  
}  
  
void GClassInfo::unregisterClass()  
{  
	if (NULL != _classTable)
	{
		_classTable->erase(_classTable->find(_className)); 
		if (_classTable->size() == 0)
		{
			delete _classTable;
			_classTable = NULL;
		}
	}     
}  
  
