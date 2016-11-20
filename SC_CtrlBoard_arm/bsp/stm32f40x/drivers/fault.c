#include "rtdef.h"
#include "rtthread.h"
#include "rtservice.h"
#include "fault.h"
#include "DataStruct.h"
#include "ScManagerExtern.h"
#include "time.h"

#define FaultOccur      1
#define FaultRemove     0

typedef struct 
{
    uint32_t    unix_time;
    uint32_t    is_occur;
    uint32_t    fault_type;
}FAULT_PACKAGE;

typedef struct 
{
    FAULT_PACKAGE   fault;
    rt_list_t       list;
}FAULT_LIST;

typedef struct 
{
    uint32_t        cur_id;
    FAULT_PACKAGE   fault[LOG_RING_LENGTH];
}LOG_RING;

FAULT_LIST  fault_list = { 0 };
LOG_RING    log_ring = { 0 };
uint8_t fault_bit[16] = { 0 };
uint8_t bak_bit[16] = { 0 };
char* fault_text[128];

void fault_init(void)
{   
    rt_list_init(&fault_list.list);

    fault_text[0] = "¾¯¸æ: canÍ¨ÐÅ¹ÊÕÏ";
	fault_text[1] = "¾¯¸æ£º1#ÕûÁ÷Ä£¿éÎÂ¸Ð¹ÊÕÏ";
	fault_text[2] = "¾¯¸æ£º2#ÕûÁ÷Ä£¿éÎÂ¸Ð¹ÊÕÏ";
	fault_text[3] = "¾¯¸æ£º1#µçÈÝÄ£¿éÎÂ¸Ð¹ÊÕÏ";
	fault_text[4] = "¾¯¸æ£º2#µçÈÝÄ£¿éÎÂ¸Ð¹ÊÕÏ";
	fault_text[5] = "¾¯¸æ£º1#Ö÷½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[6] = "¾¯¸æ£º2#Ö÷½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[7] = "¾¯¸æ£ºÔ¤³äµç½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[8] = "¾¯¸æ£º1#Êä³ö½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[9] = "¾¯¸æ£º2#Êä³ö½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[10] = "¾¯¸æ£º3#Êä³ö½Ó´¥Æ÷·´À¡¹ÊÕÏ";
	fault_text[11] = "¾¯¸æ£º1#½»Á÷µçÑ¹´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[12] = "¾¯¸æ£ºÄ¸ÏßµçÑ¹´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[13] = "¾¯¸æ£º1#ÕûÁ÷Ä£¿éµçÁ÷´«¸ÐÒì³£";
	fault_text[14] = "¾¯¸æ£º2#ÕûÁ÷Ä£¿éµçÁ÷´«¸ÐÒì³£";
	fault_text[15] = "¾¯¸æ£º1#½»Á÷ÊäÈëÇ·Ñ¹";
	fault_text[16] = "¾¯¸æ£º1#Õ¶²¨Ä£¿éÆô¶¯Ê§°Ü";
	fault_text[17] = "¾¯¸æ£º2#Õ¶²¨Ä£¿éÆô¶¯Ê§°Ü";
	fault_text[18] = "¾¯¸æ£º3#Õ¶²¨Ä£¿éÆô¶¯Ê§°Ü";
	fault_text[19] = "¾¯¸æ£º4#Õ¶²¨Ä£¿éÆô¶¯Ê§°Ü";
	fault_text[32] = "¹ÊÕÏ£º1#ÊäÈëÈÛ¶ÏÆ÷¹ÊÕÏ";
	fault_text[33] = "¹ÊÕÏ£º2#ÊäÈëÈÛ¶ÏÆ÷¹ÊÕÏ";
	fault_text[34] = "¹ÊÕÏ£º1#½»Á÷ÊäÈë¹ýÑ¹";
	fault_text[35] = "¹ÊÕÏ£ºÄ¸Ïß¹ýÑ¹";
	fault_text[36] = "¹ÊÕÏ£ºÄ¸ÏßÇ·Ñ¹";
	fault_text[37] = "¹ÊÕÏ£º1#ÕûÁ÷Ä£¿é¹ýÎÂ";
	fault_text[38] = "¹ÊÕÏ£º2#ÕûÁ÷Ä£¿é¹ýÎÂ";
	fault_text[39] = "¹ÊÕÏ£º1#µçÈÝÄ£¿é¹ýÎÂ";
	fault_text[40] = "¹ÊÕÏ£º2#µçÈÝÄ£¿é¹ýÎÂ";
	fault_text[41] = "¹ÊÕÏ£º1#ÕûÁ÷Ä£¿éµçÁ÷´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[42] = "¹ÊÕÏ£º2#ÕûÁ÷Ä£¿éµçÁ÷´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[43] = "¹ÊÕÏ£º1#ÕûÁ÷Ä£¿éµçÁ÷¹ýÁ÷";
	fault_text[44] = "¹ÊÕÏ£º2#ÕûÁ÷Ä£¿éµçÁ÷¹ýÁ÷";
	fault_text[64] = "¹ÊÕÏ£º1#³äµç¹ýÁ÷";
	fault_text[65] = "¹ÊÕÏ£º1#³äµç¹ýÑ¹";
	fault_text[66] = "¹ÊÕÏ£º1#Êä³öµçÑ¹´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[67] = "¹ÊÕÏ£º1#Êä³öÈÛ¶ÏÆ÷¸ÐÆ÷¹ÊÕÏ";
	fault_text[68] = "¹ÊÕÏ£º1#Õ¶²¨Ä£¿é¹Ø¶ÏÊ§°Ü";
	fault_text[69] = "¹ÊÕÏ£º1#DCDCÄ£¿é¹ÊÕÏ";
	fault_text[70] = "¹ÊÕÏ£º2#Êä³öÈÛ¶ÏÆ÷¸ÐÆ÷¹ÊÕÏ";
	fault_text[71] = "¹ÊÕÏ£º2#Õ¶²¨Ä£¿é¹Ø¶ÏÊ§°Ü";
	fault_text[72] = "¹ÊÕÏ£º2#DCDCÄ£¿é¹ÊÕÏ";
	fault_text[73] = "¹ÊÕÏ£º2#³äµç¹ýÁ÷";
	fault_text[74] = "¹ÊÕÏ£º2#³äµç¹ýÑ¹";
	fault_text[75] = "¹ÊÕÏ£º2#Êä³öµçÑ¹´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[76] = "¹ÊÕÏ£º3#³äµç¹ýÑ¹";
	fault_text[77] = "¹ÊÕÏ£º3#Êä³öµçÑ¹´«¸ÐÆ÷¹ÊÕÏ";
	fault_text[78] = "¹ÊÕÏ£º3#Êä³öÈÛ¶ÏÆ÷¹ÊÕÏ";
	fault_text[79] = "¹ÊÕÏ£º3#Õ¶²¨Ä£¿é¹Ø¶ÏÊ§°Ü";
	fault_text[80] = "¹ÊÕÏ£º3#DCDCÄ£¿é¹ÊÕÏ";
	fault_text[81] = "¹ÊÕÏ£º4#Êä³öÈÛ¶ÏÆ÷¸ÐÆ÷¹ÊÕÏ";
	fault_text[82] = "¹ÊÕÏ£º4#Õ¶²¨Ä£¿é¹Ø¶ÏÊ§°Ü";
	fault_text[83] = "¹ÊÕÏ£º4#DCDCÄ£¿é¹ÊÕÏ";
}

