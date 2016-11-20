#include "canApp.h"
#include "canTask.h"
#include "stm32f4xx.h"
#include <rtthread.h>
#include "string.h"
#include "can.h"
#include "can_app_def.h"
#include "ScManagerExtern.h"
#include "ScManager.h"
#include "DataStruct.h"

extern CanApp can;

/****************************************************************
* Description    CAN数据帧 配置表
*****************************************************************/
static STD_MSG_CON_TB _std_msg_table[CAN_STD_FRAME_NUM] = 
{ 
	/*port     type          dest         src       func                   		 len    rxFrame  update   candata              */
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_CHARGE,    		8,     0,      0, 	 (uint8_t *)&can.CanBuf.dcdcCmd,    (uint8_t *)&can.sCan.canAppBuf.dcdcCmd,    		can.MsgRx_Deal},
	{PORTx, CAN_RX_DIRC, can.mxcx_id,    can.mon_id,  	CAN_FUNC_REBOOT,	   		8,     0,      0, 	 can.CanBuf.reBoot,   				(uint8_t *)&can.sCan.canAppBuf.reBoot,    		RT_NULL},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_SET_PARA,  	   24,     0,      0, 	 (uint8_t *)&can.CanBuf.cbWrPara,    (uint8_t *)&can.sCan.cbWrPara,    				can.MsgRx_Deal},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_SET_ADJUST,       24,     0,      0, 	 (uint8_t *)&can.CanBuf.cbWrAdj,    (uint8_t *)&can.sCan.cbWrAdj,    				can.MsgRx_Deal},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_S_POLL,     		8,     7,      0,    can.CanBuf.null,    				(uint8_t *)&can.sCan.canAppBuf.null,    		RT_NULL},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_P_POLL,	        8,     8,      0, 	 can.CanBuf.null,   				(uint8_t *)&can.sCan.canAppBuf.null,    		RT_NULL},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_AD_POLL,	        8,     9,      0, 	 can.CanBuf.null,   				(uint8_t *)&can.sCan.canAppBuf.null,    		RT_NULL},

	{PORTx, CAN_ZX_DIRC, can.mxcx_id,     can.ctrl_id,  	CAN_FUNC_S_POLL,           24,     0,      0, 	 (uint8_t *)&can.CanBuf.cbStatus,   (uint8_t *)&can.sCan.cbStatus,    				RT_NULL},
	{PORTx, CAN_ZX_DIRC, can.mxcx_id,     can.ctrl_id,  	CAN_FUNC_P_POLL,           24,     0,      0, 	 (uint8_t *)&can.CanBuf.cbPara,     (uint8_t *)&can.sCan.cbPara,    				RT_NULL},
	{PORTx, CAN_ZX_DIRC, can.mxcx_id,     can.ctrl_id,  	CAN_FUNC_AD_POLL,          24,     0,      0, 	 (uint8_t *)&can.CanBuf.cbAdj,      (uint8_t *)&can.sCan.cbAdj,    					RT_NULL},	
	                                                                               
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_IAP_CHECK,   	    8,    13,      0, 	 (uint8_t *)&can.CanBuf.iap_check,  (uint8_t *)&can.sCan.canAppBuf.iap_check,    	can.MsgRx_Deal},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_IAP_INFO,  	 0x10,    13,      0, 	 (uint8_t *)&can.CanBuf.iap_info,   (uint8_t *)&can.sCan.canAppBuf.iap_info,    	can.MsgRx_Deal},
	{PORTx, CAN_RX_DIRC, can.ctrl_id,    can.mon_id,  	CAN_FUNC_IAP_WRITE,      0xff,    13,      0, 	 (uint8_t *)&can.CanBuf.iap_file,   (uint8_t *)&can.sCan.canAppBuf.iap_file,    	can.MsgRx_Deal},
	{PORTx, CAN_ZX_DIRC, can.mxcx_id,  	 can.ctrl_id,   CAN_FUNC_IAP,   		    8,     0,      0, 	 (uint8_t *)&can.CanBuf.iap_reply,  (uint8_t *)&can.sCan.canAppBuf.iap_reply,    	RT_NULL},
};

//static __IO STD_MSG_CON_TB *_std_msg = _std_msg_table;

extern u16 local_id;

