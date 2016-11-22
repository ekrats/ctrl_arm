/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    queue.h
 * @brief   This file contains definition of class queue
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/10     Coase        the first version
 */


/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/

#ifndef __CPP__QUEUE__H__
#define __CPP__QUEUE__H__

template<typename T>
class Queue
{
public:
    // 摘要:
    //     初始化 Queue<T> 类的新实例，该实例为空并且具有默认初始容量。
    Queue();
    ~Queue();

    // 摘要:
    //     获取 Queue<T> 中包含的元素数。
    //
    // 返回结果:
    //     Queue<T> 中包含的元素数。
    int Count();
    // 摘要:
    //     从 Queue<T> 中移除所有对象。
    void Clear();
    //
    // 摘要:
    //     确定某元素是否在 Queue<T> 中。
    //
    // 参数:
    //   item:
    //     要在 Queue<T> 中定位的对象。对于引用类型，该值可以为 null。
    //
    // 返回结果:
    //     如果在 Queue<T> 中找到 item，则为 true；否则为 false。
    bool Contains(T &);
    //
    // 摘要:
    //     移除并返回位于 Queue<T> 开始处的对象。
    //
    // 返回结果:
    //     从 Queue<T> 的开头移除的对象。
    //
    // 异常:
    //   System.InvalidOperationException:
    //     Queue<T> 为空。
    T Dequeue();
    //
    // 摘要:
    //     将对象添加到 Queue<T> 的结尾处。
    //
    // 参数:
    //   item:
    //     要添加到 Queue<T> 的对象。对于引用类型，该值可以为 null。
    void Enqueue(T &);
    //
    // 摘要:
    //     返回位于 Queue<T> 开始处的对象但不将其移除。
    //
    // 返回结果:
    //     位于 Queue<T> 的开头的对象。
    //
    // 异常:
    //   System.InvalidOperationException:
    //     Queue<T> 为空。
    T Peek();

private:
    Link<T> * font;
    Link<T> * tail;
    int length;
    void Init()
    {
        font = tail = NULL;
        length = 0;
    }
};

template<typename T>
T Queue<T>::Peek()
{
    return font->eletem;
}

template<typename T>
void Queue<T>::Enqueue( T& item )
{
    if (font == NULL)
    {
        //add to head
        font = tail = new Link<T>(item);
    }
    else
    {
        //add to the tail
        tail = tail->next = new Link<T>(item);
    }
    ++length;

}

template<typename T>
T Queue<T>::Dequeue()
{
    if (font == NULL)
    {
        return NULL;
    }
    T rtn = font->eletem;
    Link<T> *curr = font;

    if (font->next == NULL)
    {
        tail = NULL;
    }
    font = font->next;

    delete curr;
    --length;
    return rtn;
}

template<typename T>
bool Queue<T>::Contains( T& item )
{
    bool rtn = false;
    Link <T> * fence = font;
    for (int i = 0; i < length; i++)
    {
        if (fence->eletem == item)
        {
            rtn = true;
            break;
        }
        fence = fence->next;
    }
    return rtn;
}

template<typename T>
void Queue<T>::Clear()
{
    Link <T> * fence = NULL;
    while(font != NULL)
    {
        fence = font;
        font = font->next;
        delete fence;
    }
    length = 0;
}

template<typename T>
int Queue<T>::Count()
{
    return length;
}

template<typename T>
Queue<T>::~Queue()
{
    Clear();
}

template<typename T>
Queue<T>::Queue()
{
    Init();
}
#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

