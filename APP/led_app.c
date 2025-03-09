#include "led_app.h"

uint8_t ucLed[8];  // LED ״̬����

/**
 * @brief ��ʾ��ر�Led
 *
 *
 * @param ucLed Led���ݴ�������
 */
void led_disp(uint8_t *ucLed)
{
    // ���ڼ�¼��ǰ LED ״̬����ʱ����
    uint8_t temp = 0x00;
    // ��¼֮ǰ LED ״̬�ı����������ж��Ƿ���Ҫ������ʾ
    static uint8_t temp_old = 0xff;

    for (int i = 0; i < 8; i++)
        // ����ǰԪ��������Ӧ��λ�������ۼӵ�temp��
        temp |= (ucLed[i] << (7 - i));

    // ������ǰ״̬��֮ǰ״̬��ͬ��ʱ�򣬲Ÿ�����ʾ
    if (temp != temp_old)
    {
        // �� GPIOC ���ֽ����㣬���ֽڸ���Ϊ�µ� temp ֵ
        GPIOC->ODR &= 0x00ff;       // ��� GPIOC ���ֽ�
        GPIOC->ODR |= ~(temp << 8); // ���� GPIOC ���ֽ�Ϊ temp ���෴ֵ
        GPIOD->BSRR |= 0x01 << 2;   // ���� GPIOD ��2λ
        GPIOD->BRR |= 0x01 << 2;    // ���� GPIOD ��2λ
        temp_old = temp;            // ���¼�¼�ľ�״̬
    }
}

/**
 * @brief LED ��ʾ������
 *
 * ÿ�ε��øú���ʱ��LED �Ƹ��� ucLed �����е�ֵ�������ǿ������ǹر�
 */
void led_proc(void)
{
    // ��ʾ��ǰ Led_Pos λ�õ� LED ��״̬
    led_disp(ucLed);
}
