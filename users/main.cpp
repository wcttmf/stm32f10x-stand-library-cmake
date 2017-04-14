#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "stimer.h"


typedef void (*void_func_void_t)(void *);

int main(void) {
    delay_init(72);
    Led led = Led(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_13);
    SoftwareTimer timer = SoftwareTimer();
    timer.setInterval(Led::callback, &led, 100);
    while (1) {
        timer.refresh();
        timer.execult();
        delay_ms(1);
    }
}
