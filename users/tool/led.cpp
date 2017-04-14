#include <led.h>

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Led::change(void) {
  if (status) {
    status = 0;
    GPIO_SetBits(family, port);
  } else {
    status = 1;
    GPIO_ResetBits(family, port);
  }
}

Led::Led(uint32_t RCC_APB2Periph, GPIO_TypeDef *family, uint16_t pin) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);

  GPIO_InitTypeDef led;

  led.GPIO_Speed = GPIO_Speed_50MHz;
  led.GPIO_Mode = GPIO_Mode_Out_PP;
  led.GPIO_Pin = this->port = pin;

  GPIO_Init(this->family = family, &led);
}
