#ifndef __HAM_BO_SUNG_H
#define __HAM_BO_SUNG_H
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include <string.h>
void ssd1306_init();
void task_ssd1306_display_text(const void *arg_text);
void task_ssd1306_display_clear(void *ignore);
void ssd1306_set_horizontal_addressing_mode();
void ssd1306_draw_128x64_image(uint8_t *image);
void ssd1306_set_page(uint8_t page);
#endif