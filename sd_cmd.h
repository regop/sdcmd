#ifndef SD_COMMON_H
#define SD_COMMON_H

#define MMC_BLOCK_MAJOR             (179)

/*MMC COMMAND DEFINATION*/
/*Class 0*/
#define MMC_GO_IDLE                 (0)
#define MMC_SEND_OP_COND            (1)
#define MMC_ALL_SEND_CID            (2)
#define MMC_SET_RCA                 (3)
#define MMC_SET_DSR                 (4)
#define MMC_SLEEP_AWAKE              (5)
#define MMC_SWITCH	                 (6)
#define MMC_SELECT                  (7)
#define MMC_SEND_EXT_CSD	         (8)
#define MMC_SEND_CSD                (9)
#define MMC_SEND_CID                (10)
#define MMC_STOP_TRANS              (12)
#define MMC_SEND_STATUS             (13)
#define MMC_BUS_TEST_R              (14)
#define MMC_GO_INACTIVE_STATE       (15)
#define MMC_BUS_TEST_W              (19)

/*Class 2*/
#define MMC_SET_BLOCKLEN            (16)
#define MMC_READ_SINGLE_BLOCK       (17)
#define MMC_READ_MULTIPLE_BLOCK     (18)
#define MMC_SEND_TUNING_BLOCK       (21)


/*Class 4*/
#define MMC_SET_BLOCK_COUNT         (23)
#define MMC_WRITE_BLOCK             (24)
#define MMC_WRITE_MULTIPLE_BLOCK    (25)
#define MMC_PROGRAM_CID             (26)
#define MMC_PROGRAM_CSD             (27)
#define MMC_SET_TIME                (49)


/*Class 5*/
#define MMC_ERASE_GROUP_START           (35)
#define MMC_ERASE_GROUP_END             (36)
#define MMC_ERASE                        (38)



/*Class 6*/
#define MMC_SET_WRITE_PROT              (28)
#define MMC_CLR_WRITE_PROT              (29)
#define MMC_SEND_WRITE_PROT             (30)
#define MMC_SEND_WRITE_PROT_TYPE        (31)

#define DEFAULT_BLOCK_SIZE      (512)
#define EXT_CSD_SIZE            (512)
#define DEFAULT_RCA             (1 << 16)
#define OCR_WITHOUT_BUSY        (0x40FF8080)


/*Class MP*/
#define MMC_CMD61            (61)




#define MMC_CMD61_BLK_CNT_MASK  (0x0000FFFF);


//copy from kernel include/linux/mmc/core.h
#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	    (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	    (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	    (MMC_RSP_PRESENT)
#define MMC_RSP_R4	    (MMC_RSP_PRESENT)
#define MMC_RSP_R5	    (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	    (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	    (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

typedef enum _write_flag
{
    MMC_DATA_READ = 0,
    MMC_DATA_WRITE
}rw_flag_t;

typedef enum _current_status
{
    MMC_CUR_STAT_IDLE = 0,
    MMC_CUR_STAT_READY = 1,
    MMC_CUR_STAT_IDENT = 2,
    MMC_CUR_STAT_STBY = 3,
    MMC_CUR_STAT_TRANS = 4,
    MMC_CUR_STAT_DATA = 5,
    MMC_CUR_STAT_RECV = 6,
    MMC_CUR_STAT_PROG = 7,
    MMC_CUR_STAT_DIS = 8,
    MMC_CUR_STAT_BTST = 9,
    MMC_CUR_STAT_SLP = 10,
    MMC_CUR_STAT_RSVD  
}current_state;

int read_ext_csd(char *dev_path);
int mmc_switch(char *dev_path, uint32_t args);
int mmc_send_status(char *dev, uint32_t args);
int mmc_select_device(char *dev_path, uint32_t args);
int mmc_send_csd(char *dev_path, uint32_t args);
int mmc_send_cid(char *dev_path, uint32_t args);
int mmc_sleep_awake(char *dev_path, uint32_t args);
int mmc_go_idle_state(char *dev_path, uint32_t args);
int mmc_send_op_cond(char *dev_path, uint32_t args);
int mmc_all_send_cid(char *dev_path, uint32_t args);
int mmc_set_rca(char *dev_path, uint32_t args);
int mmc_stop_trans(char *dev_path, uint32_t args);
int mmc_bus_test_r(char *dev_path, uint32_t args);
int mmc_go_inactive_state(char *dev_path, uint32_t args);
int mmc_read_single_block(char *dev_path, uint32_t args, char *output_file_path);
int mmc_set_block_count(char *dev_path, uint32_t args);
int mmc_read_multiple_block(char *dev_path, uint32_t args, uint32_t blk_cnt, char *output_file_path);
int mmc_write_block(char *dev_path, uint32_t args, char *file_path);
int mmc_write_multiple_block(char *dev_path, uint32_t args, uint32_t blk_cnt, char *file_path);
int mmc_erase_group_start(char *dev_path, uint32_t args);
int mmc_erase_group_end(char *dev_path, uint32_t args);
int mmc_erase(char *dev_path, uint32_t args);
int mmc_set_write_prot(char *dev_path, uint32_t args);
int mmc_clr_write_prot(char *dev_path, uint32_t args);
int mmc_send_write_prot(char *dev_path, uint32_t args);
int mmc_send_write_prot_type(char *dev_path, uint32_t args);
int mmc_program_csd(char *dev_path, uint32_t args, char *file_path);
int mmc_cmd61(char *dev_path, uint32_t args, char *file_path);


#endif
