#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <linux/types.h>
#include <asm/types.h>
#include <linux/mmc/ioctl.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "common.h"
#include "sd_cmd.h"
#include "sd_misc.h"

#define SD_BLK_SIZE             (512)
bool issue_sd_cmd(char *dev_path, struct mmc_ioc_cmd *cmd_desc)
{
    bool result = true;
    int dev_fd = open(dev_path, O_RDONLY);
    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return false;
    }

    if(ioctl(dev_fd, MMC_IOC_CMD, cmd_desc) == -1)
    {
        perror("ioctl: ");
        result = false;
    }

    if(close(dev_fd) == -1)
    {
        perror("close device: ");
        return false;
    }

    return result;
    
}


int sd_read_single_block(char *dev_path, uint32_t args, char *out_file_path)
{

    uint32_t blk_cnt = 1;
    struct mmc_ioc_cmd cmd_desc;
    uint32_t blk_size = SD_BLK_SIZE;
    char *buf = (char *)malloc(blk_size * blk_cnt);
    FILE *out_file = NULL;
    
    if(buf == NULL)
    {
        fprintf(stderr, "-%s: %d- fail to malloc memory", __FILE__, __LINE__);
        return -ENOMEM;
    }
    
    memset(buf, 0, blk_size * blk_cnt);
    memset(&cmd_desc, 0, sizeof(cmd_desc));   

    
    cmd_desc.write_flag = SD_DATA_READ;
    cmd_desc.opcode = SD_CMD_READ_SINGLE_BLK;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = blk_size;
    cmd_desc.blocks = blk_cnt;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    if(issue_sd_cmd(dev_path, &cmd_desc))
    {
        fprintf(stdout, "0x%08X\n", cmd_desc.response[0]);

        if(out_file_path == NULL)
        {
            return 0;
        }
        else
        {
            out_file = fopen(out_file_path, "w+");
        }
        
        if(out_file)
        {
            if(fwrite(buf, 1, blk_size * blk_cnt, out_file) != blk_size)
            {
                fprintf(stderr, "error! write to file\n");
                fclose(out_file);
                return -1;
            }
            else
            {
                fclose(out_file);
                return 0;
            }
        }
        else
        {
            perror("open file: ");
            return -1;
        }
        
        return 0;
    }
    else
    {
        fprintf(stderr, "cmd error\n");
        return -1;
    }
    
    
}

int sd_read_multiple_block(char *dev_path, uint32_t args, uint32_t rd_blk_cnt, char *out_file_path)
{

    uint32_t blk_cnt = rd_blk_cnt;    
    uint32_t blk_size = SD_BLK_SIZE;
    struct mmc_ioc_cmd cmd_desc;
    char *buf = (char *)malloc(blk_size * blk_cnt);
    FILE *out_file = NULL;
    
    if(buf == NULL)
    {
        fprintf(stderr, "-%s: %d- fail to malloc memory", __FILE__, __LINE__);
        return -ENOMEM;
    }
    
    memset(buf, 0, blk_size * blk_cnt);
    memset(&cmd_desc, 0, sizeof(cmd_desc));   

    
    cmd_desc.write_flag = SD_DATA_READ;
    cmd_desc.opcode = SD_CMD_READ_MULTIPLE_BLK;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = blk_size;
    cmd_desc.blocks = blk_cnt;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    if(issue_sd_cmd(dev_path, &cmd_desc))
    {
        fprintf(stdout, "0x%08X\n", cmd_desc.response[0]);

        if(out_file_path == NULL)
        {
            return 0;
        }
        else
        {
            out_file = fopen(out_file_path, "w+");
        }
        
        if(out_file)
        {
            if(fwrite(buf, 1, blk_size * blk_cnt, out_file) != blk_size*blk_cnt)
            {
                fprintf(stderr, "error! write to file\n");
                fclose(out_file);
                return -1;
            }
            else
            {
                fclose(out_file);
                return 0;
            }
        }
        else
        {
            perror("open file: ");
            return -1;
        }
        
        return 0;
    }
    else
    {
        fprintf(stderr, "cmd error\n");
        return -1;
    }
    
    
}

int sd_send_status(char *dev_path, uint32_t args)
{

    struct mmc_ioc_cmd cmd_desc;
    memset(&cmd_desc, 0, sizeof(cmd_desc)); 
    
    cmd_desc.write_flag = SD_DATA_READ;
    cmd_desc.opcode = SD_CMD_SEND_STAT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = 0;
    cmd_desc.blocks = 0;

     if(issue_sd_cmd(dev_path, &cmd_desc))
     {
        fprintf(stdout, "0x%08X\n", cmd_desc.response[0]);
        return 0;
     }
     else
     {
        fprintf(stderr, "cmd error\n");
        return -1;
     }
}