int is_over_length = 0;
static void fault_control(uint32_t byte, uint32_t bit, uint32_t type)
{
    uint32_t text_index;
    FAULT_LIST* buffer;
    FAULT_LIST* fault;
    rt_list_t* node;

    text_index = 8 * byte + bit;

    switch (type)
    {
    case FaultOccur:
        buffer = (FAULT_LIST*)rt_malloc(sizeof(FAULT_LIST));
        //buffer->fault.unix_time = get_unix_time();
        buffer->fault.fault_type = text_index;
        rt_list_insert_after(fault_list.list.prev, &buffer->list);
        
        //log_ring.fault[log_ring.cur_id].unix_time = get_unix_time();
        log_ring.fault[log_ring.cur_id].is_occur = FaultOccur;
        log_ring.fault[log_ring.cur_id].fault_type = text_index;
        break;

    case FaultRemove:
        for (node = fault_list.list.next; node != &fault_list.list; node = node->next)
        {
            fault = rt_list_entry(node, FAULT_LIST, list);
            if (fault->fault.fault_type == text_index)
            {
                rt_list_remove(node);
                rt_free(fault);
                break;
            }
        }

        //log_ring.fault[log_ring.cur_id].unix_time = get_unix_time();
        log_ring.fault[log_ring.cur_id].is_occur = FaultRemove;
        log_ring.fault[log_ring.cur_id].fault_type = text_index;
        break;

    default:
        break;
    }

    log_ring.cur_id++;
    if (log_ring.cur_id == LOG_RING_LENGTH)
    {
        log_ring.cur_id = 0;
        is_over_length = 1;
    }
}