CanApp::CanApp(ScData & scCan):sCan(scCan)                                                                                                                                                   
{
	_std_msg = _std_msg_table;
	
	CanStateWord = 0x000F;
	
	canOffTime = 200;
}

void CanApp::Init(void)
{
	ctrl_id = local_id;
	u16 i = 0;
	
	if (local_id <= CAN_ID_C4)
	{
		mon_id = CAN_ID_M1;
		mxcx_id = CAN_ID_M1CX;
	}
	else if (local_id >= CAN_ID_C9)
	{
		mon_id = CAN_ID_M2;
		mxcx_id = CAN_ID_M2CX;
	}
	for (i=0; i<CAN_STD_FRAME_NUM; i++)
	{
		if (_std_msg[i].direction == CAN_RX_DIRC)
		{
			_std_msg[i].Dest = can.ctrl_id;
			_std_msg[i].Src = can.mon_id;
		}
		else if (_std_msg[i].direction == CAN_ZX_DIRC)
		{
			_std_msg[i].Dest = can.mxcx_id;
			_std_msg[i].Src = can.ctrl_id;
		}
	}
	
	_std_msg[1].Dest = can.mxcx_id;
}

void CanApp::MsgRx_Deal(CAN_RX_DATA_RAM* _p, EXTID_UTYP* _tRecId, uint8_t frame)
{
	static uint16_t DataLen[CAN_STD_FRAME_NUM] = {0};
	//------------------------------------------------------------------
	// 收到首帧
	//------------------------------------------------------------------
	if(_tRecId->st.packetID == 0)  DataLen[frame] = 0;
	//------------------------------------------------------------------
	// 接收数据
	//------------------------------------------------------------------
	memcpy((uint8_t*)(can._std_msg[frame].CanBuffData + _tRecId->st.packetID * 8),
		_p->data.Data,
		_p->data.DLC);
	//------------------------------------------------------------------
	// 统计收到帧长
	//------------------------------------------------------------------
	DataLen[frame] += _p->data.DLC;
	//------------------------------------------------------------------
	// 收到尾帧
	//------------------------------------------------------------------
	if(_tRecId->st.packetID == ((can._std_msg[frame].len - 1) / 8))
	{
		//----------------------------------------------------------------
		// 帧长正确,更新数据
		//----------------------------------------------------------------
		if(DataLen[frame] == can._std_msg[frame].len)
		{
			can._std_msg[frame].update = 1;
		}
		//----------------------------------------------------------------
		// 清0帧长
		//----------------------------------------------------------------
		DataLen[frame] = 0;
	}
}

static int tx_index = 0;

void CanApp::TriggerMsgPush(void)
{
	uint32_t ExtId = 0;
	uint16_t frameLen = 0;
	uint8_t Sendlen = 0;
	uint8_t ScanFrame = 0; 
	uint8_t frameCnt = 0;
	uint8_t	lastFrameFlag = 0;
	//-----------------------------------------------------------------
	// 扫描端口信息
	//------------------------------------------------------------------
	for(ScanFrame = 0; ScanFrame < CAN_STD_FRAME_NUM; ScanFrame++)
	{
		if(_std_msg[ScanFrame].direction == CAN_ZX_DIRC)
		{			
			if(_std_msg[ScanFrame].update  == 0)
				continue;

			_std_msg[ScanFrame].update = 0;

			if((_std_msg[ScanFrame].CanBuffData == RT_NULL) && (_std_msg[ScanFrame].len > 0))
				continue;

			//-----------------------------------------------------------------
			// 获取要发送端口数据长度
			//------------------------------------------------------------------
			frameLen = _std_msg[ScanFrame].len;
			//-----------------------------------------------------------------
			// 首帧帧编号
			//------------------------------------------------------------------
			frameCnt = 0;
			//-----------------------------------------------------------------
			// 拆包发送
			//------------------------------------------------------------------
			while(frameLen > 0)
			{
				//-----------------------------------------------------------
				// 计算本包数据长度
				//-----------------------------------------------------------
				if(frameLen > 8)
				{
					lastFrameFlag = 0;
					Sendlen = 8;
					frameLen -= 8;
				}
				else
				{
					lastFrameFlag = 1;
					Sendlen = frameLen;
					frameLen = 0;
				}

				ExtId = MakeCanMsgExtID(_std_msg[ScanFrame].direction, //优先级
					_std_msg[ScanFrame].FuncID,                          //功能号
					frameCnt,                                            //帧号
					_std_msg[ScanFrame].Dest,                            //宿端
					_std_msg[ScanFrame].Src,
					lastFrameFlag);                             //源端

				MakeTxAckMsg(ExtId,						           //扩展ID
					Sendlen,                                       //发送数据字节长度
					&_std_msg[ScanFrame].CanBuffData[frameCnt*8],  //要发送数据
					&_tCyc);                                       //发送缓冲区

				if (_std_msg[ScanFrame].canPort == PORTx)
				{
					if (tx_index)
					{
						can_send_buffer(PORT1 , &_tCyc);
						tx_index  = false;
					}
					else
					{
						can_send_buffer(PORT2 , &_tCyc);
						tx_index  = true;
					}
				}
				else
					can_send_buffer(_std_msg[ScanFrame].canPort , &_tCyc);

				//-----------------------------------------------------------------
				// 维护帧编号
				//------------------------------------------------------------------
				frameCnt++;
			}
		}
	}
}

