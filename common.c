#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "common.h"


void dump_memory_u8(uint8_t *addr, int len, FILE *steam)
{

    int index=0;

    assert(addr != NULL);
    assert(steam != NULL);
    assert(len > 0);
    
    if(steam != stdout)
    {
        fwrite(addr, 1, len, steam);
    }
    
    for(index=0; index<len; index++)
    {
        fprintf(stdout, "%08d: 0x%02X\n", index, addr[index]);
    }
}
uint32_t get_bit_value_from_u32(uint32_t value,int start_bit, int end_bit)
{

    
    uint32_t return_value;
    uint32_t len = end_bit - start_bit + 1;
    uint32_t mask = 0xFFFFFFFF;
    uint32_t offset_value = value >> start_bit;
    uint32_t offset_mask = mask << len;

    assert(start_bit <= end_bit);
    
    offset_mask = ~offset_mask;
    return_value = offset_value & offset_mask;
    
    return return_value;

}

uint32_t get_bit_value_from_u128(uint32_t *addr, int start_bit, int end_bit)
{  
    uint32_t start_locate = start_bit / 32;
    uint32_t start_offset = start_bit % 32;
    uint32_t end_locate = end_bit / 32;
    uint32_t end_offset = end_bit % 32;
    uint32_t lsb = 0;
    uint32_t msb = 0;
    uint32_t value = 0;
    assert(start_bit <= end_bit);
    assert((end_bit - start_bit + 1) <= 32);

    if(start_locate == end_locate)
    {
        return get_bit_value_from_u32(addr[start_locate], start_offset, end_offset);
    }
    
    lsb = get_bit_value_from_u32(addr[start_locate], start_offset, 31);
    msb = get_bit_value_from_u32(addr[end_locate], 0,end_offset);

    value = lsb | msb << (32 - start_offset);

    return value;        
    
}


