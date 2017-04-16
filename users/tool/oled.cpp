

#include "delay.h"
#include "oledfont.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include <stdint.h>
#include "oled.h"

#define DOT0(y, x) buffer[y / 8][x] &= (uint8_t)(~(1 << y % 8))
#define DOT1(y, x) buffer[y / 8][x] |= (1 << y % 8)

void OLED::IIC_Start() {
  GPIO_SetBits(GPIOx_SCL, Pin_SCL);
  GPIO_SetBits(GPIOx_SDA, Pin_SDA);
  GPIO_ResetBits(GPIOx_SDA, Pin_SDA);
  GPIO_ResetBits(GPIOx_SCL, Pin_SCL);
}

void OLED::IIC_Stop() {
  GPIO_SetBits(GPIOx_SCL, Pin_SCL);
  GPIO_ResetBits(GPIOx_SDA, Pin_SDA);
  GPIO_SetBits(GPIOx_SDA, Pin_SDA);
}

void OLED::IIC_Wait_Ack() {
  GPIO_SetBits(GPIOx_SCL, Pin_SCL);
  GPIO_ResetBits(GPIOx_SCL, Pin_SCL);
}

void OLED::Write_IIC_Byte(uint8_t IIC_Byte) {
  uint8_t i, m, da;
  da = IIC_Byte;
  GPIO_ResetBits(GPIOx_SCL, Pin_SCL);
  for (i = 0; i < 8; i++) {
    m = da;
    m = (uint8_t)(m & 0x80);
    if (m == 0x80) {
      GPIO_SetBits(GPIOx_SDA, Pin_SDA);
    } else
      GPIO_ResetBits(GPIOx_SDA, Pin_SDA);
    da = da << 1;
    GPIO_SetBits(GPIOx_SCL, Pin_SCL);
    GPIO_ResetBits(GPIOx_SCL, Pin_SCL);
  }
}

void OLED::OLED_Set_Pos(uint8_t x, uint8_t y) {
  Write_IIC_Command((uint8_t)(0xb0 + y));
  Write_IIC_Command((uint8_t)(((x & 0xf0) >> 4) | 0x10));
  Write_IIC_Command((uint8_t)(x & 0x0f));
}

void OLED::Write_IIC_Data(uint8_t IIC_Data) {
  IIC_Start();
  Write_IIC_Byte(0x78);
  IIC_Wait_Ack();
  Write_IIC_Byte(0x40);
  IIC_Wait_Ack();
  Write_IIC_Byte(IIC_Data);
  IIC_Wait_Ack();
  IIC_Stop();
}

void OLED::Write_IIC_Command(uint8_t IIC_Command) {
  IIC_Start();
  Write_IIC_Byte(0x78);
  IIC_Wait_Ack();
  Write_IIC_Byte(0x00);
  IIC_Wait_Ack();
  Write_IIC_Byte(IIC_Command);
  IIC_Wait_Ack();
  IIC_Stop();
}

void OLED::dot(uint8_t y, uint8_t x, uint8_t color) {
  if (color)
    DOT1(y, x);
  else
    DOT0(y, x);
}

void OLED::line(uint8_t x0, uint8_t y0, uint8_t x1,
                                     uint8_t y1, uint8_t color) {}

void OLED::print(uint8_t line, uint8_t x, uint8_t color,
                                      char *str) {
  int t = 0;
  while (*str != '\0') {
    for (t = 0; t < 6; ++t) {
      if (color)
        buffer[line][(x + t)] = (uint8_t)(~F6x8[*str - ' '][t]);
      else
        buffer[line][(x + t)] = F6x8[*str - ' '][t];
    }
    str++;
    x += 6;
  }
}

void OLED::print(uint8_t line, uint8_t y, uint8_t color,
                                      uint8_t *str) {
  print(line, y, color, (char *)str);
}

void OLED::setBuffer(uint8_t line, uint8_t y,
                                          uint8_t value) {
  buffer[line][y] = value;
}

void OLED::commit(void) {
  uint8_t line, y;
  for (line = 0; line < 8; line++) {
    Write_IIC_Command((uint8_t)(0xb0 + line));
    Write_IIC_Command(0x00);
    Write_IIC_Command(0x10);
    for (y = 0; y < 128; y++) Write_IIC_Data(buffer[line][y]);
  }
}

void OLED::clear() {
  uint8_t line, y;
  for (line = 0; line < 8; line++) {
    for (y = 0; y < 128; y++) buffer[line][y] = 0;
  }
}

OLED::OLED(
    uint32_t RCC_APB2Periph_GPIOx, GPIO_TypeDef *GPIOx_scl, uint16_t Pin_scl,
    GPIO_TypeDef *GPIOx_sda, uint16_t Pin_sda) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

  GPIO_InitTypeDef i2c;
  i2c.GPIO_Mode = GPIO_Mode_Out_PP;
  i2c.GPIO_Speed = GPIO_Speed_50MHz;
  i2c.GPIO_Pin = Pin_scl;
  GPIO_Init(GPIOx_scl, &i2c);

  i2c.GPIO_Mode = GPIO_Mode_Out_PP;
  i2c.GPIO_Speed = GPIO_Speed_50MHz;
  i2c.GPIO_Pin = Pin_sda;
  GPIO_Init(GPIOx_sda, &i2c);

  GPIOx_SDA = GPIOx_sda;
  GPIOx_SCL = GPIOx_scl;
  Pin_SDA = Pin_sda;
  Pin_SCL = Pin_scl;

  delay_ms(100);

  Write_IIC_Command(0xAE);  //--display off
  Write_IIC_Command(0x00);  //---set low column address
  Write_IIC_Command(0x10);  //---set high column address
  Write_IIC_Command(0x40);  //--set start line address
  Write_IIC_Command(0xB0);  //--set page address
  Write_IIC_Command(0x81);  // contract control
  Write_IIC_Command(0xFF);  //--128
  Write_IIC_Command(0xA1);  // set segment remap
  Write_IIC_Command(0xA6);  //--normal / reverse
  Write_IIC_Command(0xA8);  //--set multiplex ratio(1 to 64)
  Write_IIC_Command(0x3F);  //--1/32 duty
  Write_IIC_Command(0xC8);  // Com scan direction
  Write_IIC_Command(0xD3);  //-set display offset
  Write_IIC_Command(0x00);  //

  Write_IIC_Command(0xD5);  // set osc division
  Write_IIC_Command(0x80);  //

  Write_IIC_Command(0xD8);  // set area color mode off
  Write_IIC_Command(0x05);  //

  Write_IIC_Command(0xD9);  // Set Pre-Charge Period
  Write_IIC_Command(0xF1);  //

  Write_IIC_Command(0xDA);  // set com pin configuartion
  Write_IIC_Command(0x12);  //

  Write_IIC_Command(0xDB);  // set Vcomh
  Write_IIC_Command(0x30);  //

  Write_IIC_Command(0x8D);  // set charge pump enable
  Write_IIC_Command(0x14);  //

  Write_IIC_Command(0xAF);  //--turn on oled panel

  clear();
}

