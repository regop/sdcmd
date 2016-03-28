#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <linux/types.h>
#include <asm/types.h>
#include <sys/types.h>
#include <linux/mmc/ioctl.h>

#include "sd_cmd.h"

#define VERSION ("0.0.1")

void print_help(void);

int main(int argc, char *argv[])
{
    
    int32_t opt;
    int cmd_index = 0;
    uint32_t args=0;
    uint32_t blk_cnt = 0;
    bool need_help = false;
    
    char *endptr = NULL;
    char *cmd_index_str = NULL;
    char *args_str = NULL;
    char *blk_cnt_str = NULL;
    char *dev_path = NULL;
    char *input_file_path = NULL;
    char *output_file_path = NULL;
    bool args_valid=true;
    int ret = 0;
    
    while((opt=getopt(argc, argv, ":hc:a:n:i:o:")) != -1)
    {
        switch(opt)
        {
        
            case 'h':
                need_help = true;
                break;
            case 'c':
                cmd_index_str = optarg;
                break;
            case 'a':
                args_str = optarg;
                break;
            case 'n':
                blk_cnt_str = optarg;
                break;
            case 'i':
                input_file_path = optarg;
                break;
            case 'o':
                output_file_path = optarg;
                break;
            case ':':
                printf("option needs a value\n");
                args_valid = false;
                break;
            default:
                printf("unkown option %c\n", optopt);
                args_valid = false;
                break;
        }
    }

#if 0
    printf("cmd index string is %s\n", cmd_index_str);
    printf("args string is      %s\n", args_str);
    printf("blk_cnt string is   %s\n", blk_cnt_str);
#endif

    if(argc < 2 || need_help)
    {
        print_help();
        exit(EXIT_SUCCESS);
    }
    else if(!args_valid)
    {
        exit(EXIT_FAILURE);
    }

    if(cmd_index_str == NULL)
    {
        print_help();
        exit(EXIT_FAILURE);
    }
    else
    {
        cmd_index = strtol(cmd_index_str, &endptr, 0);
        if((cmd_index < 0) || (cmd_index > 64) ||
            strcmp(cmd_index_str, endptr) == 0)
        {
            printf("Invalid cmd index\n");
            exit(EXIT_FAILURE);
        }

    }

    if(args_str != NULL)
    {
        args = strtoll(args_str, &endptr, 0);
    }

    if(blk_cnt_str != NULL)
    {
        blk_cnt = strtoll(blk_cnt_str, &endptr, 0);
    }

    
    dev_path =  argv[optind];

#if 0
    printf("cmd index is %d\n", cmd_index);
    printf("args is      0x%08X\n", args);
    printf("blk_cnt is   %d\n", blk_cnt);
    printf("dev_path is  %s\n", dev_path);
#endif

    switch(cmd_index)
    {
        case SD_CMD_READ_SINGLE_BLK:
            ret = sd_read_single_block(dev_path, args, output_file_path);
            break;

        case SD_CMD_READ_MULTIPLE_BLK:
            ret = sd_read_multiple_block(dev_path, args, blk_cnt, output_file_path);
            break;

        case SD_CMD_SEND_STAT:
            ret = sd_send_status(dev_path, args);
            break;

        case SD_CMD_STOP_TRANS:
            ret = sd_stop_trans(dev_path);
            break;
            
        default:
            printf("unkown op code: %d\n", cmd_index);
            ret = -1;
            break;
    }
    
    return ret;
}

void print_help(void)
{
    printf("sdmd version %s\n", VERSION);
    printf("Copyright (C) 2016 by lijg\n"
           "Usage: sdcmd [options] device \n"
           "Options\n"
           " -c index      specify command index\n"
           " -o file       specify read command output file\n"
           " -a arg        specify command arg\n"
           " -i file       specify write command input file\n"
           " -n count      specify multiple read/write command block count\n"
           "\nExample\n"
           "read single block:\n"
           "./sdcmd -c 17 -a 0 -o out_file /dev/mmcblk0\n"
           "write block:\n"
           "./sdcmd -c 24 -a 0 -i input_file /dev/mmcblk0\n"
           );

}
