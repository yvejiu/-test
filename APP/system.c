#include "system.h"

/**
 * @brief ϵͳ��ʼ������
 * 
 * �ú������ڳ�ʼ��ϵͳ�� GPIO ���ã������� GPIOC �� GPIOD ������Ĵ������в�����
 * �������������
 * 1. ��� GPIOC �� 8 λ��������ݼĴ�����
 * 2. �� GPIOC �� 8 λ��������ݼĴ�����Ϊ�ߵ�ƽ��
 * 3. ��λ GPIOD �ĵ� 2 λ��Ȼ��λ GPIOD �ĵ� 2 λ��
 */
void system_init(void)
{
    // ��� GPIOC �� 8 λ��������ݼĴ����������� 8 λ
    GPIOC->ODR &= 0x00ff;

    // �� GPIOC �� 8 λ��������ݼĴ�����Ϊ�ߵ�ƽ
    GPIOC->ODR |= ~(0x00 << 8);

    // ��λ GPIOD �ĵ� 2 λ
    GPIOD->BSRR |= 0x01 << 2;

    // ��λ GPIOD �ĵ� 2 λ
    GPIOD->BRR  |= 0x01 << 2;
}