int sd_stop_trans(char *dev_path)
{

    struct mmc_ioc_cmd cmd_desc;
    memset(&cmd_desc, 0, sizeof(cmd_desc)); 
    
    cmd_desc.write_flag = SD_DATA_READ;
    cmd_desc.opcode = SD_CMD_STOP_TRANS;
    cmd_desc.arg = 0;
    cmd_desc.flags = MMC_RSP_R1B;
    cmd_desc.blksz = 0;
    cmd_desc.blocks = 0;

     if(issue_sd_cmd(dev_path, &cmd_desc))
     {
        fprintf(stdout, "0x%08X\n", cmd_desc.response[0]);
        return 0;
     }
     else
     {
        fprintf(stderr, "cmd error\n");
        return -1;
     }
}

#if 0
static bool read_single_block(int fd, uint32_t args, int opcode, uint8_t *buf)
{
    int ret = -1;
    int dev_fd = fd;
    struct mmc_ioc_cmd cmd_desc;
        
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = opcode;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
        return false;
    }
    else
    {
        return true;
    }

}



int __send_ext_csd(int fd, uint8_t *ext_csd)
{

    int ret = 0;
    struct mmc_ioc_cmd cmd_desc;
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    memset(ext_csd, 0, sizeof(uint8_t) * EXT_CSD_SIZE);
    
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_SEND_EXT_CSD;
    cmd_desc.arg = 0;
    cmd_desc.flags = MMC_RSP_R1 ;
    cmd_desc.blksz = EXT_CSD_SIZE;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, ext_csd);

    ret = ioctl(fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
        perror("ioctl");

    return ret;
    
   
}

int read_ext_csd(char *dev_path)
{
    uint8_t buf[EXT_CSD_SIZE];
    int ret = -1;
    int dev_fd = -1;

    dev_fd = open(dev_path, O_RDONLY);
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }


    ret = __send_ext_csd(dev_fd,buf);
    
    if(ret == 0)
    {
        dump_memory_u8(buf, EXT_CSD_SIZE, stdout);
        decode_ext_csd(buf, stdout);
    }

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;

}

int __mmc_write_ext_csd(int fd, uint32_t index, uint32_t value)
{
    int ret = 0;
    uint32_t args = (3 << 24) | (index << 16) | (value << 8);
    printf("args is 0x%08X\n", args);
    
    struct mmc_ioc_cmd cmd_desc;
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    
    cmd_desc.write_flag = MMC_DATA_WRITE;
    cmd_desc.opcode = MMC_SWITCH;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B ;

    ret = ioctl(fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
    }
    
#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    return ret;
}



int mmc_switch(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;

    
    dev_fd = open(dev_path, O_RDONLY);
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }

    uint8_t buf[64];    
    memset(buf, 0x0, 64);
    
    struct mmc_ioc_cmd cmd_desc;
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_SWITCH;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = 64;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, buf);


    dump_cmd_desc(&cmd_desc, stdout);
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);

    if (ret)
    {
        perror("ioctl");
    }

    dump_memory_u8(buf, 64, stdout);
    
    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}

static bool __get_device_status(int fd, uint32_t args, uint32_t *status)
{
    int ret = -1;
    struct mmc_ioc_cmd cmd_desc;
    if(args == 0)
    {
        args = DEFAULT_RCA;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));    
    cmd_desc.opcode = MMC_SEND_STATUS;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    
    ret = ioctl(fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
        return false;
    }
    else
    {
        memcpy(status,&cmd_desc.response[0],sizeof(uint32_t));
        return true;
    }    
    
}

int mmc_send_status(char *dev_path, uint32_t args)
{
    bool ret = false;
    int dev_fd = -1;
    uint32_t status = 0;
    
    dev_fd = open(dev_path, O_RDONLY);    

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }   
    
    ret = __get_device_status(dev_fd, args, &status);

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    if(ret)
    {
        decode_device_status(&status);
        return 0;
    }
    else
    {
        return -EIO;        
    }

}


int mmc_select_device(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);
    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_SELECT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;    
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
    }
    
    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}

int mmc_send_csd(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc={0};

    if(args == 0)
    {
        args = DEFAULT_RCA;
    }
    
    dev_fd = open(dev_path, O_RDONLY);

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));    
    cmd_desc.opcode = MMC_SEND_CSD;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R2;
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        decode_csd(&cmd_desc.response[0], stdout);
    }

#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}

