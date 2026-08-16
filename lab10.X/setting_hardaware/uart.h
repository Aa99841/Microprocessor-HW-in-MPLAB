#ifndef _UART_H
#define _UART_H
      
void UART_Initialize(void);
char * GetString();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();
void UART_DeleteN(int n);
void Update(int num);

#endif