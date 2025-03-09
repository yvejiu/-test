#include "uart_app.h"

#define BUUFER_SIZE 64

// ���廷�λ������ͽ��ջ�����
ringbuffer_t usart_rb;
uint8_t usart_read_buffer[BUUFER_SIZE];

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//   if (huart->Instance == USART1)
//   {
//     uart_rx_ticks = uwTick;
//     uart_rx_index++;
//     HAL_UART_Receive_IT(&huart1, &uart_rx_buffer[uart_rx_index], 1);
//   }
// }

/**
 * @brief UART DMA������ɻص�����
 *        �����յ�������д�뻷�λ������������DMA������
 * @param huart UART���
 * @param Size ���յ������ݴ�С
 * @retval None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
//    printf("dma data:%s\r\n", uart_rx_dma_buffer);
    // ������λ�����δ��
    if(!ringbuffer_is_full(&usart_rb))
    {
        // ��DMA�������е�����д�뻷�λ�����
        ringbuffer_write(&usart_rb, uart_rx_dma_buffer, Size);
    }
    // ���DMA������
    memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
}

/**
 * @brief  ����UART���ջ������е����ݡ�
 *         �����100ms��û�н��յ��µ����ݣ�����ջ�������
 * @param  None
 * @retval None
 */
// ����UART���ջ������е�����
void uart_proc(void)
{
    // // �����������Ϊ0��˵��û��������Ҫ����ֱ�ӷ���
    // if(uart_rx_index == 0) return;

    // // ��������һ�ν��յ����ݵ������Ѿ�����100ms
    // if(uwTick - uart_rx_ticks > 100) // 100ms��û���յ��µ�����
    // {
    //     printf("uart data: %s\n", uart_rx_buffer);

    //     // ��ս��ջ���������������������
    //     memset(uart_rx_buffer, 0, uart_rx_index);
    //     uart_rx_index = 0;
        
    //     // ��UART���ջ�����ָ������Ϊ���ջ���������ʼλ��
    //     huart1.pRxBuffPtr = uart_rx_buffer;
    // }

    // ������λ�����Ϊ�գ�ֱ�ӷ���
    if(ringbuffer_is_empty(&usart_rb)) return;
    
    // �ӻ��λ�������ȡ���ݵ���ȡ������
    ringbuffer_read(&usart_rb, usart_read_buffer, usart_rb.itemCount);
    
    // ��ӡ��ȡ�������е�����
    printf("ringbuffer data: %s\n", usart_read_buffer);
    
    // ��ն�ȡ������
    memset(usart_read_buffer, 0, sizeof(uint8_t) * BUUFER_SIZE);
}
