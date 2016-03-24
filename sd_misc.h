#ifndef SD_MISC_H
#define SD_MISC_H

typedef struct __device_status
{
    uint32_t manu_test_mode:2;
    uint32_t app_spec:2;
    uint32_t rsvd0:1;
    uint32_t app_cmd:1;
    uint32_t except_event:1;
    uint32_t switch_error:1;
    uint32_t rdy_for_data:1;
    uint32_t current_state:4;
    uint32_t erase_reset:1;
    uint32_t rsvd1:1;
    uint32_t wp_erase_skip:1;
    uint32_t cid_csd_over_write:1;
    uint32_t obsolete1:1;
    uint32_t obsolete2:1;
    uint32_t error:1;
    uint32_t cc_error:1;
    uint32_t dev_ecc_fail:1;
    uint32_t illegal_cmd:1;
    uint32_t cmd_crc_error:1;
    uint32_t lock_unlock_fail:1;
    uint32_t dev_is_locked:1;
    uint32_t wp_violation:1;
    uint32_t erase_param:1;
    uint32_t erase_seq_error:1;
    uint32_t blk_len_error:1;
    uint32_t addr_misalign:1;
    uint32_t addr_out_of_range:1;
}device_status_t;

#pragma pack(1)
typedef struct __ext_csd
{
    //0-15
    uint8_t rsvd0_15[15];
    uint8_t cmdq_mode_en;


    //16-17
    struct 
    {
        uint8_t spt_sec_rm_type:4;
        uint8_t conf_sec_rm_type:2;
        uint8_t rsvd:2;
    }secure_removal_type;
   
    struct
    {
        uint8_t man_mode_spt:1;
        uint8_t auto_mode_spt:1;
        uint8_t rsvd1:2;
        uint8_t pdt_stat_aware_en:1;
        uint8_t mode:1;
        uint8_t rsvd2:2;
    }pdt_stat_aware_en;

    //18-25
    uint32_t max_loading_data_size;
    uint32_t pre_loading_data_size;

    //26-31
    uint8_t ffu_stat;
    uint16_t rsvd1;
    uint8_t mode_operation_codes;
    uint8_t mode_conf;
    uint8_t barrier_ctrl;

    //32-36    
    struct 
    {
        uint8_t flush:1;
        uint8_t barrier:1;
        uint8_t rsvd:6;
    }flush_cache;
    
    uint8_t cache_ctrl;
    uint8_t power_off_notification;
    uint8_t packed_failure_index;
    
    struct
    {   
        uint8_t error:1;
        uint8_t indexed_error:1;
        uint8_t rsvd:6;
    }packed_command_status;
 
    //37-51
    struct
    {
        uint8_t conf_direct_activate:2;
        uint8_t large_uint_context:1;
        uint8_t large_uint_mult:3;
        uint8_t reliability_mode:2;
    }context_conf[15];


    //52-53
    struct
    {
        uint8_t ext1:4;
        uint8_t ext2:4;
        uint8_t ext3:4;
        uint8_t ext4:4;
    }ext_part_attr;

    //54-55
    struct
    {
        uint8_t urgent_bkops:1;
        uint8_t dyncap_needed:1;
        uint8_t syspool_exhausted:1;
        uint8_t packed_failure:1;
        uint8_t extended_security:1;
        uint8_t rvsd1:3;
        uint8_t rvsd2;
    }exception_events_status;

    //56-57

    struct
    {
        uint8_t rsvd1:1;
        uint8_t dyncap_event_en:1;
        uint8_t syspool_event_en:1;
        uint8_t packed_event_en:1;
        uint8_t extended_security_en:1;
        uint8_t rvsd2:3;
        uint8_t rvsd3;
    }exception_events_ctrl;

    //58-63
    uint8_t dyncap_needed;
    uint8_t class_6_ctrl;
    uint8_t ini_timeout_emu;
    uint8_t data_sector_size;
    uint8_t use_native_sector;
    uint8_t native_sector_size;

    
    //64-127
    uint8_t vu[64];


    //128-129
    uint16_t rsvd2;


    //130-135
    uint8_t program_cid_csd_ddr_spt;

    struct 
    {
        uint8_t wakeup_period:5;
        uint8_t wakeup_unit:3;
    }periodic_wakeup;


    uint8_t tcase_spt;
    uint8_t production_stat_aware;
    uint8_t sec_bad_blk_mgmnt;
    uint8_t rsvd3;

    //136-139
    uint8_t enh_start_addr[4];

    //140-142
    uint8_t enh_size_mult[3];

    //143-154
    struct
    {
        uint8_t gp_size_mult_0;
        uint8_t gp_size_mult_1;
        uint8_t gp_size_mult_2;        
    }gp_size_mult[4];

    //155-159
    uint8_t part_setting_complete;
    struct
    {
        uint8_t enh_usr:1;
        uint8_t enh_1:1;
        uint8_t enh_2:1;
        uint8_t enh_3:1;
        uint8_t enh_4:1;
        uint8_t rsvd:3;
    }part_attr;

    uint8_t max_enh_size_mult[3];

    //160-165
    struct
    {
        uint8_t part_en:1;
        uint8_t enh_attr_en:1;
        uint8_t ext_attr_en:1;
        uint8_t rsvd:5;
    }part_spt;
    
    uint8_t hpi_mgmt;
    uint8_t rst_n_func;

    struct
    {
        uint8_t manual_en:1;
        uint8_t auto_en:1;
        uint8_t rsvd:6;
    }bkops_en;

    uint8_t bkpos_start;
    uint8_t sanitize_start;


    //166 - 170
    struct
    {
        uint8_t hs_ctrl_rel:1;
        uint8_t rsvd1:1;
        uint8_t en_rel_wr:1;
        uint8_t rsvd2:1;
        uint8_t en_rpmb_rel_wr:1;
        uint8_t rsvd3:3;
    }wr_rel_param;

    struct
    {
        uint8_t wr_data_rel_usr:1;
        uint8_t wr_data_rel_1:1;
        uint8_t wr_data_rel_2:1;
        uint8_t wr_data_rel_3:1;
        uint8_t wr_data_rel_4:1;
        uint8_t rsvd:3;
    }wr_rel_set;

    uint8_t rpmb_size_mult;
    uint8_t fw_config;    
    uint8_t rsvd4;


    //171 - 174
    struct
    {
        uint8_t us_pwr_wp_en:1;
        uint8_t rsvd1:1;
        uint8_t us_perm_wp_en:1;
        uint8_t us_pwr_wp_dis:1;
        uint8_t us_perm_wp_dis:1;
        uint8_t rsvd2:1;
        uint8_t cd_perm_wp_dis:1;
        uint8_t perm_pswd_dis:1;
    }user_wp;
    
    uint8_t rsvd5;

    struct
    {
        uint8_t b_pwr_wp_en:1;
        uint8_t b_pwr_wp_sec_sel:1;
        uint8_t b_perm_wp_en:1;
        uint8_t b_perm_wp_sec_sel:1;
        uint8_t b_perm_wp_dis:1;
        uint8_t rsvd:1;
        uint8_t b_pwr_wp_dis:1;
        uint8_t b_sec_wp_sel:1;
    }boot_wp;

    struct
    {
        uint8_t b_area_1_wp:2;
        uint8_t b_area_2_wp:2;
        uint8_t rsvd:4;
    }boot_wp_stat;

    //175-180
    uint8_t erase_grp_def;
    uint8_t rsvd176;
    struct
    {
        uint8_t boot_bus_width:2;
        uint8_t reset_boot_bus_cond:1;
        uint8_t boot_mode:2;
        uint8_t rsvd:3;
    }boot_bus_cond;
    

    struct
    {
        uint8_t pwr_boot_config_prot:1;
        uint8_t rsvd1:3;
        uint8_t perm_boot_config_prot:1;
        uint8_t rsvd2:3;
    }boot_conf_prot;

    struct
    {
        uint8_t part_access:3;
        uint8_t boot_part_en:3;
        uint8_t boot_ack:1;
        uint8_t rsvd:1;
    }part_conf;

    uint8_t rsvd180;
    
    //181 - 186
    uint8_t erased_mem_cont;
    uint8_t rsvd182;
    struct
    {
        uint8_t bus_mode_sel:4;
        uint8_t rsvd:3;
        uint8_t enh_strobe:1;
    }bus_width;

    uint8_t strobe_spt;

    struct
    {
        uint8_t timing_interface:4;
        uint8_t sel_dri_strength:4;
    }hs_timing;
    uint8_t rsvd186;

    //187-191
    struct
    {
        uint8_t dev_pwr_class_code:4;
        uint8_t rsvd:4;
    }pwr_class;
    uint8_t rsvd188;
    uint8_t cmd_set_rev;
    uint8_t rsvd190;
    uint8_t cmd_set;
    
    //192-196
    uint8_t ext_csd_rev;
    uint8_t rsvd193;
    uint8_t csd_struct;
    uint8_t rsvd195;
    uint8_t device_type;

    //197 -199
    struct
    {
        uint8_t type0:1;
        uint8_t type1:1;
        uint8_t type2:1;
        uint8_t type3:1;
        uint8_t type4:1;
        uint8_t rsvd:3;
    }driv_strength;
    uint8_t out_of_int_time;
    uint8_t part_switch_time;
    
    //200-204
    uint8_t PWR_CL_26_360;
    uint8_t PWR_CL_52_360;
    uint8_t PWR_CL_26_195;
    uint8_t PWR_CL_52_195;
    uint8_t rsvd204;

    //205-210
    uint8_t MIN_PERF_R_4_26;
    uint8_t MIN_PERF_W_4_26;
    uint8_t MIN_PERF_R_8_26_4_52;
    uint8_t MIN_PERF_W_8_26_4_52;
    uint8_t MIN_PERF_R_8_52;
    uint8_t MIN_PERF_W_8_52;
    
    //211 - 217     
    struct
    {
        uint8_t secure_wp_spt:1;
        uint8_t secure_wp_en_stat:1;
        uint8_t rsvd:6;
    }secure_wp_info;
    
    uint32_t sector_count;
    uint8_t sleep_notification_time;
    uint8_t s_a_timeout;

    //218 - 220
    uint8_t pdt_stat_aware_to;
    uint8_t sleep_cur_vccq;
    uint8_t sleep_cur_vcc;

    //221 - 227    
    uint8_t hc_wp_grp_size;
    uint8_t rel__wr_sec_c;
    uint8_t erase_to_mult;
    uint8_t hc_erase_grp_size;
    uint8_t acc_size;
    uint8_t boot_size_mult;
    uint8_t rsvd227;

    //228 - 233
    struct
    {
        uint8_t alt_boot_mode:1;
        uint8_t ddr_boot_mode:1;
        uint8_t hs_boot_mode:1;
        uint8_t rsvd:5;
    }boot_info;
    uint8_t sec_trim_mult;
    uint8_t sec_erase_mult;

    struct
    {
        uint8_t secure_er_en:1;
        uint8_t rsvd1:1;
        uint8_t sec_bd_blk_en:1;
        uint8_t rsvd3:1;
        uint8_t sec_gb_cl_en:1;
        uint8_t rsvd5:1;
        uint8_t sec_sanitize:1;
        uint8_t rsvd7:1;
    }sec_feat_spt;
    uint8_t trim_mult;
    uint8_t rsvd233;
    
    //234-241
    uint8_t MIN_PERF_DDR_R_8_52;
    uint8_t MIN_PERF_DDR_W_8_52;
    uint8_t PWR_CL_200_130;
    uint8_t PWR_CL_200_195;
    uint8_t PWR_CL_DDR_52_195;
    uint8_t PWR_CL_DDR_52_360;
    uint8_t cache_flush_policy;
    uint8_t ini_timeout_ap;
    
    //242-253
    uint8_t correctly_prg_sectors_num[4];
    uint8_t bkops_status;
    uint8_t pwr_off_long_time;
    uint8_t generic_cmd6_time;
    uint32_t cache_size;
    uint8_t PWR_CL_DDR_200_360;

    //254-263
    uint8_t fw_ver[8];
    uint8_t dev_ver[2];
    
    //264-269
    uint8_t optimal_trim_unit_size;
    uint8_t optimal_write_size;
    uint8_t optimal_read_size;
    uint8_t pre_eol_info;
    uint8_t dev_life_time_est_typ_a;
    uint8_t dev_life_time_est_typ_b;

    //270-306
    uint8_t vendor_health_report[32];
    uint32_t num_of_fw_sector_correct_prg;
    uint8_t rsvd306;

    //307-308
    uint8_t cmdq_depth;
    uint8_t cmdq_spt;

    //309-485
    uint8_t rsvd309_485[177];

    //486-492
    uint8_t barrier_spt;
    uint32_t ffu_arg;
    uint8_t opt_codes_to;
    uint8_t ffu_feat;

    //493 - 499
    struct
    {
        uint8_t ffu:1;
        uint8_t vsm:1;
        uint8_t rsvd:6;
    }spt_modes;

    struct
    {
        uint8_t sys_code:1;
        uint8_t non_pers:1;
        uint8_t rsvd:6;
    }ext_spt;

    uint8_t large_unit_size_m1;

    struct
    {
        uint8_t max_context_id:4;
        uint8_t large_unit_max_mult_m1:3;
        uint8_t rsvd:1;
    }context_caps;

    uint8_t tag_res_size;
    uint8_t tag_unit_size;
    uint8_t data_tag_spt;

    //500-505
    uint8_t max_packed_writes;
    uint8_t max_packed_reads;
    uint8_t bkops_spt;
    uint8_t hpi_feat;
    uint8_t s_cmd_set;
    struct
    {
        uint8_t sec_invalid_cmd_param:1;
        uint8_t access_denied:1;
        uint8_t rsvd:6;
    }ext_secure_err;

    //505-511
    uint8_t rsvd505_511[6];
}ext_csd_t;

void decode_device_status(uint32_t *dev_status);
void decode_csd(uint32_t *resp, FILE* stream);
void decode_ext_csd(uint8_t *ext_csd_buf, FILE *stream);
void dump_cmd_desc(struct mmc_ioc_cmd *cmd_desc, FILE* stream);

#endif
