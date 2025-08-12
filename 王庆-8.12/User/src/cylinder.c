//#include "cylinder.h"
//CyLinderStruct Cyl;
//unsigned char CYL_table[8] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00};

///**
// * @brief 使用8位二进制控制电磁阀开关
// * @param Data 从高到低依次控制1-8位；例如11111111=255为全亮，10000000=128为1亮
// */
//void Valve_Ctrl(u8 iicmsg)
//{
//    u8 i = 0;
//    if (iicmsg > 0xFF)
//        iicmsg = 0XFF;
//    for (i = 0; i < 8; i++)
//    {
//        Reset_CYLCLK(); // 时钟低电平

//        if (iicmsg & 0x01)     // 如果数据最后一位为1
//				{Set_CYLDATA(); }  // 1，移位寄存器锁存数据
//        else                 // 如果为0
//				{  Reset_CYLDATA();}// 0
//        iicmsg >>= 1;          // 右移一位
//        Set_CYLCLK();        // 时钟高电平
//    }

//    // 注意新电磁阀版需要后面两句 也就是在比赛前一天刚换的，// add by Fzh,2023-08-05 14:17,
//    Reset_CYL8CLK();
//    Set_CYL8CLK();
//}

#include "cylinder.h"
#include "main.h"

int cylinderenable = 0;


void CLK_set(){

HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
}
void CLK_reset(){
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
}
void reset_SI(){
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
	
}
void set_SI(){
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
}

void ctrl(uint8_t cmd){
uint8_t data;
	
	if(cmd>0xff)
		data=0xff;
	else
		data=cmd;		
	HAL_Delay(1);
CLK_reset();	
		set_SI();
	reset_SI();
	CLK_set();
//HAL_Delay(100);
	for(int i=0;i<4;i++){
		
		CLK_reset();


	if((data&0xff)!=(data>>1)<<1)		//0000 1010
	{	
	set_SI();
	}
	else
	{

	reset_SI();

	}


	CLK_set();
//	CLK_reset();
//	CLK_set();
data=data>>1;


	}
CLK_reset();	
		set_SI();
	reset_SI();
	CLK_set();
}
