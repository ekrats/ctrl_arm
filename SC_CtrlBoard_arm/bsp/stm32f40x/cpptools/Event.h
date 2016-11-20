#ifndef _event_
#define _event_

template<typename T>
class Event
{
public:
	Event(T times = 5)
	{
		severlTime = times;
		length = 10;
	}
protected:
	// ժҪ: 
    //     ���ݻ�����
    T buffer[10];
	// ժҪ: 
    //     ���ݻ��泤�ȡ�
    T length;
	T result;
	// ժҪ: 
    //     ���������ڻ������е��±ꡣ
    T outIndex;
	
	T inIndex;

	T count;

	T severlTime;

	T preEent;

public:
	void UpdateOutIndex()
    {
        ++outIndex;
        if (outIndex >= length)
        {
            outIndex = 0;
        }
    }
	
	void UpdateInIndex()
    {
        ++inIndex;
        if (inIndex >= length)
        {
            inIndex = 0;
        }
    }
	
	void put(T e)
	{
		if (e != preEent)
		{
			buffer[inIndex] = e;
			preEent = e;
			UpdateInIndex();
		}
	}
	
	T get()
	{
		if (outIndex != inIndex)
		{
			result = buffer[outIndex];
			if (++count >= severlTime)
			{
				UpdateOutIndex();
				count = 0;
			}
		}
		else
		{
			result = 0;
		}
		return result;
	}
};

#endif
