#include "lcd_app.h"

/**
 * @brief  ��ʽ���ַ�������ʾ��ָ����LCD���ϡ�
 * 
 * �ú�������һ���кź�һ����ʽ���ַ�����������printf����
 * ��ʽ���ַ����󣬽�����ʾ��LCD��ָ�����ϡ�
 *
 * @param  Line    Ҫ��ʾ�ַ�����LCD�кš�
 * @param  format  ��ʽ���ַ��������Ҫ��ʽ���Ĳ�����
 *
 * �ú����ڲ�ʹ�� `vsprintf` ����ʽ���ַ�����Ȼ��
 * ���� `LCD_DisplayStringLine` ��LCD����ʾ��ʽ������ַ�����
 *
 * ʾ���÷�:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */
void LcdSprintf(uint8_t Line, char *format,...)
{
    char String[21];  // ���������ڴ洢��ʽ������ַ���
    va_list arg;      // �����б����ڴ洢�ɱ����
    va_start(arg, format);  // ʹ�ø�ʽ���ַ�����ʼ�������б�
    vsprintf(String, format, arg);  // ��ʽ���ַ������洢�ڻ�������
    va_end(arg);  // ��������б�
    LCD_DisplayStringLine(Line,String);  // ��LCD��ָ������ʾ��ʽ������ַ���
}

uint8_t count;
void lcd_proc(void)
{

	LcdSprintf(Line0,"Test LCD App");
    LcdSprintf(Line1,"Count: %d",count);
    count++;
}
