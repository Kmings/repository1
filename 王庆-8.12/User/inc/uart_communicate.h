#pragma once

#include"main.h"
#include<stdbool.h>

#define HEAD_CHAR1 0xff
#define HEAD_CHAR2 0xfe
#define TAIL_CHAR1 0x0a
#define TAIL_CHAR2 0x0d

typedef struct uart_communicate {
	unsigned char head[2];
	unsigned char tail[2];
	unsigned char rx_data[50];
	int write;
	unsigned char tx_data[100];
	bool get_head;
	UART_HandleTypeDef* huart;
}uart_communicate;

void uart_communicate_init(uart_communicate* uc, 	UART_HandleTypeDef* huart);
void uart_communicate_receive(uart_communicate* uc, unsigned char c);
void uart_communicate_transmit(uart_communicate* uc, unsigned char id, unsigned char* data, int size);
void uart_communicate_deal(uart_communicate* uc);
bool uart_communicate_detection(uart_communicate* uc );




