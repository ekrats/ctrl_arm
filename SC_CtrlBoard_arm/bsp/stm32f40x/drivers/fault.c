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

    fault_text[0] = "����: canͨ�Ź���";
	fault_text[1] = "���棺1#����ģ���¸й���";
	fault_text[2] = "���棺2#����ģ���¸й���";
	fault_text[3] = "���棺1#����ģ���¸й���";
	fault_text[4] = "���棺2#����ģ���¸й���";
	fault_text[5] = "���棺1#���Ӵ�����������";
	fault_text[6] = "���棺2#���Ӵ�����������";
	fault_text[7] = "���棺Ԥ���Ӵ�����������";
	fault_text[8] = "���棺1#����Ӵ�����������";
	fault_text[9] = "���棺2#����Ӵ�����������";
	fault_text[10] = "���棺3#����Ӵ�����������";
	fault_text[11] = "���棺1#������ѹ����������";
	fault_text[12] = "���棺ĸ�ߵ�ѹ����������";
	fault_text[13] = "���棺1#����ģ����������쳣";
	fault_text[14] = "���棺2#����ģ����������쳣";
	fault_text[15] = "���棺1#��������Ƿѹ";
	fault_text[16] = "���棺1#ն��ģ������ʧ��";
	fault_text[17] = "���棺2#ն��ģ������ʧ��";
	fault_text[18] = "���棺3#ն��ģ������ʧ��";
	fault_text[19] = "���棺4#ն��ģ������ʧ��";
	fault_text[32] = "���ϣ�1#�����۶�������";
	fault_text[33] = "���ϣ�2#�����۶�������";
	fault_text[34] = "���ϣ�1#���������ѹ";
	fault_text[35] = "���ϣ�ĸ�߹�ѹ";
	fault_text[36] = "���ϣ�ĸ��Ƿѹ";
	fault_text[37] = "���ϣ�1#����ģ�����";
	fault_text[38] = "���ϣ�2#����ģ�����";
	fault_text[39] = "���ϣ�1#����ģ�����";
	fault_text[40] = "���ϣ�2#����ģ�����";
	fault_text[41] = "���ϣ�1#����ģ���������������";
	fault_text[42] = "���ϣ�2#����ģ���������������";
	fault_text[43] = "���ϣ�1#����ģ���������";
	fault_text[44] = "���ϣ�2#����ģ���������";
	fault_text[64] = "���ϣ�1#������";
	fault_text[65] = "���ϣ�1#����ѹ";
	fault_text[66] = "���ϣ�1#�����ѹ����������";
	fault_text[67] = "���ϣ�1#����۶�����������";
	fault_text[68] = "���ϣ�1#ն��ģ��ض�ʧ��";
	fault_text[69] = "���ϣ�1#DCDCģ�����";
	fault_text[70] = "���ϣ�2#����۶�����������";
	fault_text[71] = "���ϣ�2#ն��ģ��ض�ʧ��";
	fault_text[72] = "���ϣ�2#DCDCģ�����";
	fault_text[73] = "���ϣ�2#������";
	fault_text[74] = "���ϣ�2#����ѹ";
	fault_text[75] = "���ϣ�2#�����ѹ����������";
	fault_text[76] = "���ϣ�3#����ѹ";
	fault_text[77] = "���ϣ�3#�����ѹ����������";
	fault_text[78] = "���ϣ�3#����۶�������";
	fault_text[79] = "���ϣ�3#ն��ģ��ض�ʧ��";
	fault_text[80] = "���ϣ�3#DCDCģ�����";
	fault_text[81] = "���ϣ�4#����۶�����������";
	fault_text[82] = "���ϣ�4#ն��ģ��ض�ʧ��";
	fault_text[83] = "���ϣ�4#DCDCģ�����";
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
                    /* ����������־���λ������ */
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
            rt_sprintf(log_info, "%d-%02d-%02d %02d:%02d:%02d  %s (����)", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec, fault_text[log_ring.fault[id].fault_type]);
        }
        else
        {
            rt_sprintf(log_info, "%d-%02d-%02d %02d:%02d:%02d  %s (�ָ�)", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
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
