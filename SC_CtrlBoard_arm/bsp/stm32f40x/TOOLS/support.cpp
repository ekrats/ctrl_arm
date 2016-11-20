#include "string.h"
#include "support.h"
extern "C" void * static_malloc(size_t size);

#ifndef _WIN32
    #include <rtthread.h>
    void* operator new(size_t size)
    {
        void* p;
        
        if (rt_thread_self()->name != RT_NULL)
        {
            p = rt_calloc(1, size);
        }
        else
        {
            p = static_malloc(size);
            memset(p, 0, size);
        }
        //void* p = static_malloc(size);
        return p;
    }

    void* operator new[](size_t size)
    {
        void* p = static_malloc(size);
        memset(p, 0, size);
        return p;
    };
    void operator delete(void* p)
    {
        if(((unsigned int) p) > 0x20000000)
        {
            rt_free(p);
        }
    }

    void operator delete[](void* p)
    {
        if(((unsigned int) p) > 0x20000000)
        {
            rt_free(p);
        }
    }
#endif


/**
 * @brief This function ...
 *
 * @param filterValue
 * @param newValue
 * @param para
 *
 * @return 
 */
int Filter(int filterValue, int newValue, int para)
{
    return (((filterValue * (100 - para)) + (newValue * para)) / 100); 
}


/**
 * @brief This function ...
 *
 * @param filterValue
 * @param newValue
 * @param para
 *
 * @return 
 */
float Filter(int filterValue, int newValue, float para)
{
    return ((filterValue * (1.0f - para)) + (newValue * para));
}

/**
 * @brief This function ...
 *
 * @param filterValue
 * @param newValue
 * @param para
 *
 * @return 
 */
float Filter(float filterValue, float newValue, float para)
{
    return ((filterValue * (1.0f - para)) + (newValue * para));
}
