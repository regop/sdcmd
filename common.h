#ifndef _COMMON_H
#define _COMMON_H

#define BIT(x)              ((uint32_t)1 << (x))
#define CHKBIT(val,bit)     ((val)&(bit))

void dump_memory_u8(uint8_t *addr, int len, FILE *steam);
uint32_t get_bit_value_from_u32(uint32_t value,int start_bit, int end_bit);
uint32_t get_bit_value_from_u128(uint32_t *addr, int start_bit, int end_bit);

#endif
