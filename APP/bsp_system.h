#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "rtc.h"
#include "lcd.h"
#include "main.h"
#include "usart.h"
#include "system.h"
#include "filter.h"
#include "scheduler.h"
#include "led_app.h"
#include "key_app.h"
#include "lcd_app.h"
#include "uart_app.h"
#include "adc_app.h"
#include "rtc_app.h"
#include "tim_app.h"
#include "ringbuffer.h"

extern uint16_t uart_rx_index;//���ջ�������
extern uint32_t uart_rx_ticks;//���ջ���ʱ���
extern uint8_t uart_rx_buffer[128];//���ջ���
extern uint8_t ucLed[8];// LED ״̬����
extern uint8_t uart_rx_dma_buffer[128];//���ջ���
extern uint32_t dma_buff[2][30];//DMA���ջ���
extern float adc_value[2];//ADC����ֵ����
extern RTC_TimeTypeDef time;//����ʱ��ṹ��
extern RTC_DateTypeDef date;//�������ڽṹ��
extern uint32_t tim_ic_buffer[64];// ����洢���벶��ֵ�Ļ�����
extern uint32_t tim_ic_val;// ���ռ���õ������벶��ֵ


#endif


