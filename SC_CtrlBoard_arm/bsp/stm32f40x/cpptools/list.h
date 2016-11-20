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
    // ժҪ:
    //     ��ȡ List<T> ��ʵ�ʰ�����Ԫ������
    //
    // ���ؽ��:
    //     List<T> ��ʵ�ʰ�����Ԫ������
    int Count();
    // ժҪ:
    //     ��������ӵ� List<T> �Ľ�β����
    //
    // ����:
    //   item:
    //     Ҫ��ӵ� List<T> ��ĩβ���Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    void Add(const T&);
    //
    // ժҪ:
    //     �� List<T> ���Ƴ�����Ԫ�ء�
    void Begin();
    void Clear();
    //
    // ժҪ:
    //     ȷ��ĳԪ���Ƿ��� List<T> �С�
    //
    // ����:
    //   item:
    //     Ҫ�� List<T> �ж�λ�Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    //
    // ���ؽ��:
    //     ����� List<T> ���ҵ� item����Ϊ true������Ϊ false��
    bool Contains(const T&);
    //
    // ժҪ:
    //     ��Ԫ�ز��� List<T> ��ָ����������
    //
    // ����:
    //   index:
    //     ���㿪ʼ��������Ӧ�ڸ�λ�ò��� item��
    //
    //   item:
    //     Ҫ����Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    //
    // �쳣:
    //   System.ArgumentOutOfRangeException:
    //     index С�� 0��- �� -index ���� List<T>.Count��
    bool Insert(const int, const T&);
    //
    // ժҪ:
    //     ����ָ���Ķ��󣬲��������� List<T> �е�һ��ƥ����Ĵ��㿪ʼ��������
    //
    // ����:
    //   item:
    //     Ҫ�� List<T> �ж�λ�Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    //
    // ���ؽ��:
    //     ��������� List<T> ���ҵ� item �ĵ�һ��ƥ�����Ϊ����Ĵ��㿪ʼ������������Ϊ
    //     -1��
    int IndexOf(const T&);
    //
    // ժҪ:
    //     �� List<T> ���Ƴ��ض�����ĵ�һ��ƥ���
    //
    // ����:
    //   item:
    //     Ҫ�� List<T> ���Ƴ��Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    //
    // ���ؽ��:
    //     ����ɹ��Ƴ� item����Ϊ true������Ϊ false�� ����� List<T> ��û���ҵ�
    //     item���÷���Ҳ�᷵�� false��
    T Next();
    bool Remove(const T&);
    //
    // ժҪ:
    //     �Ƴ� List<T> ��ָ����������Ԫ�ء�
    //
    // ����:
    //   index:
    //     Ҫ�Ƴ���Ԫ�صĴ��㿪ʼ��������
    //
    // �쳣:
    //   System.ArgumentOutOfRangeException:
    //     index С�� 0��- �� -index ���ڻ���� List<T>.Count��
    void RemoveAt(const int);
    //
    // ժҪ:
    //     �� List<T> ���Ƴ�һ����Χ��Ԫ�ء�
    //
    // ����:
    //   index:
    //     Ҫ�Ƴ���Ԫ�صķ�Χ���㿪ʼ����ʼ������
    //
    //   count:
    //     Ҫ�Ƴ���Ԫ������
    //
    // �쳣:
    //   System.ArgumentOutOfRangeException:
    //     index С�� 0��- �� -count С�� 0��
    //
    //   System.ArgumentException:
    //     index �� count ����ʾ List<T> ��Ԫ�ص���Ч��Χ��
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
