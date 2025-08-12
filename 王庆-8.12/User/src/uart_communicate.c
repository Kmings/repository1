#include "uart_communicate.h"

void uart_communicate_init(uart_communicate* uc, 	UART_HandleTypeDef* huart){
	uc->write = 0;
	uc->get_head = 0;
	uc->huart = huart;
}
	
void uart_communicate_receive(uart_communicate* uc, unsigned char c){
	if(uc->get_head){
		uc->tail[0] = uc->tail[1];
		uc->tail[1] = c;
		if(uc->tail[1] == TAIL_CHAR2 && uc->tail[0] == TAIL_CHAR1){
			if(uart_communicate_detection(uc))
				uart_communicate_deal(uc);
			uc->write = 0;
			uc->get_head = 0;
			return;
		}
		uc->rx_data[uc->write++] = c;
	}
	else {
		uc->head[0] = uc->head[1];
		uc->head[1] = c;
		if(uc->head[1] == HEAD_CHAR2 &&uc->head[0] == HEAD_CHAR1)
			uc->get_head = true;
	}
	if(uc->write >= 50){
		uc->write = 0;
		uc->get_head = 0;
	}
}

bool uart_communicate_detection(uart_communicate* uc ){
  unsigned short sum = 0;
	int i;
	for(i = 0; i< uc->write - 2; ++i){
		sum += uc->rx_data[i];
		sum &= 0x00ff;
	}
	if(sum != uc->rx_data[i]) return false;
	return true;
}

void uart_communicate_deal(uart_communicate* uc) {
	switch(uc->rx_data[0]) {
		case 0:
			break;
		case 1:
			if(uc->rx_data[1] == 1)
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
				uart_communicate_transmit(uc, 0, &uc->rx_data[1], 1);
			break;
	}
}
void uart_communicate_transmit(uart_communicate* uc, unsigned char id, unsigned char* data, int size){
	uc->tx_data[0] = HEAD_CHAR1;
	uc->tx_data[1] = HEAD_CHAR2;
	uc->tx_data[2] = id;
	unsigned short sum = 0;
	int i;
	for(i = 3; i< size + 3; ++i){
		uc->tx_data[i] = data[i-3];
		sum += data[i-3];
		sum &= 0x00ff;
	}
	uc->tx_data[i++] = sum;
	uc->tx_data[i++] = TAIL_CHAR1;
	uc->tx_data[i++] = TAIL_CHAR2;
	HAL_UART_Transmit_DMA(uc->huart, uc->tx_data, i);
}





