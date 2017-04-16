//
// Created by lsy on 17-4-15.
//

#ifndef OLED_H
#define OLED_H

#include <stdint.h>

class OLED {
private:
    GPIO_TypeDef *GPIOx_SDA;
    GPIO_TypeDef *GPIOx_SCL;
    uint16_t Pin_SDA;
    uint16_t Pin_SCL;
    uint8_t buffer[8][128];

    void IIC_Start(void);

    void IIC_Stop(void);

    void IIC_Wait_Ack(void);

    void Write_IIC_Byte(uint8_t IIC_Byte);

    void Write_IIC_Command(uint8_t IIC_Command);

    void Write_IIC_Data(uint8_t IIC_Data);

    void OLED_Set_Pos(uint8_t x, uint8_t y);

public:
    OLED(uint32_t RCC_APB2Periph_GPIOx,
                              GPIO_TypeDef *GPIOx_scl, uint16_t Pin_scl,
                              GPIO_TypeDef *GPIOx_sda, uint16_t Pin_sda);

    void clear(void);

    void dot(uint8_t x, uint8_t y, uint8_t color);

    void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

    void print(uint8_t line, uint8_t y, uint8_t color, char *str);

    void print(uint8_t line, uint8_t y, uint8_t color, uint8_t *str);

    void setBuffer(uint8_t line, uint8_t y, uint8_t value);

    void commit(void);

    static void callback(void *oled){
        OLED *o = (OLED *)oled;
        o->commit();
    };
};

#endif
