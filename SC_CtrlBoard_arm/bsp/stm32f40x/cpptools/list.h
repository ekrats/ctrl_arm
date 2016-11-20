/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    list.h
 * @brief   This file contains definition of class list
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/10    Coase        the first version
 */


#ifndef __CPP__LIST__H__
#define __CPP__LIST__H__
#include "link.h"

template<typename T>
class List
{
public:
    List();
    ~List();
    //
    // 摘要:
    //     获取 List<T> 中实际包含的元素数。
    //
    // 返回结果:
    //     List<T> 中实际包含的元素数。
    int Count();
    // 摘要:
    //     将对象添加到 List<T> 的结尾处。
    //
    // 参数:
    //   item:
    //     要添加到 List<T> 的末尾处的对象。对于引用类型，该值可以为 null。
    void Add(const T&);
    //
    // 摘要:
    //     从 List<T> 中移除所有元素。
    void Begin();
    void Clear();
    //
    // 摘要:
    //     确定某元素是否在 List<T> 中。
    //
    // 参数:
    //   item:
    //     要在 List<T> 中定位的对象。对于引用类型，该值可以为 null。
    //
    // 返回结果:
    //     如果在 List<T> 中找到 item，则为 true，否则为 false。
    bool Contains(const T&);
    //
    // 摘要:
    //     将元素插入 List<T> 的指定索引处。
    //
    // 参数:
    //   index:
    //     从零开始的索引，应在该位置插入 item。
    //
    //   item:
    //     要插入的对象。对于引用类型，该值可以为 null。
    //
    // 异常:
    //   System.ArgumentOutOfRangeException:
    //     index 小于 0。- 或 -index 大于 List<T>.Count。
    bool Insert(const int, const T&);
    //
    // 摘要:
    //     搜索指定的对象，并返回整个 List<T> 中第一个匹配项的从零开始的索引。
    //
    // 参数:
    //   item:
    //     要在 List<T> 中定位的对象。对于引用类型，该值可以为 null。
    //
    // 返回结果:
    //     如果在整个 List<T> 中找到 item 的第一个匹配项，则为该项的从零开始的索引；否则为
    //     -1。
    int IndexOf(const T&);
    //
    // 摘要:
    //     从 List<T> 中移除特定对象的第一个匹配项。
    //
    // 参数:
    //   item:
    //     要从 List<T> 中移除的对象。对于引用类型，该值可以为 null。
    //
    // 返回结果:
    //     如果成功移除 item，则为 true；否则为 false。 如果在 List<T> 中没有找到
    //     item，该方法也会返回 false。
    T Next();
    bool Remove(const T&);
    //
    // 摘要:
    //     移除 List<T> 的指定索引处的元素。
    //
    // 参数:
    //   index:
    //     要移除的元素的从零开始的索引。
    //
    // 异常:
    //   System.ArgumentOutOfRangeException:
    //     index 小于 0。- 或 -index 等于或大于 List<T>.Count。
    void RemoveAt(const int);
    //
    // 摘要:
    //     从 List<T> 中移除一定范围的元素。
    //
    // 参数:
    //   index:
    //     要移除的元素的范围从零开始的起始索引。
    //
    //   count:
    //     要移除的元素数。
    //
    // 异常:
    //   System.ArgumentOutOfRangeException:
    //     index 小于 0。- 或 -count 小于 0。
    //
    //   System.ArgumentException:
    //     index 和 count 不表示 List<T> 中元素的有效范围。
    void RemoveRange(const int, const int);
private:
    Link<T> *head;
    Link<T> *tail;
    Link<T> *fence;
    int length;
    void Init()
    {
        head = tail = fence = NULL;
        length = 0;
    }
};

template<typename T>
T List<T>::Next()
{
    T rtn = NULL;
    if (fence != NULL)
    {
        rtn = fence->eletem;
        fence = fence->next;
    }
    return rtn;
}

template<typename T>
void List<T>::Begin()
{
    fence = head;
}

template<typename T>
void List<T>::RemoveRange( const int index, const int count )
{
    if (index + count > length)
    {
        return;
    }

    for (int i = 0; i < count; i++)
    {
    	RemoveAt(index);
    }
}

template<typename T>
void List<T>::Clear()
{
    while(head != NULL)
    {
        fence = head;
        head = head->next;
        delete fence;
    }
    length = 0;
}

template<typename T>
void List<T>::RemoveAt( const int index )
{
    if (index >= length)
    {
        return;
    }

    Link<T> *prev = head;
    Link<T> *curr = head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr->next == NULL)
    {
        tail = prev;
    }

    if (curr == head)
    {
        head = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    delete curr;
    --length;
}

template<typename T>
bool List<T>::Remove( const T& item )
{
    Link<T> *prev = head;
    Link<T> *curr = head;
    bool rtn = false;

    for (int i = 0; i < length; i++)
    {
        if (curr->eletem == item)
        {
            prev->next = curr->next;
            if (curr->next == NULL)
            {
                tail = prev;
            }

            delete curr;
            --length;
            rtn = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    return rtn;
}

template<typename T>
int List<T>::IndexOf( const T& item)
{
    int rtn = -1;
    fence = head;
    for (int i = 0; i < length; i++)
    {
        if (fence->eletem == item)
        {
            rtn = i;
            break;
        }
        fence = fence->next;
    }
    return rtn;
}

template<typename T>
bool List<T>::Insert( const int index, const T& item)
{
    if (index > length)
    {
        return false;
    }
    if (index == length)
    {
        Add(item);
        return true;
    }
    Link<T> *prev = head;
    Link<T> *curr = head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
    	curr = curr->next;
    }

    if (curr == head)
    {
        head = new Link<T>(item, curr); 
    }
    else
    {
        prev->next = new Link<T>(item, curr);
    }

    ++length;

    return true;
}

template<typename T>
bool List<T>::Contains( const T& item)
{
    return IndexOf(item) != -1;
}

template<typename T>
void List<T>::Add( const T& item)
{
    if (head == NULL)
    {
        //add to head
        head = tail = new Link<T>(item);
    }
    else
    {
        //add to the tail
        tail = tail->next = new Link<T>(item);
    }
    ++length;
}

template<typename T>
int List<T>::Count()
{
    return (length);
}

template<typename T>
List<T>::List()
{
    Init();
}

template<typename T>
List<T>::~List()
{
    Clear();
}

#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/
