#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <asm/types.h>
#include <linux/types.h>
#include <linux/mmc/ioctl.h>

#include "sd_misc.h"
#include "common.h"
#include "sd_cmd.h"

void dump_cmd_desc(struct mmc_ioc_cmd *cmd_desc, FILE* stream)
{
    fprintf(stream, "write flag: %d\n", cmd_desc->write_flag);
    fprintf(stream, "is_acmd: %d\n", cmd_desc->is_acmd);
    fprintf(stream, "opcode: %d\n", cmd_desc->opcode);
    fprintf(stream, "args: 0x%08X\n", cmd_desc->arg);
    fprintf(stream, "flags: 0x%08X\n", cmd_desc->flags);
    fprintf(stream, "blksz: %d\n", cmd_desc->blksz);
    fprintf(stream, "blocks: %d\n", cmd_desc->blocks);
}

void decode_device_status(uint32_t *dev_status)
{

    device_status_t status = {0};
    memcpy(&status, dev_status, sizeof(device_status_t));    
    printf("\nDevice Status\n");
    printf("ADDRESS_OUT_OF_RANGE:   %d\n", status.addr_out_of_range);
    printf("ADDRESS_MISALIGN:       %d\n", status.addr_misalign);
    printf("BLOCK_LEN_ERROR:        %d\n", status.blk_len_error);
    printf("ERASE_SEQ_ERROR:        %d\n", status.erase_seq_error);
    printf("ERASE_PARAM:            %d\n", status.erase_param);
    printf("WP_VIOLATION:           %d\n", status.wp_violation);
    printf("DEVICE_IS_LOCKED:       %d\n", status.dev_is_locked);
    printf("LOCK_UNLOCK_FAILED:     %d\n", status.lock_unlock_fail);
    printf("COM_CRC_ERROR:          %d\n", status.cmd_crc_error);
    printf("ILLEGAL_COMMAND:        %d\n", status.illegal_cmd);
    printf("DEVICE_ECC_FAILED:      %d\n", status.dev_ecc_fail);
    printf("CC_ERROR:               %d\n", status.cc_error);
    printf("ERROR:                  %d\n", status.error);
    printf("CID/CSD_OVERWRITE:      %d\n", status.cid_csd_over_write);
    printf("WP_ERASE_SKIP:          %d\n", status.wp_erase_skip);
    printf("ERASE_RESET:            %d\n", status.erase_reset);
    printf("CURRENT_STATE:          %d", status.current_state);
    
    switch(status.current_state)
    {
        case 0:
            printf("(Idle)\n");
            break;
        case 1:
            printf("(Ready)\n");
            break;
        case 2:
            printf("(Ident)\n");
            break;
        case 3:
            printf("(Stby)\n");
            break;
        case 4:
            printf("(Tran)\n");
            break;
        case 5:
            printf("(Data)\n");
            break;
        case 6:
            printf("(Recv)\n");
            break;
        case 7:
            printf("(Prg)\n");
            break;
        case 8:
            printf("(Dis)\n");
            break;
        case 9:
            printf("(Btst)\n");
            break;
        case 10:
            printf("(Slp)\n");
            break;
        default:
            printf("(Reserved)\n");
            break;
    }
    printf("READY_FOR_DATA:         %d\n", status.rdy_for_data);
    printf("SWITCH_ERROR:           %d\n", status.switch_error);
    printf("EXCEPTION_EVENT:        %d\n", status.except_event);    
    printf("APP_CMD:                %d\n", status.app_cmd);
    printf("APP_SPEC_CMD_RVSD:      %d\n", status.app_spec);
    printf("MANU_TestMode_RVSD:     %d\n", status.manu_test_mode);
    
}