int mmc_send_cid(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc = {0};

    if(args == 0)
    {
        args = DEFAULT_RCA;
    }
    
    dev_fd = open(dev_path, O_RDONLY);

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
        
    memset(&cmd_desc, 0, sizeof(cmd_desc));    
    cmd_desc.opcode = MMC_SEND_CID;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R2;
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
    }
        
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}

int mmc_sleep_awake(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }

    memset(&cmd_desc, 0, sizeof(cmd_desc));    
    cmd_desc.opcode = MMC_SLEEP_AWAKE;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        if(CHKBIT(args,BIT(15)))
        {
            printf("Sleep\n");
        }
        else
        {
            printf("Awake\n");
        }
    }
        
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
    
     
}

int mmc_go_idle_state(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_GO_IDLE;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_NONE;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
      
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
    
     
}


int mmc_send_op_cond(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc; 
    
    dev_fd = open(dev_path, O_RDONLY);
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }

    /*
    if(args == 0)
    {
        args = OCR_WITHOUT_BUSY;
    }
    */

    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_SEND_OP_COND;
    cmd_desc.arg = args;
    cmd_desc.flags = 0xE1;
    //cmd_desc.cmd_timeout_ms = 1000;
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);    
    if (ret)
    {
        perror("ioctl");
    }   
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
    
     
}

int mmc_all_send_cid(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    dev_fd = open(dev_path, O_RDONLY);

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_ALL_SEND_CID;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R2;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }   
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
    
     
}

int mmc_set_rca(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    dev_fd = open(dev_path, O_RDONLY);

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }

    if(args == 0)
    {
        args = DEFAULT_RCA;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_SET_RCA;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }    
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;    
     
}

int mmc_stop_trans(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    dev_fd = open(dev_path, O_RDONLY);

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_STOP_TRANS;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }    
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;    
}

int mmc_bus_test_r(char *dev_path, uint32_t args)
{

    int ret = -1;
    // ddr mode
 #if 0
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    dev_fd = open(dev_path, O_RDONLY);
    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));
    cmd_desc.opcode = MMC_BUS_TEST_R;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    mmc_ioc_cmd_set_data(cmd_desc, ext_csd);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }    
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
#endif
    return ret;    
}


int mmc_go_inactive_state(char *dev_path, uint32_t args)
{
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;

    if(args == 0)
    {
        args = DEFAULT_RCA;
    }
    
    dev_fd = open(dev_path, O_RDONLY);    

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));    
    cmd_desc.opcode = MMC_GO_INACTIVE_STATE;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        printf("GO INACTIVE STATE\n");
    }
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}

static bool __read_single_block(int fd, uint32_t args, int opcode, uint8_t *buf)
{
    int ret = -1;
    int dev_fd = fd;
    struct mmc_ioc_cmd cmd_desc;
        
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = opcode;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
        return false;
    }
    else
    {
        return true;
    }

}



int mmc_read_single_block(char *dev_path, uint32_t args, char *output_file_path)
{
    int ret = 0;
    bool result = false;
    int dev_fd = -1;
    FILE *out_stream = NULL; 
    
    dev_fd = open(dev_path, O_RDONLY); 
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }

    if(output_file_path == NULL)
    {
        out_stream = stdout;
    }
    else
    {
        out_stream = fopen(output_file_path, "w+");
        if(out_stream == NULL)
        {
            printf("Fail to open %s\n", output_file_path);
            close(dev_fd);
            return -EINVAL;
        }
    }
    
    uint8_t buf[DEFAULT_BLOCK_SIZE];
    
    memset(buf, 0, DEFAULT_BLOCK_SIZE);
    result = __read_single_block(dev_fd, args, MMC_READ_SINGLE_BLOCK, buf);

    if(close(dev_fd))
    {
        perror("close device: ");
    }

    if (result)
    {
        dump_memory_u8(buf,DEFAULT_BLOCK_SIZE, out_stream);
        ret = 0;
    }
    else
    {
        ret = -EIO;
    }

    if(fclose(out_stream))
    {
        perror("close file: ");
    }

    return ret;

}

int mmc_set_block_count(char *dev_path, uint32_t args)
{    
    int ret = -1;
    int dev_fd = -1;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    

    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
   
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.opcode = MMC_SET_BLOCK_COUNT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif
    
    if(close(dev_fd))
    {
        perror("close device: ");
    }

    return ret;
}


