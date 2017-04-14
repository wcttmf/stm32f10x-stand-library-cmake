#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

class Led {
 private:
  GPIO_TypeDef *family;
  uint16_t port;

 public:
  char status = 0;

  Led(uint32_t RCC_APB2Periph, GPIO_TypeDef *family, uint16_t pin);

  void change(void);

  static void callback(void *data) {
    Led *led = (Led *)data;
    led->change();
  };


};

#endif