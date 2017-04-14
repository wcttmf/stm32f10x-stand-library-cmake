#ifndef USART_H
#define USART_H

#include "stm32f10x.h"
#include <stdio.h>

// user setting..
#define PRINT_TO_USART 	1

#define USART1_RECEIVED_FUNC(recv_char)
#define USART2_RECEIVED_FUNC(recv_char)
#define USART3_RECEIVED_FUNC(recv_char)
// user setting--

// dev : USART1  
// port: GPIOA
// Tx:   GPIO_Pin_9
// Rx:   GPIO_Pin_10
void usart1_init(u32 bound);
void usart1_send(char data);

// dev : USART2  
// port: GPIOA
// Tx:   GPIO_Pin_2
// Rx:   GPIO_Pin_3
void usart2_init(u32 bound);
void usart2_send(char data);

// dev : USART3  
// port: GPIOB
// Tx:   GPIO_Pin_10
// Rx:   GPIO_Pin_11
void usart3_init(u32 bound);
void usart3_send(char data);

#endif