void CanApp::MsgRx_Manage(CAN_RX_DATA_RAM* pbuf, uint32_t _tId)
{
	CAN_RX_DATA_RAM *_p  = pbuf;
	PEXTID_UTYP _tRecStdId = (PEXTID_UTYP)&_tId;
	static uint8_t timeDelay = 0;
	uint8_t ScanFrame = 0;

	if(_p->parent.CanPort == PORT1 || _p->parent.CanPort == PORT2)
	{
		for(ScanFrame = 0; ScanFrame < CAN_STD_FRAME_NUM; ScanFrame++)
		{
			if((_std_msg[ScanFrame].direction == CAN_RX_DIRC)
				&& (_std_msg[ScanFrame].Src == _tRecStdId->st.sourceID)
				&& (_std_msg[ScanFrame].Dest == _tRecStdId->st.sinkID)
				&& (_std_msg[ScanFrame].FuncID == _tRecStdId->st.func))
			{
				if(_std_msg[ScanFrame].callback != RT_NULL)
				{
					_std_msg[ScanFrame].callback(_p, _tRecStdId, ScanFrame);
				}
				
				RxMsgUpdate_data();
				
				//回复
				if (_std_msg[ScanFrame].txFrame > 0)
				{
					_std_msg[_std_msg[ScanFrame].txFrame].canPort = _p->parent.CanPort;
					SendCan(_std_msg[ScanFrame].txFrame);
				}
				
				switch (_tRecStdId->st.func)
				{
					case CAN_FUNC_CHARGE:
						break;
					
					case CAN_FUNC_S_POLL:
						break;
					
					case CAN_FUNC_P_POLL:
						break;
					
					case CAN_FUNC_AD_POLL:
						break;
					
					case CAN_FUNC_REBOOT:
						break;
					
					case CAN_FUNC_SET_PARA:
						break;
					
					case CAN_FUNC_SET_ADJUST:
						break;
					
					case CAN_FUNC_IAP_INFO:
						break;
					
					case CAN_FUNC_IAP_WRITE:
						break;
					
					case CAN_FUNC_IAP_CHECK:
						break;
					
					default:
						break;
				}
				
				can.CanCnt = 0;				
				//-----------------------------------------------------
				// can指示灯
				//-----------------------------------------------------
				if(++timeDelay % 2 == 0)
				{
					timeDelay = 0;
					can_led();
				}
			}
		}
	}
	else
	{
		/*  do some warning*/
	}
}

void CanApp::RxMsgUpdate_data(void)
{
	uint8_t  ScanFrame = 0;
	//-----------------------------------------------------------------
	// 扫描端口信息
	//------------------------------------------------------------------
	for(ScanFrame = 0; ScanFrame < CAN_STD_FRAME_NUM; ScanFrame++)
	{
		if(_std_msg[ScanFrame].direction == CAN_RX_DIRC)
		{
			if(_std_msg[ScanFrame].update == 1)
			{
				memcpy(_std_msg[ScanFrame].AppBuffData, _std_msg[ScanFrame].CanBuffData, _std_msg[ScanFrame].len);
				_std_msg[ScanFrame].update = 0;
			}
		}
	}
}