int mmc_read_multiple_block(char *dev_path, uint32_t args, uint32_t blk_cnt, char *output_file_path)
{
    int ret = 0;
    int dev_fd = -1;
    FILE *out_stream = NULL;
    struct mmc_ioc_cmd cmd_desc;

    if(blk_cnt < 1)
    {
        printf("Please specify block count using \'n\' option\n");
        return -EINVAL;        
    }
    
    if(output_file_path == NULL)
    {
        out_stream = stdout;
    }
    else
    {
        out_stream = fopen(output_file_path, "w+");
        if(out_stream == NULL)
        {
            printf("Fail to open %s\n", output_file_path);
            return -EINVAL;
        }
    }

    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -1;
    }
    
    uint8_t *buf = (uint8_t *)malloc(DEFAULT_BLOCK_SIZE * blk_cnt);   
    memset(buf, 0, DEFAULT_BLOCK_SIZE * blk_cnt);           
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_READ_MULTIPLE_BLOCK;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = blk_cnt;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        dump_memory_u8(buf,DEFAULT_BLOCK_SIZE * blk_cnt, out_stream);
    }

#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    fclose(out_stream);

    free(buf);
    
    if(close(dev_fd))
    {
        perror("close device: ");
    }
    
    return ret;
}



int mmc_write_block(char *dev_path, uint32_t args, char *file_path)
{
    int ret = 0;
    int dev_fd = 0;
    int file_fd = 0;
    char *buf = NULL;
    struct mmc_ioc_cmd cmd_desc;
    struct stat file_stat;


    if(file_path == NULL)
    {
        printf("Please specify input file using \'i\' option\n");
        return -EINVAL;
    }
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 


    
    file_fd = open(file_path, O_RDONLY);
    if(file_fd == -1)
    {
        perror("open file: ");
        return -EINVAL;
    }
    
    fstat(file_fd, &file_stat);

    if(file_stat.st_size < DEFAULT_BLOCK_SIZE)
    {
        printf("%s is too small to write\n",file_path);
        
        if(close(file_fd))
            perror("close file: ");

        if(close(dev_fd))
            perror("close device: ");
            
        return -EINVAL;
    }

    buf = mmap(NULL, DEFAULT_BLOCK_SIZE, PROT_READ, MAP_SHARED, file_fd ,0);
    if(buf == MAP_FAILED)
    {
        return -EINVAL;
    }
         
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_WRITE;
    cmd_desc.opcode = MMC_WRITE_BLOCK;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    if(close(file_fd))
    {
        perror("close deivce");
    }
    
#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    return ret;
}

int mmc_write_multiple_block(char *dev_path, uint32_t args, uint32_t blk_cnt, char *file_path)
{
    int ret = 0;
    int dev_fd = 0;
    int file_fd = 0;
    char *buf = NULL;
    struct mmc_ioc_cmd cmd_desc;
    struct stat file_stat;

    if(blk_cnt < 1)
    {
        printf("Please specify block count using \'n\' option\n");
        return -EINVAL;        
    }

    if(file_path == NULL)
    {
        printf("Please specify input file using \'i\' option\n");
        return -EINVAL;
    }
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 

    
    file_fd = open(file_path, O_RDONLY);
    if(file_fd == -1)
    {
        perror("open file: ");
        return -EINVAL;
    }
    
    fstat(file_fd, &file_stat);

    if(file_stat.st_size < (DEFAULT_BLOCK_SIZE * blk_cnt))
    {
        printf("%s is too small to write\n",file_path);
        
        if(close(file_fd))
            perror("close file: ");

        if(close(dev_fd))
            perror("close device: ");
            
        return -EINVAL;
    }

    
    buf = mmap(NULL, (DEFAULT_BLOCK_SIZE * blk_cnt), PROT_READ, MAP_SHARED, file_fd ,0);
    if(buf == MAP_FAILED)
    {
        return -EINVAL;
    }
         
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_WRITE;
    cmd_desc.opcode = MMC_WRITE_MULTIPLE_BLOCK;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = blk_cnt;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    if(close(file_fd))
    {
        perror("close file");
    }
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    return ret;
}

int mmc_erase_group_start(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_ERASE_GROUP_START;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif  

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;

}


