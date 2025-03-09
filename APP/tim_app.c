#include "tim_app.h"

/**
 * @brief ����PWMռ�ձ�
 * 
 * �˺�����������TIM2��CH1ͨ����PWMռ�ձȡ�ռ�ձȵķ�ΧΪ0%��100%��
 * 
 * @param Duty ռ�ձȣ���ΧΪ0.0��100.0��
 */
void pwm_set_duty(float Duty)
{
    // ����ռ�ձȼ��㲶��/�ȽϼĴ�����ֵ
    TIM2->CCR2 = (TIM2->ARR + 1) * (Duty / 100.0f);
}

/**
 * @brief ����PWMƵ��
 * 
 * �˺�����������TIM2��PWMƵ�ʡ�Ƶ�ʵĵ�λΪHz��
 * 
 * @param Frequency Ƶ�ʣ���λΪHz��
 */
void pwm_set_frequency(int Frequency)
{
    // ��ȡ��ʱ����ʱ��Ƶ�ʣ�����TIM2ʹ�õ�ʱ��Ƶ��ΪTIM2_CLK��
    uint32_t TIM2_CLK = 72000000;  // ����72MHz, ��Ҫ����ʵ���������

    // ���������Ƶ�ʼ����Զ���װ�ؼĴ�����ֵ
    uint32_t ARR_Value = (TIM2_CLK / Frequency) - 1;

    // �����Զ���װ�ؼĴ���
    TIM2->ARR = ARR_Value;

    // ���²���/�ȽϼĴ���CCR2�����ֵ�ǰռ�ձȲ���
    TIM2->CCR2 = (ARR_Value + 1) * (TIM2->CCR2 / (float)(TIM2->ARR + 1));
    
    // ���������¼���ˢ�¼Ĵ���
    TIM2->EGR = TIM_EGR_UG;
}



uint32_t tim_ic_buffer[64];// ����洢���벶��ֵ�Ļ�����
uint32_t tim_ic_val = 0;// ���ռ���õ������벶��ֵ
uint32_t tim_ic_temp = 0;// ��ʱ�洢���벶�������м�ֵ

/**
 * @brief �������벶������
 * 
 * �˺������ڴ������벶�����ݡ�������tim_ic_buffer�����в���ֵ��ƽ��ֵ��
 * Ȼ����ת��Ϊ��Ӧ��Ƶ��ֵ����λΪHz�������洢��tim_ic_val�С�
 * ��󣬵���limit_value������tim_ic_val�������ƣ�ȷ������1��20000֮�䡣
 */
void ic_proc(void)
{
    tim_ic_temp = 0;
    
    // �Ի������еĲ���ֵ���
    for (int i = 0; i < 64; i++)
    {
        tim_ic_temp += tim_ic_buffer[i];
    }

    // ����ƽ��ֵ
    tim_ic_temp /= 64;

    // ����Ƶ��ֵ����λΪHz
    tim_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim_ic_temp);

    // ������õ���Ƶ��ֵ������1��20000֮��
    limit_value(&tim_ic_val, 1, 500, 20000);
}

