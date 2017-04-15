#ifndef STIMEER_H
#define STIMEER_H

#include <stdint.h>


typedef void (*callback_func_t)(void *data);

#define TIM_NUMBER 32

class SoftwareTimer {
 private:
  uint32_t offset[TIM_NUMBER] = {};
  uint32_t temp[TIM_NUMBER] = {};
  uint8_t exec[TIM_NUMBER] = {};
  callback_func_t func[TIM_NUMBER] = {};
  void *param[TIM_NUMBER] = {};

  int8_t getIdle(void);

 public:
  int8_t clear(uint8_t id);

  int8_t setInterval(callback_func_t callback, void *data, uint32_t tim);

  int8_t setTimeout(callback_func_t callback, void *data, uint32_t tim);

  void refresh(void);

  static void callback(void *timer);

  void execult(void);
};

#endif
