#ifndef FILTER_H
#define FILTER_H

#include "bsp_system.h"

uint32_t adc_filter(uint32_t value);
void limit_value(uint32_t *data, int size, uint32_t min_val, uint32_t max_val);
int compare(const void *a, const void *b);
uint32_t mid_value(uint32_t *data, int size);
uint32_t avg_value(uint32_t *data, int size);
uint32_t adc_filter(uint32_t CurrValue);

#endif
