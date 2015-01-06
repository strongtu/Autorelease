#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>

class EmptyType {};

typedef EmptyType Delegater;
typedef void (Delegater::*OnEventFunc)();

template <typename EventFuncType>
class EventObject
{
public:
    EventObject() : ObjPtr(nullptr), FuncPtr(nullptr) {}
    template <typename ObjectType, typename MemberFuncType>
    EventObject(ObjectType* pObj, MemberFuncType pMemberFunc)
    {
        ObjPtr = (EmptyType*)pObj;
        FuncPtr = *((EventFuncType*)&pMemberFunc);
    }
public:
    EmptyType* ObjPtr;
    EventFuncType FuncPtr;
};

template <typename EventFuncType>
class EventSource
{
public:
    friend class Object;
public:
    typedef std::vector<EventObject<EventFuncType>> EventList;
    typedef typename std::vector<EventObject<EventFuncType>>::iterator EventIterator;
public:
    EventSource() : pEventList(nullptr) {}
    virtual ~EventSource()
    {
        if (pEventList) delete pEventList;
    }

    bool IsEmpty()
    {
        return !(pEventList && !pEventList->empty());
    }

    template <typename ObjectType, typename MemberFuncType>
    void Listen(ObjectType* pObj, MemberFuncType pMemberFunc)
    {
        EventObject<EventFuncType> eventObj;
        this->operator += eventObj;
    }

    template <typename ObjectType, typename MemberFuncType>
    void Unlisten(ObjectType* pObj, MemberFuncType pMemberFunc)
    {
        EventObject<EventFuncType> eventObj(pObj, pMemberFunc);
        this->operator -= eventObj;
    }

    void operator +=(EventObject<EventFuncType>& eventObj)
    {
        if (!pEventList) pEventList = new EventList();
        bool bFind = false;
        EventIterator iter;
        for (iter = pEventList->begin(); iter != pEventList->end(); ++iter)
        {
            if (iter->ObjPtr == eventObj.ObjPtr && iter->FuncPtr == eventObj.FuncPtr)
            {
                bFind = true;
                break;
            }
        }
        if (!bFind) pEventList->push_back(eventObj);
    }

    void operator -=(EventObject<EventFuncType>& eventObj)
    {
        if (!pEventList) return;
        EventIterator iter;
        for (iter = pEventList->begin(); iter != pEventList->end(); ++iter)
        {
            if (iter->ObjPtr == eventObj.ObjPtr && iter->FuncPtr == eventObj.FuncPtr)
            {
                pEventList->erase(iter);
                break;
            }
        }
    }
public:
    EventList* pEventList;
};

#define GFireEvent(type, event, params)\
{\
    retain();\
    if (event.pEventList && (!event.pEventList->empty()))\
    {\
        EventSource<type>::EventList eventList = *event.pEventList;\
        EventSource<type>::EventIterator iter;\
        for (iter = eventList.begin(); iter != eventList.end(); ++iter)\
        {\
            ((iter->ObjPtr)->*(iter->FuncPtr))params;\
        }\
    }\
    release();\
}

#endif