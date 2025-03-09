/**
 * CT117E-M4 / GPIO - I2C
*/

#include "i2c_hal.h"

#define DELAY_TIME	20

//
void SDA_Input_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//
void SDA_Output_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//
void SDA_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_7;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_7;
    }
}

//
void SCL_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_6;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_6;
    }
}

//
uint8_t SDA_Input(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET){
		return 1;
	}else{
		return 0;
	}
}

//
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

//
void I2CStart(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//
void I2CStop(void)
{
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(1);
    delay1(DELAY_TIME);

}

//
unsigned char I2CWaitAck(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode();
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    while(SDA_Input())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode();
            I2CStop();
            return ERROR;
        }
    }
    SCL_Output(0);
    SDA_Output_Mode();
    delay1(DELAY_TIME);
    return SUCCESS;
}

//
void I2CSendAck(void)
{
    SDA_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//
void I2CSendNotAck(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//
void I2CSendByte(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output(0);
        delay1(DELAY_TIME);
        SDA_Output(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//
unsigned char I2CReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input();
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode();
    return cR_Byte;
}

//
void I2CInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void I2CInit(void);


/**
 * @brief ��EEPROMд������
 * 
 * @param EEPROM_String ָ��Ҫд��EEPROM�����ݵ�ָ��
 * @param addr EEPROM�ڴ��е���ʼ��ַ
 * @param num Ҫд��������ֽ���
 */
void eeprom_write(uint8_t *EEPROM_String, uint8_t addr, uint8_t num)
{
    I2CStart();               // ����I2Cͨ��
    I2CSendByte(0xa0);        // ����EEPROMд���������ַ + д����λ��
    I2CWaitAck();             // �ȴ�EEPROM��ACK�ź�
    
    I2CSendByte(addr);        // ����Ҫд���EEPROM��ʼ��ַ
    I2CWaitAck();             // �ȴ�EEPROM��ACK�ź�
    
    while(num--)              // ���ֽ�д������
    {
        I2CSendByte(*EEPROM_String++);  // ���͵�ǰ�ֽڵ�����
        I2CWaitAck();                   // �ȴ�EEPROM��ACK�ź�
        delay1(200);                    // ������ʱ��ȷ������д��
    }
    I2CStop();                // ֹͣI2Cͨ��
    HAL_Delay(5);             // ��ʱ5ms��ȷ��������д��EEPROM
}

/**
 * @brief ��EEPROM��ȡ����
 * 
 * @param EEPROM_String ָ��洢��ȡ���ݵĻ�������ָ��
 * @param addr EEPROM�ڴ��е���ʼ��ַ
 * @param num Ҫ��ȡ�������ֽ���
 */
void eeprom_read(uint8_t *EEPROM_String, uint8_t addr, uint8_t num)
{
    I2CStart();               // ����I2Cͨ��
    I2CSendByte(0xa0);        // ����EEPROMд���������ַ + д����λ��
    I2CWaitAck();             // �ȴ�EEPROM��ACK�ź�
    
    I2CSendByte(addr);        // ����Ҫ��ȡ��EEPROM��ʼ��ַ
    I2CWaitAck();             // �ȴ�EEPROM��ACK�ź�
    
    I2CStart();               // �ٴ�����I2Cͨ�ţ�׼��������
    I2CSendByte(0xa1);        // ����EEPROM�����������ַ + ������λ��
    I2CWaitAck();             // �ȴ�EEPROM��ACK�ź�
    
    while(num--)              // ���ֽڶ�ȡ����
    {
        *EEPROM_String++ = I2CReceiveByte();  // ���յ�ǰ�ֽڵ�����
        if(num)
            I2CSendAck();      // �������Ҫ������ȡ������ACK�ź�
        else
            I2CSendNotAck();   // ����Ѷ�ȡ��ϣ����ͷ�ACK�ź�
    }
    I2CStop();                // ֹͣI2Cͨ��
}

/**
 * @brief ��MCP4017д������
 * 
 * @param data Ҫд��MCP4017����ֵ
 */
void mcp4017_write(uint8_t data)
{
    I2CStart();               // ����I2Cͨ��
    I2CSendByte(0x5e);        // ����MCP4017��д���������ַ + д����λ��
    I2CWaitAck();             // �ȴ�MCP4017��ACK�ź�
    I2CSendByte(data);        // ����Ҫд�������
    I2CWaitAck();             // �ȴ�MCP4017��ACK�ź�
    I2CStop();                // ֹͣI2Cͨ��
}

/**
 * @brief ��MCP4017��ȡ����
 * 
 * @param data ָ��洢��ȡ���ݵı�����ָ��
 */
void mcp4017_read(uint8_t *data)
{
    I2CStart();               // ����I2Cͨ��
    I2CSendByte(0x5f);        // ����MCP4017�Ķ����������ַ + ������λ��
    I2CWaitAck();             // �ȴ�MCP4017��ACK�ź�
    *data = I2CReceiveByte(); // ��������MCP4017������
    I2CSendNotAck();          // ���ͷ�ACK�źţ�����������
    I2CStop();                // ֹͣI2Cͨ��
}