#if 0
void decode_csd(uint32_t *resp, FILE* stream)
{

    FILE *fp;
    uint8_t buf[DEFAULT_BLOCK_SIZE];
    uint32_t csd[4];
    csd[3] = resp[0];
    csd[2] = resp[1];
    csd[1] = resp[2];
    csd[0] = resp[3];

    memset(buf, 0, DEFAULT_BLOCK_SIZE);
    fp = fopen("csd.bin", "w+");
    if(fp != NULL)
    {
        memcpy(buf, (uint8_t *)csd, 16);
        fwrite(buf, 1, DEFAULT_BLOCK_SIZE, fp);
        fclose(fp);
    }
    
    dump_memory_u8((uint8_t *)csd, 16, stream);

    
    uint32_t csd_read_bl_len = get_bit_value_from_u128(csd,80,83);
    double block_len = pow(2, csd_read_bl_len);
    
    uint32_t csd_c_size_mult = get_bit_value_from_u128(csd,47,49);
    double c_size_mult = pow(2, csd_c_size_mult + 2);

    uint32_t csd_c_size  = get_bit_value_from_u128(csd,62,73);

    fprintf(stream, "-----------CSD Register-----------\n");
    fprintf(stream, "CSD_STRUCTURE:          %d\n", get_bit_value_from_u128(csd, 126, 127));
    fprintf(stream, "SPEC_VERS:              %d\n", get_bit_value_from_u128(csd, 122, 125));
    fprintf(stream, "RSVD:                   %d\n", get_bit_value_from_u128(csd, 120, 121));
    fprintf(stream, "TAAC:                   %d\n", get_bit_value_from_u128(csd, 112, 119));
    fprintf(stream, "NSAC:                   %d\n", get_bit_value_from_u128(csd,104, 111));
    fprintf(stream, "TRAN_SPEED:             0x%X\n", get_bit_value_from_u128(csd,96,103));
    fprintf(stream, "CCC:                    0x%X\n", get_bit_value_from_u128(csd,84,95));
    fprintf(stream, "READ_BL_LEN:            0x%X (%1.0lf bytes)\n", csd_read_bl_len, block_len);
    fprintf(stream, "READ_BL_PARTIAL:        %d\n", get_bit_value_from_u128(csd,79,79));
    fprintf(stream, "WRITE_BLK_MISALIGN:     %d\n", get_bit_value_from_u128(csd,78,78));
    fprintf(stream, "READ_BLK_MISALIGN:      %d\n", get_bit_value_from_u128(csd,77,77));
    fprintf(stream, "DSR_IMP:                %d\n", get_bit_value_from_u128(csd,76,76));
    fprintf(stream, "RSVD:                   %d\n", get_bit_value_from_u128(csd, 74, 75));
    fprintf(stream, "C_SIZE:                 0x%X\n", csd_c_size);
    fprintf(stream, "VDD_R_CURR_MIN:         0x%X\n", get_bit_value_from_u128(csd,59,61));   
    fprintf(stream, "VDD_R_CURR_MAX:         0x%X\n", get_bit_value_from_u128(csd,56,58)); 
    fprintf(stream, "VDD_W_CURR_MIN:         0x%X\n", get_bit_value_from_u128(csd,53,55));
    fprintf(stream, "VDD_W_CURR_MAX:         0x%X\n", get_bit_value_from_u128(csd,50,52));
    fprintf(stream, "C_SIZE_MULT:            %d (%1.0lf)\n", csd_c_size_mult, c_size_mult);
    fprintf(stream, "ERASE_GRP_SIZE:         %d\n", get_bit_value_from_u128(csd,42,46));
    fprintf(stream, "ERASE_GRP_MULT:         %d\n", get_bit_value_from_u128(csd,37,41));
    fprintf(stream, "WP_GRP_SIZE:            %d\n", get_bit_value_from_u128(csd,32,36));
    fprintf(stream, "WP_GRP_ENABLE:          %d\n", get_bit_value_from_u128(csd,31,31));
    fprintf(stream, "DEFAULT_ECC:            %d\n", get_bit_value_from_u128(csd,29,30));
    fprintf(stream, "R2W_FACTOR:             %d\n", get_bit_value_from_u128(csd,26,28));
    fprintf(stream, "WRITE_BL_LEN:           %d\n", get_bit_value_from_u128(csd,22,25));
    fprintf(stream, "WRITE_BL_PARTIAL:       %d\n", get_bit_value_from_u128(csd,21,21));
    fprintf(stream, "CONTENT_PROT_APP:       %d\n", get_bit_value_from_u128(csd,16,16));
    fprintf(stream, "FILE_FORMAT_GRP:        %d\n", get_bit_value_from_u128(csd,15,15));
    fprintf(stream, "COPY:                   %d\n", get_bit_value_from_u128(csd,14,14));
    fprintf(stream, "PERM_WRITE_PROTECT:     %d\n", get_bit_value_from_u128(csd,13,13));
    fprintf(stream, "TMP_WRITE_PROTECT:      %d\n", get_bit_value_from_u128(csd,12,12));
    fprintf(stream, "FILE_FORMAT:            %d\n",get_bit_value_from_u128(csd,10,11));
    fprintf(stream, "ECC:                    %d\n",get_bit_value_from_u128(csd,8,9));
    fprintf(stream, "CRC:                    %d\n",get_bit_value_from_u128(csd,1,7));    

}

