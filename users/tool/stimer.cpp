#include "stimer.h"

int8_t SoftwareTimer::getIdle() {
  uint8_t id = 0;
  while (offset[id] != 0 || temp[id] != 0 || id >= TIM_NUMBER) id++;
  if (id >= TIM_NUMBER) return -1;
  return id;
}

int8_t SoftwareTimer::clear(uint8_t id) {
  if (id >= TIM_NUMBER) return 1;
  offset[id] = 0;
  temp[id] = 0;
  func[id] = (void (*)(void *))0;
  param[id] = (void *)0;
  return 0;
}

int8_t SoftwareTimer::setInterval(callback_func_t callback, void *data,
                                  uint32_t tim) {
  int8_t id;
  id = getIdle();
  if (id < 0) return id;

  param[id] = data;
  func[id] = callback;

  offset[id] = tim;
  temp[id] = tim;
  return id;
}

int8_t SoftwareTimer::setTimeout(callback_func_t callback, void *data,
                                 uint32_t tim) {
  int8_t id;
  id = getIdle();
  if (id < 0) return id;

  param[id] = data;
  func[id] = callback;

  offset[id] = 0;
  temp[id] = tim;
  return id;
}

void SoftwareTimer::refresh() {
  uint8_t id = 0;
  for (id = 0; id < TIM_NUMBER; ++id) {
    if (offset[id] || temp[id]) {
      if (temp[id] == 0 || --temp[id] == 0) {
        exec[id]++;
        temp[id] = offset[id];
      }
    }
  }
}

void SoftwareTimer::callback(void *timer) {
  SoftwareTimer *timer1 = (SoftwareTimer *)timer;
  timer1->refresh();
}

void SoftwareTimer::execult(void) {
  uint8_t id = 0;
  for (id = 0; id < TIM_NUMBER; id++)
    if (exec[id]) {
      exec[id]--;
      if (func[id]) func[id](param[id]);
    }
}