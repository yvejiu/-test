#include "tim_app.h"

extern TIM_HandleTypeDef htim2;

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
 * @param Frequency Ƶ�ʣ���λΪHz�� channel Ϊ�����βΣ����忴��Ŀ�Ƿ��ж��ͨ����Ҫ�޸ġ�
 */
void pwm_set_frequency(int Frequency, uint8_t channel)
{
//    uint32_t clk = HAL_RCC_GetPCLK2Freq();  // ��ȡPCLK2ʱ��Ƶ��
//    if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) {
//        clk *= 2;
//    }
    
    // ��������ʵ�����
    uint32_t clk = 80 * 1000 * 1000;
    
    float duty = 0;
    uint32_t prescaler = 1;
    uint32_t arr;
    
    // ����ARR��Ԥ��Ƶ��
    arr = clk / (Frequency * prescaler) - 1;
    
    // ���ARR������Χ������Ԥ��Ƶ��
    if (arr > 65535) {
        prescaler = (arr + 65535) / 65535;
        arr = clk / (Frequency * prescaler) - 1;
//        printf("Using prescaler = %d\r\n", prescaler);
    }
    
    /* DEBUG */
//    printf("Target Frequency = %d Hz\r\n", Frequency);
//    printf("Clock = %d Hz\r\n", clk);
//    printf("ARR = %d\r\n", arr);
    
    // ����ԭռ�ձ�
    duty = (float)TIM2->CCR1 / (TIM2->ARR + 1);
//            printf("Original duty for channel 1 = %.3f\r\n", duty);
    
    // �����µ�Ԥ��Ƶֵ��ARR
    __HAL_TIM_SET_PRESCALER(&htim2, prescaler-1);
    TIM2->ARR = arr;
//            printf("New TIM16->ARR = %d\r\n", TIM16->ARR);
    
    // ����ռ�ձȲ���
    TIM2->CCR1 = (TIM2->ARR + 1) * duty;
//            printf("New TIM16->CCR1 = %d\r\n", TIM16->CCR1);
    
    // ���¼Ĵ���
    TIM2->EGR = TIM_EGR_UG;
}

uint32_t tim3_ic_buffer[64];// ����洢���벶��ֵ�Ļ�����
uint32_t tim3_ic_val = 0;// ���ռ���õ������벶��ֵ
uint32_t tim3_ic_temp = 0;// ��ʱ�洢���벶�������м�ֵ

uint32_t tim8_ic_buffer[64];// ����洢���벶��ֵ�Ļ�����
uint32_t tim8_ic_val = 0;// ���ռ���õ������벶��ֵ
uint32_t tim8_ic_temp = 0;// ��ʱ�洢���벶�������м�ֵ

/**
 * @brief �������벶������
 * 
 * �˺������ڴ������벶�����ݡ�������tim_ic_buffer�����в���ֵ��ƽ��ֵ��
 * Ȼ����ת��Ϊ��Ӧ��Ƶ��ֵ����λΪHz�������洢��tim_ic_val�С�
 * ��󣬵���limit_value������tim_ic_val�������ƣ�ȷ������1��20000֮�䡣
 */
void ic_proc(void)
{
    tim3_ic_temp = 0;
    tim8_ic_temp = 0;
    
    // �Ի������еĲ���ֵ���
    for (int i = 0; i < 64; i++)
    {
        tim3_ic_temp += tim3_ic_buffer[i];
        tim8_ic_temp += tim8_ic_buffer[i];
    }

    // ����ƽ��ֵ
    tim3_ic_temp /= 64;
    tim8_ic_temp /= 64;
    
    // ����Ƶ��ֵ����λΪHz
    tim3_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim3_ic_temp);
    tim8_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim8_ic_temp);
    
    // ������õ���Ƶ��ֵ������1��20000֮��
    limit_value(&tim3_ic_val, 1, 500, 20000);
    limit_value(&tim8_ic_val, 1, 500, 20000);
}


