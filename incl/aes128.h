#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint8_t aes128_en(uint8_t* in, int len, uint8_t** out);
uint8_t aes128_de(uint8_t* in, int len);
