#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "stimer.h"

#include "oled.h"


int main(void) {
    delay_init(72);
    Led led = Led(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_13);
    OLED oled = OLED(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_8,GPIOB,GPIO_Pin_9);
    SoftwareTimer timer = SoftwareTimer();


    timer.setInterval(Led::callback, &led, 97);
    timer.setInterval(OLED::callback, &oled, 337);

    oled.print(0,0,0,"it works!");

    while (1) {
        timer.refresh();
        timer.execult();
        delay_ms(1);
    }
}
