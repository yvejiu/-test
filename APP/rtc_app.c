#include "rtc_app.h"

RTC_TimeTypeDef time;//����ʱ��ṹ��
RTC_DateTypeDef date;//�������ڽṹ��

void rtc_proc(void)
{
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
}