void CanApp::TriggerMsgUpdate_data(uint8_t frame)
{
	//-----------------------------------------------------------------
	// 更新端口信息
	//------------------------------------------------------------------
	if(_std_msg[frame].direction == CAN_ZX_DIRC)
	{
		if((_std_msg[frame].CanBuffData != RT_NULL) && (_std_msg[frame].AppBuffData != RT_NULL))
		{
			rt_memcpy(_std_msg[frame].CanBuffData, _std_msg[frame].AppBuffData, _std_msg[frame].len);
		}
		_std_msg[frame].update = 1;
	}
}


void CanApp::MakeTxAckMsg(uint32_t extID, uint8_t len, uint8_t* data, CanTxMsg* msg)
{
	if(len >= 8)
	{
		len = 8;
	}

	msg->ExtId   = extID;
	msg->IDE     = CAN_ID_EXT;
	msg->RTR     = 0;
	msg->DLC     = len;

	if((data != RT_NULL) && (len != 0))
	{
		msg->Data[0] = *data;
		msg->Data[1] = *(data + 1);
		msg->Data[2] = *(data + 2);
		msg->Data[3] = *(data + 3);
		msg->Data[4] = *(data + 4);
		msg->Data[5] = *(data + 5);
		msg->Data[6] = *(data + 6);
		msg->Data[7] = *(data + 7);
	}
}

uint32_t CanApp::MakeCanMsgExtID(uint8_t classID, uint8_t func,  uint8_t frame,
								 uint8_t dest, uint8_t src, uint8_t last)
{
	EXTID_UTYP id;

	id.st.classID  = classID;
	id.st.packetID = frame;
	id.st.sourceID   = src;
	id.st.sinkID   = dest;
	id.st.func    = func;
	id.st.lastFrame = last;

	return (*(uint32_t*)&id & 0x1FFFFFFF);
}

void can_rx_interface(uint8_t port, CanRxMsg* msg)
{
	CAN_RX_DATA_RAM * rx_buffer = RT_NULL;
    uint8_t * tmp = msg->Data;
    
    if(tmp != RT_NULL)
    {
        rx_buffer = (CAN_RX_DATA_RAM *)rt_malloc(sizeof(CAN_RX_DATA_RAM));
        if(rx_buffer != RT_NULL)
        {
            rx_buffer->parent.CanPort = port;
			rx_buffer->data.StdId = msg->StdId;
			rx_buffer->data.ExtId = msg->ExtId;
			rx_buffer->data.IDE = msg->IDE;
			rx_buffer->data.RTR = msg->RTR;
			rx_buffer->data.DLC = msg->DLC;
            rx_buffer->data.Data[0] = tmp[0];
            rx_buffer->data.Data[1] = tmp[1];
            rx_buffer->data.Data[2] = tmp[2];
            rx_buffer->data.Data[3] = tmp[3];
            rx_buffer->data.Data[4] = tmp[4];
            rx_buffer->data.Data[5] = tmp[5];
            rx_buffer->data.Data[6] = tmp[6];
            rx_buffer->data.Data[7] = tmp[7];
            can.MsgRx_Manage(rx_buffer, msg->ExtId);
        }
		rt_free(rx_buffer);
    }
}

void CanApp_TriggerStream_Server(void)
{
	can.TriggerMsgPush();
}

void SendCan(uint8_t frame)
{
	if(frame < CAN_STD_FRAME_NUM)
	{
		can.TriggerMsgUpdate_data(frame);
		Bsp_can_send_trigger_event();
	}
}

void iap_send(uint8_t frame, uint16_t lenth)
{
	_std_msg_table[frame].len = lenth+1;
	_std_msg_table[frame].AppBuffData[lenth] = lenth + 1;
	SendCan(frame);
}

void gbch_init(void)
{
    can_services_init();
    can_rx_thread_init();
	can_thread_init();

	//---------------------------------------------------------------
	// 打开CAN控制器中断
	//---------------------------------------------------------------
#if(BSP_USE_CAN1 == 1)
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
#endif

#if(BSP_USE_CAN2 == 1)
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
#endif
}

void can_led(void)
{
	if(can.CanStateWord > 0)
	{
		stm32_hw_io_toggle(2);
	}
	else
	{
		stm32_hw_io_off(2);
	}
}