int mmc_erase_group_end(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_ERASE_GROUP_END;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_erase(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_ERASE;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_set_write_prot(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_SET_WRITE_PROT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_clr_write_prot(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_CLR_WRITE_PROT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_send_write_prot(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    uint8_t buf[DEFAULT_BLOCK_SIZE];
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 

    memset(buf, 0xFF, DEFAULT_BLOCK_SIZE);
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_SEND_WRITE_PROT;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;
    mmc_ioc_cmd_set_data(cmd_desc, buf);

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        dump_memory_u8(buf, DEFAULT_BLOCK_SIZE, stdout);
    }

#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_send_write_prot_type(char *dev_path, uint32_t args)
{
    
    int ret = 0;
    int dev_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    uint8_t buf[DEFAULT_BLOCK_SIZE];
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 

    memset(buf, 0xFF, DEFAULT_BLOCK_SIZE);
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_READ;
    cmd_desc.opcode = MMC_SEND_WRITE_PROT_TYPE;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;    
    mmc_ioc_cmd_set_data(cmd_desc, buf);
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        dump_memory_u8(buf, DEFAULT_BLOCK_SIZE, stdout);
    }
    
#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    return ret;
}


int mmc_program_csd(char *dev_path, uint32_t args, char *file_path)
{
    
    int ret = 0;
    int dev_fd = 0;
    int file_fd = 0;
    struct mmc_ioc_cmd cmd_desc;
    struct stat file_stat;
    uint8_t *buf;

    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 

    file_fd = open(file_path, O_RDONLY);

    if(file_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    }
    
    fstat(file_fd, &file_stat);

    if(file_stat.st_size < DEFAULT_BLOCK_SIZE)
    {
        printf("%s is too small to write\n",file_path);
        
        if(close(file_fd))
            perror("close file: ");

        if(close(dev_fd))
            perror("close device: ");
            
        return -EINVAL;
    }
    
    buf = mmap(NULL, DEFAULT_BLOCK_SIZE, PROT_READ, MAP_SHARED, file_fd ,0);
    if(buf == MAP_FAILED)
    {
        return -EINVAL;
    }
    
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_WRITE;
    cmd_desc.opcode = MMC_PROGRAM_CSD;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1B;   
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = 1;
    mmc_ioc_cmd_set_data(cmd_desc, buf);
    
    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }
    else
    {
        dump_memory_u8(buf, DEFAULT_BLOCK_SIZE, stdout);
    }
    
#ifdef DEBUG
        printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
        printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
        printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
        printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    if(close(file_fd))
    {
        perror("close file");
    }
    return ret;
}


int mmc_cmd61(char *dev_path, uint32_t args, char *file_path)
{
    int ret = 0;
    int dev_fd = 0;
    int file_fd = 0;
    char *buf = NULL;
    uint32_t blk_cnt = args & MMC_CMD61_BLK_CNT_MASK;
    
    struct mmc_ioc_cmd cmd_desc;
    struct stat file_stat;


    if(file_path == NULL)
    {
        printf("Please specify input file using \'i\' option\n");
        return -EINVAL;
    }
    
    dev_fd = open(dev_path, O_RDONLY);    
    if(dev_fd == -1)
    {
        perror("open device: ");
        return -EINVAL;
    } 

    file_fd = open(file_path, O_RDONLY);
    if(file_fd == -1)
    {
        perror("open file: ");
        return -EINVAL;
    }
    
    fstat(file_fd, &file_stat);

    if(file_stat.st_size < (DEFAULT_BLOCK_SIZE * blk_cnt))
    {
        printf("%s is too small to write\n",file_path);
        
        if(close(file_fd))
            perror("close file: ");

        if(close(dev_fd))
            perror("close device: ");
            
        return -EINVAL;
    }

    
    buf = mmap(NULL, (DEFAULT_BLOCK_SIZE * blk_cnt), PROT_READ, MAP_SHARED, file_fd ,0);
    if(buf == MAP_FAILED)
    {
        return -EINVAL;
    }
         
    memset(&cmd_desc, 0, sizeof(cmd_desc));   
    cmd_desc.write_flag = MMC_DATA_WRITE;
    cmd_desc.opcode = MMC_CMD61;
    cmd_desc.arg = args;
    cmd_desc.flags = MMC_RSP_R1;
    cmd_desc.blksz = DEFAULT_BLOCK_SIZE;
    cmd_desc.blocks = blk_cnt;
    mmc_ioc_cmd_set_data(cmd_desc, buf);
    
#ifdef DEBUG
    dump_cmd_desc(&cmd_desc, stdout);
#endif

    ret = ioctl(dev_fd, MMC_IOC_CMD, &cmd_desc);
    
    if (ret)
    {
        perror("ioctl");
    }

    if(close(dev_fd))
    {
        perror("close deivce");
    }

    if(close(file_fd))
    {
        perror("close file");
    }
    
#ifdef DEBUG
    printf("response 0 is 0x%08X\n", cmd_desc.response[0]);
    printf("response 1 is 0x%08X\n", cmd_desc.response[1]); 
    printf("response 2 is 0x%08X\n", cmd_desc.response[2]); 
    printf("response 3 is 0x%08X\n", cmd_desc.response[3]); 
#endif

    return ret;
}

#endif