void decode_ext_csd(uint8_t *ext_csd_buf, FILE *stream)
{
    uint32_t wp_grp_size = 0;
    uint32_t erase_unit_size = 0;
    ext_csd_t *ext_csd = (ext_csd_t *)ext_csd_buf;
    fprintf(stream, "----------------EXT_CSD Register 5.1----------------\n");
    fprintf(stream, "[15]CMDQ_MODE_EN:                        0x%X\n", ext_csd->cmdq_mode_en);
    
    fprintf(stream, "[16]SECURE_REMOVAL_TYPE:                 0x%X\n", ext_csd_buf[16] );
    fprintf(stream, "    Supported Secure Removal Type:       0x%X\n", 
                    ext_csd->secure_removal_type.spt_sec_rm_type);
    fprintf(stream, "    Configure Secure Removal Type:       0x%X\n", 
                    ext_csd->secure_removal_type.conf_sec_rm_type);

    fprintf(stream, "[17]PRODUCT_STATE_AWARENESS_ENABLEMENT:  0x%X\n",
                    ext_csd_buf[17]);
    fprintf(stream, "    Manual mode Supported:               0x%X\n", 
                    ext_csd->pdt_stat_aware_en.man_mode_spt);
    fprintf(stream, "    Auto mode Supported:                 0x%X\n",
                    ext_csd->pdt_stat_aware_en.auto_mode_spt);
    fprintf(stream, "    Production State Awareness enable:   0x%X\n",
                    ext_csd->pdt_stat_aware_en.pdt_stat_aware_en);
    fprintf(stream, "    Mode:                                0x%X\n",
                    ext_csd->pdt_stat_aware_en.mode);
                    
    fprintf(stream, "[18-21]MAX_PRE_LOADING_DATA_SIZE:  0x%X\n", 
                    ext_csd->max_loading_data_size);    
    fprintf(stream, "[22-25]PRE_LOADING_DATA_SIZE:      0x%X\n", 
                    ext_csd->pre_loading_data_size);
    fprintf(stream, "[26]FFU_STATUS:                    0x%X\n", 
                    ext_csd->ffu_stat);
    fprintf(stream, "[29]MODE_OPERATION_CODES:          0x%X\n", 
                    ext_csd->mode_operation_codes);
    fprintf(stream, "[30]MODE_CONFIG:                   0x%X\n", 
                    ext_csd->mode_conf);    
    fprintf(stream, "[31]BARRIER_CTRL:                  0x%X\n", 
                    ext_csd->barrier_ctrl);    
    fprintf(stream, "[32]FLUSH_CACHE:                   0x%X\n", 
                    ext_csd_buf[32]);
    fprintf(stream, "    FLUSH:             0x%X\n", ext_csd->flush_cache.flush);
    fprintf(stream, "    BARRIER:           0x%X\n", ext_csd->flush_cache.barrier);

    fprintf(stream, "[33]CACHE_CTRL:                    0x%X\n", ext_csd->cache_ctrl);
    fprintf(stream, "[34]POWER_OFF_NOTIFICATION:        0x%X\n", 
                    ext_csd->power_off_notification);
    fprintf(stream, "[35]PACKED_FAILURE_INDEX:          0x%X\n", 
                    ext_csd->packed_failure_index);
    
    fprintf(stream, "[36]PACKED_COMMAND_STATUS:         0x%X\n", ext_csd_buf[36]);
    fprintf(stream, "    Error:             0x%X\n", ext_csd->packed_command_status.error);
    fprintf(stream, "    IndexedError:      0x%X\n", ext_csd->packed_command_status.indexed_error);    

    fprintf(stream, "[59]CLASS_6_CTRL:                  0x%X\n", ext_csd->class_6_ctrl);
    fprintf(stream, "[130]PROGRAM_CID_CSD_DDR_SPT       0x%X\n", ext_csd->program_cid_csd_ddr_spt);
    fprintf(stream, "[171]USER_WP:                      0x%X\n", ext_csd_buf[171]);
    fprintf(stream, "    US_PWR_WP_EN:      0x%X\n", ext_csd->user_wp.us_pwr_wp_en);
    fprintf(stream, "    US_PERM_WP_EN:     0x%X\n", ext_csd->user_wp.us_perm_wp_en);
    fprintf(stream, "    US_PWR_WP_DIS:     0x%X\n", ext_csd->user_wp.us_pwr_wp_dis);
    fprintf(stream, "    US_PERM_WP_DIS:    0x%X\n", ext_csd->user_wp.us_perm_wp_dis);
    fprintf(stream, "    CD_PERM_WP_DIS:    0x%X\n", ext_csd->user_wp.cd_perm_wp_dis);
    fprintf(stream, "    PERM_PSWD_DIS:     0x%X\n", ext_csd->user_wp.perm_pswd_dis);
    
    fprintf(stream, "[175]ERASE_GROUP_DEF:              0x%X\n", ext_csd->erase_grp_def);
    fprintf(stream, "[181]ERASED_MEM_CONT:              0x%X\n", ext_csd->erased_mem_cont);
    fprintf(stream, "[192]EXT_CSD_REV:                  0x%X\n", ext_csd->ext_csd_rev);
    fprintf(stream, "[194]CSD_STRUCTURE:                0x%X\n", ext_csd->csd_struct);
    fprintf(stream, "[196]DEVICE_TYPE:                  0x%X\n", ext_csd->device_type);
    fprintf(stream, "[221]HC_WP_GRP_SIZE:               0x%X\n", ext_csd->hc_wp_grp_size);
    fprintf(stream, "[224]HC_ERASE_GRP_SIZE:            0x%X\n", ext_csd->hc_erase_grp_size);

    fprintf(stream, "[231]SEC_FEATURE_SUPPORT:          0x%X\n", ext_csd_buf[231]);
    fprintf(stream, "    SECURE_ER_EN:      0x%X\n", ext_csd->sec_feat_spt.secure_er_en);
    fprintf(stream, "    SEC_BD_BLK_EN:     0x%X\n", ext_csd->sec_feat_spt.sec_bd_blk_en);
    fprintf(stream, "    SEC_GB_CL_EN:      0x%X\n", ext_csd->sec_feat_spt.sec_gb_cl_en);
    fprintf(stream, "    SEC_SANITIZE:      0x%X\n", ext_csd->sec_feat_spt.sec_sanitize);

    fprintf(stream, "[254-261]FIRMWARE_VERSION:         %s\n", ext_csd->fw_ver);
    fprintf(stream, "[262-263]DEVICE_VERSION:           0x%X\n", *(ext_csd->dev_ver));

    fprintf(stream, "----------------Summary----------------\n");
    if(ext_csd->erase_grp_def)
    {
        //KB
        erase_unit_size = 512 * (ext_csd->hc_erase_grp_size);        
        wp_grp_size = erase_unit_size * (ext_csd->hc_wp_grp_size); 
        fprintf(stream,"Erase Group Size:           %d KB\n", erase_unit_size);  
        fprintf(stream,"Write Protect Group Size:   %d KB\n", wp_grp_size);        
    }
    else
    {
        fprintf(stream,"Write Protect Group Size:   Defined in CSD\n");
        fprintf(stream,"Erase Group Size:           Defined in CSD\n");
    }

    
    
}

#endif

