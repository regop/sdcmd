#ifndef SD_COMMON_H
#define SD_COMMON_H

#define MMC_BLOCK_MAJOR             (179)

/*SD COMMAND DEFINATION*/
#define SD_CMD_SWITCH                   (6)
#define SD_CMD_SELECT                   (7)
#define SD_CMD_SEND_CSD                 (8)
#define SD_CMD_STOP_TRANS               (12)
#define SD_CMD_SEND_STAT                (13)
#define SD_CMD_GO_INACT                 (15)

#define SD_CMD_SEND_BLK_LEN             (16)
#define SD_CMD_READ_SINGLE_BLK          (17)
#define SD_CMD_READ_MULTIPLE_BLK        (18)
#define SD_CMD_SEND_TUNING_PATTERN      (19)

#define SD_CMD_SPD_CLASS_CTRL           (20)
#define SD_CMD_SET_BLK_CNT              (23)
#define SD_CMD_WRITE_BLK                (24)
#define SD_CMD_WRITE_MULTIPLE           (25)
#define SD_CMD_PORG_CSD                 (26)

#define SD_CMD_ERASE_WR_BLK_START       (32)
#define SD_CMD_ERASE_WR_BLK_END         (33)
#define SD_CMD_ERASE                    (38)
#define SD_CMD_LOCK                     (42)

#define SD_CMD_APP_CMD                  (55)
#define SD_CMD_GEN_CMD                  (56)

#define SD_ACMD_SET_BUS_WIDTH           (6)
#define SD_ACMD_SEND_SD_STAT            (13)
#define SD_ACMD_SEND_NUM_WR_BLK         (22)
#define SD_ACMD_SET_WR_BLK_ERASE_CNT    (23)
#define SD_ACMD_SEND_SCR                (51)


//copy from kernel include/linux/mmc/core.h
#define MMC_RSP_PRESENT	    (1 << 0)
#define MMC_RSP_136	        (1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	        (1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	    (1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	    (1 << 4)		/* response contains opcode */

#define MMC_RSP_NONE	    (0)
#define MMC_RSP_R1	        (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	        (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	        (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	        (MMC_RSP_PRESENT)
#define MMC_RSP_R4	        (MMC_RSP_PRESENT)
#define MMC_RSP_R5	        (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	        (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	        (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

typedef enum _write_flag
{
    SD_DATA_READ = 0,
    SD_DATA_WRITE
}rw_flag_t;

typedef enum _current_status
{
    SD_CUR_STAT_IDLE = 0,
    SD_CUR_STAT_READY = 1,
    SD_CUR_STAT_IDENT = 2,
    SD_CUR_STAT_STBY = 3,
    SD_CUR_STAT_TRANS = 4,
    SD_CUR_STAT_DATA = 5,
    SD_CUR_STAT_RECV = 6,
    SD_CUR_STAT_PROG = 7,
    SD_CUR_STAT_DIS = 8,
    SD_CUR_STAT_BTST = 9,
    SD_CUR_STAT_SLP = 10,
    SD_CUR_STAT_RSVD  
}current_state;

int sd_read_single_block(char *dev_path, uint32_t args, char *out_file_path);
int sd_read_multiple_block(char *dev_path, uint32_t args, uint32_t blk_cnt, char *out_file_path);
int sd_send_status(char *dev_path, uint32_t args);
int sd_stop_trans(char *dev_path);

#endif

