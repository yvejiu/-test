#include "scheduler.h"


// ȫ�ֱ��������ڴ洢��������
uint8_t task_num;

typedef struct {
    void (*task_func)(void);
    uint32_t rate_ms;
    uint32_t last_run;
} task_t;

// ��̬�������飬ÿ�����������������ִ�����ڣ����룩���ϴ�����ʱ�䣨���룩
static task_t scheduler_task[] =
{
    {led_proc, 1, 0},  // ����һ������������Ϊ Led_Proc��ִ������Ϊ 10 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {key_proc, 10, 0},  // ����һ������������Ϊ Key_Proc��ִ������Ϊ 10 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {lcd_proc, 100, 0},  // ����һ������������Ϊ Lcd_Proc��ִ������Ϊ 100 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {uart_proc, 10, 0}, // ����һ������������Ϊ Uart_Proc��ִ������Ϊ 10 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {adc_proc,20,0},//����һ������������Ϊ Adc_Proc��ִ������Ϊ 20 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {rtc_proc,10,0},//����һ������������Ϊ Rtc_Proc��ִ������Ϊ 500 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
    {ic_proc,50,0}//����һ������������Ϊ Ic_Proc��ִ������Ϊ 100 ���룬��ʼ�ϴ�����ʱ��Ϊ 0
};

/**
 * @brief ��������ʼ������
 * �������������Ԫ�ظ�������������洢�� task_num ��
 */
void scheduler_init(void)
{
    // �������������Ԫ�ظ�������������洢�� task_num ��
    task_num = sizeof(scheduler_task) / sizeof(task_t);
}

/**
 * @brief ���������к���
 * �����������飬����Ƿ���������Ҫִ�С������ǰʱ���Ѿ����������ִ�����ڣ���ִ�и����񲢸����ϴ�����ʱ��
 */
void scheduler_run(void)
{
    // �������������е���������
    for (uint8_t i = 0; i < task_num; i++)
    {
        // ��ȡ��ǰ��ϵͳʱ�䣨���룩
        uint32_t now_time = HAL_GetTick();

        // ��鵱ǰʱ���Ƿ�ﵽ�����ִ��ʱ��
        if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run)
        {
            // ����������ϴ�����ʱ��Ϊ��ǰʱ��
            scheduler_task[i].last_run = now_time;

            // ִ��������
            scheduler_task[i].task_func();
        }
    }
}


