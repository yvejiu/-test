#include "tim_app.h"

extern TIM_HandleTypeDef htim2;

/**
 * @brief 设置PWM占空比
 * 
 * 此函数用于设置TIM2的CH1通道的PWM占空比。占空比的范围为0%到100%。
 * 
 * @param Duty 占空比，范围为0.0到100.0。
 */
void pwm_set_duty(float Duty)
{
    // 根据占空比计算捕获/比较寄存器的值
    TIM2->CCR2 = (TIM2->ARR + 1) * (Duty / 100.0f);
}

/**
 * @brief 设置PWM频率
 * 
 * 此函数用于设置TIM2的PWM频率。频率的单位为Hz。
 * 
 * @param Frequency 频率，单位为Hz。 channel 为保留形参，具体看题目是否有多个通道需要修改。
 */
void pwm_set_frequency(int Frequency, uint8_t channel)
{
//    uint32_t clk = HAL_RCC_GetPCLK2Freq();  // 获取PCLK2时钟频率
//    if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) {
//        clk *= 2;
//    }
    
    // 根据试题实际情况
    uint32_t clk = 80 * 1000 * 1000;
    
    float duty = 0;
    uint32_t prescaler = 1;
    uint32_t arr;
    
    // 计算ARR和预分频器
    arr = clk / (Frequency * prescaler) - 1;
    
    // 如果ARR超出范围，调整预分频器
    if (arr > 65535) {
        prescaler = (arr + 65535) / 65535;
        arr = clk / (Frequency * prescaler) - 1;
//        printf("Using prescaler = %d\r\n", prescaler);
    }
    
    /* DEBUG */
//    printf("Target Frequency = %d Hz\r\n", Frequency);
//    printf("Clock = %d Hz\r\n", clk);
//    printf("ARR = %d\r\n", arr);
    
    // 保存原占空比
    duty = (float)TIM2->CCR1 / (TIM2->ARR + 1);
//            printf("Original duty for channel 1 = %.3f\r\n", duty);
    
    // 设置新的预分频值和ARR
    __HAL_TIM_SET_PRESCALER(&htim2, prescaler-1);
    TIM2->ARR = arr;
//            printf("New TIM16->ARR = %d\r\n", TIM16->ARR);
    
    // 保持占空比不变
    TIM2->CCR1 = (TIM2->ARR + 1) * duty;
//            printf("New TIM16->CCR1 = %d\r\n", TIM16->CCR1);
    
    // 更新寄存器
    TIM2->EGR = TIM_EGR_UG;
}

uint32_t tim3_ic_buffer[64];// 定义存储输入捕获值的缓冲区
uint32_t tim3_ic_val = 0;// 最终计算得到的输入捕获值
uint32_t tim3_ic_temp = 0;// 临时存储输入捕获计算的中间值

uint32_t tim8_ic_buffer[64];// 定义存储输入捕获值的缓冲区
uint32_t tim8_ic_val = 0;// 最终计算得到的输入捕获值
uint32_t tim8_ic_temp = 0;// 临时存储输入捕获计算的中间值

/**
 * @brief 处理输入捕获数据
 * 
 * 此函数用于处理输入捕获数据。它计算tim_ic_buffer中所有捕获值的平均值，
 * 然后将其转换为对应的频率值（单位为Hz），并存储在tim_ic_val中。
 * 最后，调用limit_value函数对tim_ic_val进行限制，确保其在1到20000之间。
 */
void ic_proc(void)
{
    tim3_ic_temp = 0;
    tim8_ic_temp = 0;
    
    // 对缓冲区中的捕获值求和
    for (int i = 0; i < 64; i++)
    {
        tim3_ic_temp += tim3_ic_buffer[i];
        tim8_ic_temp += tim8_ic_buffer[i];
    }

    // 计算平均值
    tim3_ic_temp /= 64;
    tim8_ic_temp /= 64;
    
    // 计算频率值，单位为Hz
    tim3_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim3_ic_temp);
    tim8_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim8_ic_temp);
    
    // 将计算得到的频率值限制在1到20000之间
    limit_value(&tim3_ic_val, 1, 500, 20000);
    limit_value(&tim8_ic_val, 1, 500, 20000);
}


