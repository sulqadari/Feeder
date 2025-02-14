#include "stm32f1xx.h"

#define PORT GPIOA
#define SCK GPIO_Pin_5
#define MOSI GPIO_Pin_7

#define DC GPIO_Pin_3
#define RST GPIO_Pin_4
#define CE GPIO_Pin_2

void gpio_en(){
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
   
   GPIO_InitTypeDef spi_pin;
   
   //Main SPI pins
   spi_pin.GPIO_Pin = SCK | MOSI;
   spi_pin.GPIO_Speed = GPIO_Speed_50MHz;
   spi_pin.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(PORT, &spi_pin);
   
   //Other pins
   spi_pin.GPIO_Pin = DC | RST | CE;
   spi_pin.GPIO_Speed = GPIO_Speed_2MHz;
   spi_pin.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(PORT, &spi_pin);   
}

void resetOn(){
   GPIO_SetBits(PORT, RST);
}

void resetOff(){
   GPIO_ResetBits(PORT, RST);
}

void ceOn(){
   GPIO_SetBits(PORT, CE);
}

void ceOff(){
   GPIO_ResetBits(PORT, CE);
}

void dcOn(){
   GPIO_SetBits(PORT, DC);
}

void dcOff(){
   GPIO_ResetBits(PORT, DC);
}

void spi_en(){
   SPI_InitTypeDef spi;
   
   spi.SPI_Direction = SPI_Direction_1Line_Tx;
   spi.SPI_Mode = SPI_Mode_Master;
   spi.SPI_DataSize = SPI_DataSize_8b;
   
   spi.SPI_CPHA = SPI_CPHA_1Edge;
   spi.SPI_CPOL = SPI_CPOL_Low;
   spi.SPI_NSS = SPI_NSS_Soft;
   
   spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
   spi.SPI_FirstBit = SPI_FirstBit_MSB;
   
   SPI_Init(SPI1, &spi);
   SPI_Cmd(SPI1, ENABLE);
}

void spi_send(uint16_t data){
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_I2S_SendData(SPI1, data);
   
}

void chk_spi(){
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
}

void lcd_init(){
   ceOn();
   dcOn();
   resetOn();
   
   ceOff();
   resetOff();
   dcOff();
   
   spi_send(0x21);//exend COM
   spi_send(0xc1);//voltage offset
   spi_send(0x06);//temp. correction
   spi_send(0x13);//high voltage(6.42V) generator on
   spi_send(0x20);//standart COM
   spi_send(0x0c);//LCD on
   chk_spi();
   
   dcOn();
}

void lcd_data(uint16_t data){
   dcOn();
   spi_send(data);
}

void lcd_com(uint16_t data){
   dcOff();
   spi_send(data);
}

void lcd_cursor(int16_t x, int16_t y){
   if(x < 0 || x > 84 || y < 0 || y > 5) return;
   
   lcd_com(0x40 | x);
   lcd_com(0x80 | y);
   chk_spi();
}

int main(){
   gpio_en();
   spi_en();
   lcd_init();
   
   lcd_cursor(0, 0);
   
   unsigned char buff[6][84];
   
   for(char i=0; i<6; i++){
      for(char j=0; j<84; j++){
         buff[i][j] = 0x1f;
      }
   }
   
   lcd_cursor(0, 0);
   
   for(char i=0; i<6; i++){
      for(char j=0; j<84; j++){
         lcd_data(buff[i][j]);
      }
   }
   chk_spi();
   
   return 0;
}