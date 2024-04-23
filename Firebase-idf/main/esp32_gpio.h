#ifndef __ESP32_GPIO_H
#define __ESP32_GPIO_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <stdint.h>

void gpio_set_output_mode(uint8_t pinNum);
void Hello();
#endif