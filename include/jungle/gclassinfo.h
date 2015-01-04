#ifndef __GCLASS_INFO__H__  
#define __GCLASS_INFO__H__  
      
#include <map>  
#include <string>  
      
#include "gtype.h"

class GObject;  
      
typedef GObject* (*FNGObjectCreateCallback)(void);  
  
class GAPI GClassInfo  
{  
public:
    GClassInfo(const char* className, const GClassInfo* baseInfo, int size, FNGObjectCreateCallback fun);
    ~GClassInfo();
 
    static GObject* createObject(const char* name);  

    GObject* createObject();

    inline bool isDynamic() const   
    { 
        return !_createFun;
    }  
 
    bool isKindOf(const GClassInfo *info) const  
    {  
        return info && ( info == this || ( _baseInfo && _baseInfo->isKindOf(info) ) );  
    }  

    inline const char* getClassName() const  
    {
        return _className;
    }

    inline const char* getBaseClassName() const  
    {
        return _baseInfo ? _baseInfo->getClassName() : NULL;
    }  
  
    inline const GClassInfo* getBaseClass() const   
    {
        return _baseInfo;
    }  

    inline int getClassSize() const  
    {
        return _classSize;
    }

protected:   
    void registerClass();
    void unregisterClass();  
      
private:  
    typedef std::map<std::string, GClassInfo*>  ClassTable;  
      
    const char*                 _className;         // 类名  
    int                         _classSize;         // 类大小  
    const GClassInfo*           _baseInfo;          // 父类信息  
    FNGObjectCreateCallback     _createFun;         // 类构造函数指针

    static ClassTable*          _classTable;        // 类映射表  
};  

#define DECLARE_GDYNAMIC_CLASS(name)             \
public:                                         \
	static  GClassInfo  _classInfo;             \
    virtual GClassInfo* getClassInfo() const;   \
    static  GObject* createObject();                 
       
#define IMPLEMENT_GDYNAMIC_CLASS(name, basename) \
GClassInfo name::_classInfo(#name,  &basename::_classInfo, (int)sizeof(name), name::createObject); \
GClassInfo* name::getClassInfo() const  \
{                                       \
    return &name::_classInfo;           \
}                                       \
GObject* name::createObject()           \
{                                       \
    return new name;                    \
}
      
#endif