void fault_check(void)
{
    ScData * p = (ScData *)GetShareDataPtr();
	uint32_t byte_index, bit_index;
    uint32_t cur, old;
    rt_memcpy((void*)&fault_bit, (void*)&p->output.warn, (sizeof(sc_warn_t)+sizeof(sc_fault_t)));
    for (byte_index = 0; byte_index < 16; byte_index++)
    {
        if (fault_bit[byte_index] != bak_bit[byte_index])
        {
            for (bit_index = 0; bit_index < 8; bit_index++)
            {
                cur = fault_bit[byte_index] & (1 << bit_index);
                old = bak_bit[byte_index] & (1 << bit_index);
                if (cur != old)
                {
                    /* ¹ÊÕÏÁ´±í¼°ÈÕÖ¾»·ÐÎ»º´æ²Ù×÷ */
                    fault_control(byte_index, bit_index, (cur>old));
                }
            }
        }
    }
    rt_memcpy((void*)bak_bit, (void*)fault_bit, sizeof(fault_bit));
}

#ifdef RT_USING_FINSH
#include "finsh.h"

char fault_info[64] = { "" };
char* get_fault(int index)
{
    int i = 0;
    FAULT_LIST* fault;
    rt_list_t* node;
    struct tm* t;

    for (node = fault_list.list.prev; node != &fault_list.list; node = node->prev)
    {
        if (index == i)
        {
            fault = rt_list_entry(node, FAULT_LIST, list);
            t = localtime((time_t*)&(fault->fault.unix_time));
            rt_sprintf(fault_info, "%d-%02d-%02d %02d:%02d:%02d  %s", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec, fault_text[fault->fault.fault_type]);
        }
        i++;
    }
    if (index >= i)
    {
        rt_sprintf(fault_info, "");
    }
	rt_kprintf("%s\n", fault_info);
	
    return fault_info;
}
FINSH_FUNCTION_EXPORT(get_fault, get fault info. e.g: get_fault(int));

char log_info[64] = { "" };
char* get_log(int index)
{
    int id;
	int state;
    struct tm* t;
    
    id = log_ring.cur_id - index - 1;
    if (id < 0 && is_over_length)
    {
        id += 20;
    }

    if (id >= 0)
    {
        t = localtime((time_t*)&(log_ring.fault[id].unix_time));
        state = log_ring.fault[id].is_occur;
        if (state)
        {
            rt_sprintf(log_info, "%d-%02d-%02d %02d:%02d:%02d  %s (·¢Éú)", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec, fault_text[log_ring.fault[id].fault_type]);
        }
        else
        {
            rt_sprintf(log_info, "%d-%02d-%02d %02d:%02d:%02d  %s (»Ö¸´)", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec, fault_text[log_ring.fault[id].fault_type]);
        }
    }
    else
    {
        rt_sprintf(log_info, "");
        state = 2;
    }
	rt_kprintf("%s\n", log_info);
	
    return log_info;
}

FINSH_FUNCTION_EXPORT(get_log, get log info. e.g: get_log(int));

uint32_t get_log_id(void)
{
	return log_ring.cur_id;
}

FINSH_FUNCTION_EXPORT(get_log_id, get log id info.);
#endif
/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/

/*@}*/
