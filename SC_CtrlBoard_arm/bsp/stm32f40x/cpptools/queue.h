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
    // ժҪ:
    //     ��ʼ�� Queue<T> �����ʵ������ʵ��Ϊ�ղ��Ҿ���Ĭ�ϳ�ʼ������
    Queue();
    ~Queue();

    // ժҪ:
    //     ��ȡ Queue<T> �а�����Ԫ������
    //
    // ���ؽ��:
    //     Queue<T> �а�����Ԫ������
    int Count();
    // ժҪ:
    //     �� Queue<T> ���Ƴ����ж���
    void Clear();
    //
    // ժҪ:
    //     ȷ��ĳԪ���Ƿ��� Queue<T> �С�
    //
    // ����:
    //   item:
    //     Ҫ�� Queue<T> �ж�λ�Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    //
    // ���ؽ��:
    //     ����� Queue<T> ���ҵ� item����Ϊ true������Ϊ false��
    bool Contains(T &);
    //
    // ժҪ:
    //     �Ƴ�������λ�� Queue<T> ��ʼ���Ķ���
    //
    // ���ؽ��:
    //     �� Queue<T> �Ŀ�ͷ�Ƴ��Ķ���
    //
    // �쳣:
    //   System.InvalidOperationException:
    //     Queue<T> Ϊ�ա�
    T Dequeue();
    //
    // ժҪ:
    //     ��������ӵ� Queue<T> �Ľ�β����
    //
    // ����:
    //   item:
    //     Ҫ��ӵ� Queue<T> �Ķ��󡣶����������ͣ���ֵ����Ϊ null��
    void Enqueue(T &);
    //
    // ժҪ:
    //     ����λ�� Queue<T> ��ʼ���Ķ��󵫲������Ƴ���
    //
    // ���ؽ��:
    //     λ�� Queue<T> �Ŀ�ͷ�Ķ���
    //
    // �쳣:
    //   System.InvalidOperationException:
    //     Queue<T> Ϊ�ա�
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

