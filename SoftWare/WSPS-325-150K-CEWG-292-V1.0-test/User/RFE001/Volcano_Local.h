/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          VolcanoA_local.h
 *
 * \date          %modify_time%
 *
 * \author        Michael VANNIER
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 * \section info Change Information
 *
*/


#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Standard Types and defines:                                                */
/*============================================================================*/

/* Maximum number of systems supported by driver */
#define Volcano_UNITS                      4

/* POR  SPI registers Value */
#define Volcano_POR   { \
           0x10, 0x01, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0x52, 0x00, 0x00, \
           0x00, 0x00, 0x00, 0x00, 0x30, 0xFF, 0x03, 0x02, 0x01, 0x06, \
           0x16, 0x00, 0x01, 0x00, 0x04, 0x34, 0x22, 0x80, 0x00, 0x00, \
           0x00, 0x0C, 0x42, 0x44, 0x02, 0x15, 0x00, 0x00, 0x00, 0x00, \
           0x00, 0x00, 0x00, 0x00, 0x00}   /* Reg Map */

/* Driver version definition */
#define Volcano_COMP_NUM                       0  /* Major protocol change - Specification change required */
#define Volcano_MAJOR_VER                      2  /* Minor protocol change - Specification change required */
#define Volcano_MINOR_VER                      2  /* Software update - No protocol change - No specification change required */

/* Instance macros */
#define P_OBJ_VALID                             (pObj != Null)

/* I/O Functions macros */
#define P_SIO                                   pObj->sIo
#define P_SIO_READ                              P_SIO.Read
#define P_SIO_WRITE                             P_SIO.Write
#define P_SIO_WRITEREAD                         P_SIO.WriteRead
#define P_SIO_READ_VALID                        (P_OBJ_VALID && (P_SIO_READ != Null))
#define P_SIO_WRITE_VALID                       (P_OBJ_VALID && (P_SIO_WRITE != Null))

/* Time Functions macros */
#define P_STIME                                 pObj->sTime
#define P_STIME_WAIT                            P_STIME.Wait
#define P_STIME_WAIT_VALID                      (P_OBJ_VALID && (P_STIME_WAIT != Null))

/* Bus Access */
typedef enum _BusAccess_t
{
    /* RW Bus access */
    eBus_RW = 0x00,
    /* No Read Bus access */
    eBus_NoRead = 0x01,
    /* No Write Bus access */
    eBus_NoWrite = 0x02,
    /* No Bus access. Update RegMap only */
    eBus_None = eBus_NoRead|eBus_NoWrite,
    eBus_Max
    } BusAccess_t;

/*============================================================================*/
/* Registers definitions:                                                     */
/*============================================================================*/

#define Volcano_REG_ADD_SZ                             (0x01)
#define Volcano_REG_DATA_MAX_SZ                        (0x01)
#define Volcano_REG_MAP_NB_BYTES                       (45)

#define Volcano_REG_DATA_LEN(_FIRST_REG, _LAST_REG)    ( (_LAST_REG.Address - _FIRST_REG.Address) + 1)

/* Volcano Register ID_1 0x00 */
static const Volcano_BitField_t gVolcano_Reg_ID_1 = { 0x00, 0x00, 0x08, 0x00 };
/* MAJOR_REV bit(s): Major revision of the IC */
static const Volcano_BitField_t gVolcano_Reg_ID_1__MAJOR_REV = { 0x00, 0x04, 0x04, 0x00 };
/* MINOR_REV bit(s): Minor revision of the IC */
static const Volcano_BitField_t gVolcano_Reg_ID_1__MINOR_REV = { 0x00, 0x00, 0x04, 0x00 };


/* Volcano Register POWER_STATUS_1 0x01 */
static const Volcano_BitField_t gVolcano_Reg_POWER_STATUS_1 = { 0x01, 0x00, 0x08, 0x00 };
/* LO_Lock bit(s): PLL lock status indicator */
static const Volcano_BitField_t gVolcano_Reg_POWER_STATUS_1__LO_Lock = { 0x01, 0x07, 0x01, 0x00 };
/* LO_forced_lock bit(s): Forces PLL Lock bit if not present after 5ms */
static const Volcano_BitField_t gVolcano_Reg_POWER_STATUS_1__LO_forced_lock = { 0x01, 0x06, 0x01, 0x00 };
/* POR bit(s): Power on reset bit */
static const Volcano_BitField_t gVolcano_Reg_POWER_STATUS_1__POR = { 0x01, 0x00, 0x01, 0x00 };


typedef enum _Volcano_adc_vtune_Item_t {
    Volcano_adc_vtune_0_to_0d33_V = 0,		/*  0 to 0.33   */
    Volcano_adc_vtune_0d33_to_0d45_V = 1,		/*  0.33 to 0.45   */
    Volcano_adc_vtune_0d45_to_0d58_V = 2,		/*  0.45 to 0.58   */
    Volcano_adc_vtune_0d58_to_1d05_V = 3,		/*  0.58 to 1.05   */
    Volcano_adc_vtune_1d05_to_1d62_V = 4,		/*  1.05 to 1.62   */
    Volcano_adc_vtune_1d62_to_1d84_V = 5,		/*  1.62 to 1.84   */
    Volcano_adc_vtune_1d84_to_2d1_V = 6,		/*  1.84 to 2.1   */
    Volcano_adc_vtune_2d1_upper_V = 7		/*  2.1 upper  */
} Volcano_adc_vtune_t, *pVolcano_adc_vtune_t;

/* Volcano Register VCO_LOG 0x02 */
static const Volcano_BitField_t gVolcano_Reg_VCO_LOG = { 0x02, 0x00, 0x08, 0x00 };
/* adc_vtune bit(s): Output value of ADC connected to Vtune */
static const Volcano_BitField_t gVolcano_Reg_VCO_LOG__adc_vtune = { 0x02, 0x04, 0x03, 0x00 };
/* atrf_phase_read_1 bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_VCO_LOG__atrf_phase_read_1 = { 0x02, 0x00, 0x01, 0x00 };


/* Volcano Register ATRF_PHASE_SHIFTER_1 0x03 */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_1 = { 0x03, 0x00, 0x08, 0x00 };
/* atrf_phase_read_2 bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_1__atrf_phase_read_2 = { 0x03, 0x00, 0x08, 0x00 };


/* Volcano Register LO_CALIB_1 0x04 */
static const Volcano_BitField_t gVolcano_Reg_LO_CALIB_1 = { 0x04, 0x00, 0x08, 0x00 };
/* LO_calib_counter_1 bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_LO_CALIB_1__LO_calib_counter_1 = { 0x04, 0x00, 0x06, 0x00 };


/* Volcano Register LO_CALIB_2 0x05 */
static const Volcano_BitField_t gVolcano_Reg_LO_CALIB_2 = { 0x05, 0x00, 0x08, 0x00 };
/* LO_calib_counter_2 bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_LO_CALIB_2__LO_calib_counter_2 = { 0x05, 0x00, 0x08, 0x00 };


typedef enum _Volcano_XT_Dac_Item_t {
    Volcano_XT_Dac_0 = 0,		/*  0   */
    Volcano_XT_Dac_1 = 1,		/*  1   */
    Volcano_XT_Dac_2 = 2,		/*  2   */
    Volcano_XT_Dac_3 = 3,		/*  3   */
    Volcano_XT_Dac_4 = 4,		/*  4   */
    Volcano_XT_Dac_5 = 5,		/*  5   */
    Volcano_XT_Dac_6 = 6,		/*  6   */
    Volcano_XT_Dac_7 = 7,		/*  7   */
    Volcano_XT_Dac_8 = 8,		/*  8   */
    Volcano_XT_Dac_9 = 9,		/*  9   */
    Volcano_XT_Dac_10 = 10,		/*  10   */
    Volcano_XT_Dac_11 = 11,		/*  11   */
    Volcano_XT_Dac_12 = 12,		/*  12   */
    Volcano_XT_Dac_13 = 13,		/*  13   */
    Volcano_XT_Dac_14 = 14,		/*  14   */
    Volcano_XT_Dac_15 = 15		/*  15  */
} Volcano_XT_Dac_t, *pVolcano_XT_Dac_t;

/* Volcano Register XT_CAL_1 0x06 */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_1 = { 0x06, 0x00, 0x08, 0x00 };
/* XT_Dac bit(s): Xtal Osccillator DAC value (from Xtal CAL or forced mode) */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_1__XT_Dac = { 0x06, 0x00, 0x04, 0x00 };


/* Volcano Register THERMO_1 0x07 */
static const Volcano_BitField_t gVolcano_Reg_THERMO_1 = { 0x07, 0x00, 0x08, 0x00 };
/* TM_D bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_THERMO_1__TM_D = { 0x07, 0x00, 0x07, 0x00 };


/* Volcano Register IRQ_status 0x08 */
static const Volcano_BitField_t gVolcano_Reg_IRQ_status = { 0x08, 0x00, 0x08, 0x00 };
/* atrf_End bit(s): ATRF Cal end IRQ */
static const Volcano_BitField_t gVolcano_Reg_IRQ_status__atrf_End = { 0x08, 0x02, 0x01, 0x00 };
/* VCO_Freq_End bit(s): VCO Freq measurement end IRQ */
static const Volcano_BitField_t gVolcano_Reg_IRQ_status__VCO_Freq_End = { 0x08, 0x01, 0x01, 0x00 };
/* XtalCal_End bit(s): Xtal Cal end IRQ */
static const Volcano_BitField_t gVolcano_Reg_IRQ_status__XtalCal_End = { 0x08, 0x00, 0x01, 0x00 };


/* Volcano Register IRQ_enable 0x09 */
static const Volcano_BitField_t gVolcano_Reg_IRQ_enable = { 0x09, 0x00, 0x08, 0x00 };
/* atrf_Enable bit(s): ATRF IRQ enable */
static const Volcano_BitField_t gVolcano_Reg_IRQ_enable__atrf_Enable = { 0x09, 0x02, 0x01, 0x00 };
/* VCO_Freq_Enable bit(s): VCO Freq measurement IRQ enable */
static const Volcano_BitField_t gVolcano_Reg_IRQ_enable__VCO_Freq_Enable = { 0x09, 0x01, 0x01, 0x00 };
/* XtalCal_Enable bit(s): Xtal Cal IRQ enable */
static const Volcano_BitField_t gVolcano_Reg_IRQ_enable__XtalCal_Enable = { 0x09, 0x00, 0x01, 0x00 };


/* Volcano Register IRQ_clear 0x0A */
static const Volcano_BitField_t gVolcano_Reg_IRQ_clear = { 0x0A, 0x00, 0x08, 0x00 };
/* atrf_Clear bit(s): ATRF IRQ clear */
static const Volcano_BitField_t gVolcano_Reg_IRQ_clear__atrf_Clear = { 0x0A, 0x02, 0x01, 0x00 };
/* VCO_Freq_Clear bit(s): VCO Freq measurement IRQ clear */
static const Volcano_BitField_t gVolcano_Reg_IRQ_clear__VCO_Freq_Clear = { 0x0A, 0x01, 0x01, 0x00 };
/* XtalCal_Clear bit(s): Xtal Cal IRQ clear */
static const Volcano_BitField_t gVolcano_Reg_IRQ_clear__XtalCal_Clear = { 0x0A, 0x00, 0x01, 0x00 };


/* Volcano Register IRQ_set 0x0B */
static const Volcano_BitField_t gVolcano_Reg_IRQ_set = { 0x0B, 0x00, 0x08, 0x00 };
/* atrf_Set bit(s): ATRF IRQ set */
static const Volcano_BitField_t gVolcano_Reg_IRQ_set__atrf_Set = { 0x0B, 0x02, 0x01, 0x00 };
/* VCO_Freq_Set bit(s): VCO Freq measurement IRQ set */
static const Volcano_BitField_t gVolcano_Reg_IRQ_set__VCO_Freq_Set = { 0x0B, 0x01, 0x01, 0x00 };
/* XtalCal_Set bit(s): Xtal Cal IRQ set */
static const Volcano_BitField_t gVolcano_Reg_IRQ_set__XtalCal_Set = { 0x0B, 0x00, 0x01, 0x00 };


/* Volcano Register THERMO_2 0x0C */
static const Volcano_BitField_t gVolcano_Reg_THERMO_2 = { 0x0C, 0x00, 0x08, 0x00 };
/* TM_ON bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_THERMO_2__TM_ON = { 0x0C, 0x00, 0x01, 0x00 };


typedef enum _Volcano_CK_AnToDig_Item_t {
    Volcano_CK_AnToDig_No_signal = 0,		/*  No signal   */
    Volcano_CK_AnToDig_Xtal_16MHz = 1,		/*  Xtal 16MHz   */
    Volcano_CK_AnToDig_Fdiv_2 = 2,		/*  Fdiv/2   */
    Volcano_CK_AnToDig_Dig_clock_37MHz_2 = 3		/*  Dig clock (37MHz/2)  */
} Volcano_CK_AnToDig_t, *pVolcano_CK_AnToDig_t;

/* Volcano Register XT_OUT 0x0D */
static const Volcano_BitField_t gVolcano_Reg_XT_OUT = { 0x0D, 0x00, 0x08, 0x00 };
/* CK_AnToDig_BufOutEn bit(s): XTOUT test output buffer enable/disable */
static const Volcano_BitField_t gVolcano_Reg_XT_OUT__CK_AnToDig_BufOutEn = { 0x0D, 0x02, 0x01, 0x00 };
/* CK_AnToDig bit(s): XTOUT output signal selection */
static const Volcano_BitField_t gVolcano_Reg_XT_OUT__CK_AnToDig = { 0x0D, 0x00, 0x02, 0x00 };


typedef enum _Volcano_xt_cal_timer_Item_t {
    Volcano_xt_cal_timer_5d12_ms = 0,		/*  5.12   */
    Volcano_xt_cal_timer_10d24_ms = 1,		/*  10.24   */
    Volcano_xt_cal_timer_20d48_ms = 2,		/*  20.48   */
    Volcano_xt_cal_timer_40d96_ms = 3		/*  40.96  */
} Volcano_xt_cal_timer_t, *pVolcano_xt_cal_timer_t;

typedef enum _Volcano_XT_Dac_Force_Item_t {
    Volcano_XT_Dac_Force_0 = 0,		/*  0   */
    Volcano_XT_Dac_Force_1 = 1,		/*  1   */
    Volcano_XT_Dac_Force_2 = 2,		/*  2   */
    Volcano_XT_Dac_Force_3 = 3,		/*  3   */
    Volcano_XT_Dac_Force_4 = 4,		/*  4   */
    Volcano_XT_Dac_Force_5 = 5,		/*  5   */
    Volcano_XT_Dac_Force_6 = 6,		/*  6   */
    Volcano_XT_Dac_Force_7 = 7,		/*  7   */
    Volcano_XT_Dac_Force_8 = 8,		/*  8   */
    Volcano_XT_Dac_Force_9 = 9,		/*  9   */
    Volcano_XT_Dac_Force_10 = 10,		/*  10   */
    Volcano_XT_Dac_Force_11 = 11,		/*  11   */
    Volcano_XT_Dac_Force_12 = 12,		/*  12   */
    Volcano_XT_Dac_Force_13 = 13,		/*  13   */
    Volcano_XT_Dac_Force_14 = 14,		/*  14   */
    Volcano_XT_Dac_Force_15 = 15		/*  15  */
} Volcano_XT_Dac_Force_t, *pVolcano_XT_Dac_Force_t;

/* Volcano Register XT_CAL_2 0x0E */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_2 = { 0x0E, 0x00, 0x08, 0x00 };
/* XT_In_Force bit(s): Xtal oscillator Master/Slave mode */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_2__XT_In_Force = { 0x0E, 0x07, 0x01, 0x00 };
/* XT_Cal_Bypass bit(s): Allow bypass of the Xtal calibration value with XT_Dac_Force[3:0] from SPI */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_2__XT_Cal_Bypass = { 0x0E, 0x06, 0x01, 0x00 };
/* xt_cal_timer bit(s): xt_cal_timer selection */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_2__xt_cal_timer = { 0x0E, 0x04, 0x02, 0x00 };
/* XT_Dac_Force bit(s): Xtal Osc Amplitude Control value */
static const Volcano_BitField_t gVolcano_Reg_XT_CAL_2__XT_Dac_Force = { 0x0E, 0x00, 0x04, 0x00 };


/* Volcano Register POWER_DOWN_1 0x0F */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1 = { 0x0F, 0x00, 0x08, 0x00 };
/* pd_lotest bit(s): LO test input buffer enable/disable */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_lotest = { 0x0F, 0x07, 0x01, 0x00 };
/* PD_Synthe bit(s): PLL enable/disable (excluding LO Chain) */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__PD_Synthe = { 0x0F, 0x06, 0x01, 0x00 };
/* PD_adc_vtune bit(s): enables / disables the ADC connected on vtune */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__PD_adc_vtune = { 0x0F, 0x05, 0x01, 0x00 };
/* pd_rf_pa bit(s): PA On/Off */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_rf_pa = { 0x0F, 0x04, 0x01, 0x00 };
/* pd_rf_ppa bit(s): Pre PA On/Off */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_rf_ppa = { 0x0F, 0x03, 0x01, 0x00 };
/* pd_atrf bit(s): Phase shifter Test part On/Off */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_atrf = { 0x0F, 0x02, 0x01, 0x00 };
/* pd_rf_tx bit(s): Phase shifter On/Off */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_rf_tx = { 0x0F, 0x01, 0x01, 0x00 };
/* pd_lochain bit(s): LOCHAIN On/Off */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_1__pd_lochain = { 0x0F, 0x00, 0x01, 0x00 };


/* Volcano Register POWER_DOWN_2 0x10 */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_2 = { 0x10, 0x00, 0x08, 0x00 };
/* pd_rf_reg28 bit(s): RF part + level shifters 2.8V regulator control bit. Use with CARE !!! */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28 = { 0x10, 0x02, 0x01, 0x00 };
/* pd_daisy_out bit(s): Daisy chaining output buffer enable/disable */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_2__pd_daisy_out = { 0x10, 0x01, 0x01, 0x00 };
/* pd_daisy_in bit(s): Daisy chaining input buffer enable/disable */
static const Volcano_BitField_t gVolcano_Reg_POWER_DOWN_2__pd_daisy_in = { 0x10, 0x00, 0x01, 0x00 };


typedef enum _Volcano_Loch_curr_boost_Item_t {
    Volcano_Loch_curr_boost_low = 0,		/*  low   */
    Volcano_Loch_curr_boost_mid = 1,		/*  mid   */
    Volcano_Loch_curr_boost_high_POR = 2		/*  high (POR)  */
} Volcano_Loch_curr_boost_t, *pVolcano_Loch_curr_boost_t;

/* Volcano Register POWER_SAVING_MODE_1 0x11 */
static const Volcano_BitField_t gVolcano_Reg_POWER_SAVING_MODE_1 = { 0x11, 0x00, 0x08, 0x00 };
/* Loch_curr_boost bit(s): LO chain current boost !!! */
static const Volcano_BitField_t gVolcano_Reg_POWER_SAVING_MODE_1__Loch_curr_boost = { 0x11, 0x00, 0x02, 0x00 };


typedef enum _Volcano_rf_lo_dc_level_Item_t {
    Volcano_rf_lo_dc_level_1d4_V = 0,		/*  1.4   */
    Volcano_rf_lo_dc_level_1d55_V = 1,		/*  1.55   */
    Volcano_rf_lo_dc_level_1d55_2_V = 2,		/*  1.55(2)   */
    Volcano_rf_lo_dc_level_1d7_V = 3		/*  1.7  */
} Volcano_rf_lo_dc_level_t, *pVolcano_rf_lo_dc_level_t;

/* Volcano Register RF_LO_DRIVER 0x12 */
static const Volcano_BitField_t gVolcano_Reg_RF_LO_DRIVER = { 0x12, 0x00, 0x08, 0x00 };
/* rf_lo_dc_level bit(s): DAC LO input DC level control */
static const Volcano_BitField_t gVolcano_Reg_RF_LO_DRIVER__rf_lo_dc_level = { 0x12, 0x00, 0x02, 0x00 };


typedef enum _Volcano_daisy_in_cur_Item_t {
    Volcano_daisy_in_cur_400uA_150_mVpdiff = 0,		/*  400uA / 150 mVpdiff   */
    Volcano_daisy_in_cur_600uA_210_mVpdiff = 1,		/*  600uA / 210 mVpdiff   */
    Volcano_daisy_in_cur_800uA_280_mVpdiff = 2,		/*  800uA / 280 mVpdiff   */
    Volcano_daisy_in_cur_1mA_350_mVpdiff = 3		/*  1mA / 350 mVpdiff  */
} Volcano_daisy_in_cur_t, *pVolcano_daisy_in_cur_t;

/* Volcano Register DAISY_CHAIN_1 0x13 */
static const Volcano_BitField_t gVolcano_Reg_DAISY_CHAIN_1 = { 0x13, 0x00, 0x08, 0x00 };
/* lomux_to_daisy bit(s): Allow LO signal to Daisy output buffer */
static const Volcano_BitField_t gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy = { 0x13, 0x04, 0x01, 0x00 };
/* daisy_in_cur bit(s): Daisy chaining input buffer biasing current */
static const Volcano_BitField_t gVolcano_Reg_DAISY_CHAIN_1__daisy_in_cur = { 0x13, 0x02, 0x02, 0x00 };
/* daisy_out_ampl bit(s): Daisy chaining output buffer voltage swing */
static const Volcano_BitField_t gVolcano_Reg_DAISY_CHAIN_1__daisy_out_ampl = { 0x13, 0x01, 0x01, 0x00 };
/* daisy_out_drive bit(s): Daisy chaining output buffer drive */
static const Volcano_BitField_t gVolcano_Reg_DAISY_CHAIN_1__daisy_out_drive = { 0x13, 0x00, 0x01, 0x00 };


typedef enum _Volcano_rf_load_capa_Item_t {
    Volcano_rf_load_capa_75fF = 0,		/*  75fF   */
    Volcano_rf_load_capa_200fF_optimize_for_2d45_ISM_band = 1,		/*  200fF (optimize for 2.45 ISM band)   */
    Volcano_rf_load_capa_2 = 2,		/*  2   */
    Volcano_rf_load_capa_3 = 3,		/*  3   */
    Volcano_rf_load_capa_4 = 4,		/*  4   */
    Volcano_rf_load_capa_5 = 5,		/*  5   */
    Volcano_rf_load_capa_6 = 6,		/*  6   */
    Volcano_rf_load_capa_7 = 7,		/*  7   */
    Volcano_rf_load_capa_8 = 8,		/*  8   */
    Volcano_rf_load_capa_9 = 9,		/*  9   */
    Volcano_rf_load_capa_10 = 10,		/*  10   */
    Volcano_rf_load_capa_11 = 11,		/*  11   */
    Volcano_rf_load_capa_12 = 12,		/*  12   */
    Volcano_rf_load_capa_13 = 13,		/*  13   */
    Volcano_rf_load_capa_14 = 14,		/*  14   */
    Volcano_rf_load_capa_15 = 15		/*  15  */
} Volcano_rf_load_capa_t, *pVolcano_rf_load_capa_t;

typedef enum _Volcano_rf_quarter_current_Item_t {
    Volcano_rf_quarter_current_128_uA = 0,		/*  128   */
    Volcano_rf_quarter_current_160_uA = 1,		/*  160   */
    Volcano_rf_quarter_current_160_2_uA = 2,		/*  160(2)   */
    Volcano_rf_quarter_current_192_uA = 3		/*  192  */
} Volcano_rf_quarter_current_t, *pVolcano_rf_quarter_current_t;

typedef enum _Volcano_rf_dac_level_Item_t {
    Volcano_rf_dac_level_120_uA = 0,		/*  120   */
    Volcano_rf_dac_level_140_uA = 1,		/*  140   */
    Volcano_rf_dac_level_160_uA = 2,		/*  160   */
    Volcano_rf_dac_level_200_uA = 3		/*  200  */
} Volcano_rf_dac_level_t, *pVolcano_rf_dac_level_t;

/* Volcano Register RF_PHASE_SHIFTER_1 0x14 */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_1 = { 0x14, 0x00, 0x08, 0x00 };
/* rf_load_capa bit(s): capacitor value in the phase shifter load (low-pass filter) */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_1__rf_load_capa = { 0x14, 0x04, 0x04, 0x00 };
/* rf_quarter_current bit(s): Phase shifter DAC reference current control */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_1__rf_quarter_current = { 0x14, 0x02, 0x02, 0x00 };
/* rf_dac_level bit(s): Output current to the DAC */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_1__rf_dac_level = { 0x14, 0x00, 0x02, 0x00 };

typedef enum _Volcano_rf_clipper_Item_t {
    Volcano_rf_clipper_i_nom_n30_percent = 0,		/*  i_nom -30%   */
    Volcano_rf_clipper_i_nom_640uA = 1,		/*  i_nom (640uA)   */
    Volcano_rf_clipper_i_nom_p30_percent = 2,		/*  i_nom +30%   */
    Volcano_rf_clipper_i_nom_p60_percent = 3		/*  i_nom +60%  */
} Volcano_rf_clipper_t, *pVolcano_rf_clipper_t;

/* Volcano Register RF_PHASE_SHIFTER_3 0x16 */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_3 = { 0x16, 0x00, 0x08, 0x00 };
/* rf_clipper bit(s): RF Clipper bias current/voltage swing control */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_3__rf_clipper = { 0x16, 0x00, 0x02, 0x00 };

typedef enum _Volcano_atrf_pfd_dcref_Item_t {
    Volcano_atrf_pfd_dcref_ATRF_PFD_output_common_mode_voltage_is_1d2V_0_V = 0,		/*  ATRF PFD output common mode voltage is 1.2V (0)   */
    Volcano_atrf_pfd_dcref_ATRF_PFD_output_common_mode_voltage_is_1d0V_1_V = 1		/*  ATRF PFD output common mode voltage is 1.0V (1)  */
} Volcano_atrf_pfd_dcref_t, *pVolcano_atrf_pfd_dcref_t;

typedef enum _Volcano_atrf_adc_scale_Item_t {
    Volcano_atrf_adc_scale_ATRF_ADC_input_range_1d85V_0_V = 0,		/*  ATRF ADC input range 1.85V (0)   */
    Volcano_atrf_adc_scale_ATRF_ADC_input_range_1d55V_1_V = 1		/*  ATRF ADC input range 1.55V (1)  */
} Volcano_atrf_adc_scale_t, *pVolcano_atrf_adc_scale_t;

typedef enum _Volcano_atrf_pfd_scale_Item_t {
    Volcano_atrf_pfd_scale_ATRF_PFD_output_range_1d3V_00_V = 0,		/*  ATRF PFD output range 1.3V (00)   */
    Volcano_atrf_pfd_scale_ATRF_PFD_output_range_1d0V_01_V = 1,		/*  ATRF PFD output range 1.0V (01)   */
    Volcano_atrf_pfd_scale_ATRF_PFD_output_range_0d8V_10_V = 2,		/*  ATRF PFD output range 0.8V (10)   */
    Volcano_atrf_pfd_scale_ATRF_PFD_output_range_0d6V_11_V = 3		/*  ATRF PFD output range 0.6V (11)  */
} Volcano_atrf_pfd_scale_t, *pVolcano_atrf_pfd_scale_t;

/* Volcano Register ATRF_PHASE_SHIFTER_3 0x18 */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_3 = { 0x18, 0x00, 0x08, 0x00 };
/* atrf_pfd_dcref bit(s): ATRF PFD output common mode voltage */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_3__atrf_pfd_dcref = { 0x18, 0x03, 0x01, 0x00 };
/* atrf_adc_scale bit(s): ATRF ADC input range */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_3__atrf_adc_scale = { 0x18, 0x02, 0x01, 0x00 };
/* atrf_pfd_scale bit(s): ATRF PFD scale */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_3__atrf_pfd_scale = { 0x18, 0x00, 0x02, 0x00 };

typedef enum _Volcano_rf_pa_power_Item_t {
    Volcano_rf_pa_power_0_n16d6_n37d4_dBm = 0,		/*  0 (-16.6/-37.4 )   */
    Volcano_rf_pa_power_1_0_n22d3_dBm = 1,		/*  1 (0/-22.3 )   */
    Volcano_rf_pa_power_2_5d8_n16d3_dBm = 2,		/*  2 (5.8/-16.3 )   */
    Volcano_rf_pa_power_3_9d3_n13d0_dBm = 3,		/*  3 (9.3/-13.0 )   */
    Volcano_rf_pa_power_4_11d8_n10d8_dBm = 4,		/*  4 (11.8/-10.8 )   */
    Volcano_rf_pa_power_5_13d1_n9d6_dBm = 5,		/*  5 (13.1/-9.6 )   */
    Volcano_rf_pa_power_6_14d6_n8d3_dBm = 6,		/*  6 (14.6/-8.3 )   */
    Volcano_rf_pa_power_7_16d1_n7_dBm = 7,		/*  7 (16.1/-7 )   */
    Volcano_rf_pa_power_8_17d4_n5d5_dBm = 8,		/*  8 (17.4/-5.5 )   */
    Volcano_rf_pa_power_9_18d1_n4d6_dBm = 9,		/*  9 (18.1/-4.6 )   */
    Volcano_rf_pa_power_10_18d9_n3d8_dBm = 10,		/*  10 (18.9/-3.8 )   */
    Volcano_rf_pa_power_11_19d7_n2d9_dBm = 11,		/*  11 (19.7/-2.9 )   */
    Volcano_rf_pa_power_12_20d5_n2d0_dBm = 12,		/*  12 (20.5/-2.0 )   */
    Volcano_rf_pa_power_13_21_n1d4_dBm = 13,		/*  13 (21/-1.4 )   */
    Volcano_rf_pa_power_14_21d5_n0d8_dBm = 14,		/*  14 (21.5/-0.8 )   */
    Volcano_rf_pa_power_15_22_n0d2_dBm = 15,		/*  15 (22/-0.2 )   */
    Volcano_rf_pa_power_16_22d5_0d5_dBm = 16,		/*  16 (22.5/0.5 )   */
    Volcano_rf_pa_power_17_22d9_1_dBm = 17,		/*  17 (22.9/1 )   */
    Volcano_rf_pa_power_18_23d2_1d5_dBm = 18,		/*  18 (23.2/1.5 )   */
    Volcano_rf_pa_power_19_23d5_2_dBm = 19,		/*  19 (23.5/2 )   */
    Volcano_rf_pa_power_20_23d9_2d4_dBm = 20,		/*  20 (23.9/2.4 )   */
    Volcano_rf_pa_power_21_24d2_2d8_dBm = 21,		/*  21 (24.2/2.8 )   */
    Volcano_rf_pa_power_22_24d5_3d2_dBm = 22,		/*  22 (24.5/3.2 )   */
    Volcano_rf_pa_power_23_24d7_3d6_dBm = 23,		/*  23 (24.7/3.6 )   */
    Volcano_rf_pa_power_24_25_4d0_dBm = 24,		/*  24 (25/4.0 )   */
    Volcano_rf_pa_power_25_25d2_4d3_dBm = 25,		/*  25 (25.2/4.3 )   */
    Volcano_rf_pa_power_26_25d3_4d7_dBm = 26,		/*  26 (25.3/4.7 )   */
    Volcano_rf_pa_power_27_25d5_5_dBm = 27,		/*  27 (25.5/5 )   */
    Volcano_rf_pa_power_28_25d7_5d4_dBm = 28,		/*  28 (25.7/5.4 )   */
    Volcano_rf_pa_power_29_25d8_5d7_dBm = 29,		/*  29 (25.8/5.7 )   */
    Volcano_rf_pa_power_30_41451_dBm = 30,		/*  30 (41451)   */
    Volcano_rf_pa_power_31_26d1_6d2_dBm = 31,		/*  31 (26.1/6.2 )   */
    Volcano_rf_pa_power_32_26d2_6d5_dBm = 32,		/*  32 (26.2/6.5 )   */
    Volcano_rf_pa_power_33_26d3_6d7_dBm = 33,		/*  33 (26.3/6.7 )   */
    Volcano_rf_pa_power_34_26d4_7_dBm = 34,		/*  34 (26.4/7 )   */
    Volcano_rf_pa_power_35_26d5_7d2_dBm = 35,		/*  35 (26.5/7.2 )   */
    Volcano_rf_pa_power_36_26d6_7d5_dBm = 36,		/*  36 (26.6/7.5 )   */
    Volcano_rf_pa_power_37_26d6_7d7_dBm = 37,		/*  37 (26.6/7.7 )   */
    Volcano_rf_pa_power_38_26d7_8_dBm = 38,		/*  38 (26.7/8 )   */
    Volcano_rf_pa_power_39_26d7_8d2_dBm = 39,		/*  39 (26.7/8.2 )   */
    Volcano_rf_pa_power_40_26d8_8d4_dBm = 40,		/*  40 (26.8/8.4 )   */
    Volcano_rf_pa_power_41_26d8_8d6_dBm = 41,		/*  41 (26.8/8.6 )   */
    Volcano_rf_pa_power_42_26d9_8d8_dBm = 42,		/*  42 (26.9/8.8 )   */
    Volcano_rf_pa_power_43_26d9_9_dBm = 43,		/*  43 (26.9/9 )   */
    Volcano_rf_pa_power_44_26d9_9d2_dBm = 44,		/*  44 (26.9/9.2 )   */
    Volcano_rf_pa_power_45_26d9_9d4_dBm = 45,		/*  45 (26.9/9.4 )   */
    Volcano_rf_pa_power_46_26d9_9d6_dBm = 46,		/*  46 (26.9/9.6 )   */
    Volcano_rf_pa_power_47_26d9_9d8_dBm = 47,		/*  47 (26.9/9.8 )   */
    Volcano_rf_pa_power_48_26d9_9d9_dBm = 48,		/*  48 (26.9/9.9 )   */
    Volcano_rf_pa_power_49_27_10d1_dBm = 49,		/*  49 (27/10.1 )   */
    Volcano_rf_pa_power_50_27_10d2_dBm = 50,		/*  50 (27/10.2 )   */
    Volcano_rf_pa_power_51_27_10d4_dBm = 51,		/*  51 (27/10.4 )   */
    Volcano_rf_pa_power_52_27_10d6_dBm = 52,		/*  52 (27/10.6 )   */
    Volcano_rf_pa_power_53_27_10d66_dBm = 53,		/*  53 (27/10.66)   */
    Volcano_rf_pa_power_54_27d1_10d8_dBm = 54,		/*  54 (27.1/10.8 )   */
    Volcano_rf_pa_power_55_27d1_10d9_dBm = 55,		/*  55 (27.1/10.9 )   */
    Volcano_rf_pa_power_56_27d1_11_dBm = 56,		/*  56 (27.1/11 )   */
    Volcano_rf_pa_power_57_27d1_11d1_dBm = 57,		/*  57 (27.1/11.1 )   */
    Volcano_rf_pa_power_58_27d1_11d15_dBm = 58,		/*  58 (27.1/11.15)   */
    Volcano_rf_pa_power_59_27d1_11d2_dBm = 59,		/*  59 (27.1/11.2 )   */
    Volcano_rf_pa_power_60_27d1_11d25_dBm = 60,		/*  60 (27.1/11.25)   */
    Volcano_rf_pa_power_61_27d1_11d3_dBm = 61,		/*  61 (27.1/11.3 )   */
    Volcano_rf_pa_power_62_27d1_11d4_dBm = 62,		/*  62 (27.1/11.4 )   */
    Volcano_rf_pa_power_63_27d1_11d45_dBm = 63		/*  63 (27.1/11.45)  */
} Volcano_rf_pa_power_t, *pVolcano_rf_pa_power_t;

/* Volcano Register RF_PPA_PA_1 0x19 */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_1 = { 0x19, 0x00, 0x08, 0x00 };
/* rf_pa_power bit(s): PA/PPA output power control */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_1__rf_pa_power = { 0x19, 0x00, 0x06, 0x00 };

/* Volcano Register ATRF_PHASE_SHIFTER_2 0x17 */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_2 = { 0x17, 0x00, 0x08, 0x00 };
/* atrf_phase_selector bit(s): ATRF Phase shifter control - from 0 to 360Degre / 1.4Degre step */
static const Volcano_BitField_t gVolcano_Reg_ATRF_PHASE_SHIFTER_2__atrf_phase_selector = { 0x17, 0x00, 0x08, 0x00 };

/* Volcano Register RF_PHASE_SHIFTER_2 0x15 */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_2 = { 0x15, 0x00, 0x08, 0x00 };
/* rf_phase_selector bit(s): RF Phase shifter control - from 0 to 360Degre / 1.4Degre step */
static const Volcano_BitField_t gVolcano_Reg_RF_PHASE_SHIFTER_2__rf_phase_selector = { 0x15, 0x00, 0x08, 0x00 };


typedef enum _Volcano_rf_pa_ramp_up_Item_t {
    Volcano_rf_pa_ramp_up_0d66_us = 0,		/*  0.66   */
    Volcano_rf_pa_ramp_up_1d34_us = 1,		/*  1.34   */
    Volcano_rf_pa_ramp_up_2d01_us = 2,		/*  2.01   */
    Volcano_rf_pa_ramp_up_2d68_us = 3,		/*  2.68   */
    Volcano_rf_pa_ramp_up_5d36_us = 4,		/*  5.36   */
    Volcano_rf_pa_ramp_up_8d04_us = 5,		/*  8.04   */
    Volcano_rf_pa_ramp_up_10d72_us = 6,		/*  10.72   */
    Volcano_rf_pa_ramp_up_13d4_us = 7		/*  13.4  */
} Volcano_rf_pa_ramp_up_t, *pVolcano_rf_pa_ramp_up_t;

typedef enum _Volcano_rf_pa_bias_ctrl_Item_t {
    Volcano_rf_pa_bias_ctrl_p27dBm_150uA_300uA = 0,		/*  +27dBm / 150uA / 300uA     */
    Volcano_rf_pa_bias_ctrl_p27dBm_200uA_400uA = 1,		/*  +27dBm / 200uA / 400uA     */
    Volcano_rf_pa_bias_ctrl_p27dBm_250uA_500uA = 2,		/*  +27dBm / 250uA / 500uA     */
    Volcano_rf_pa_bias_ctrl_p21dBm_150uA_300uA = 3		/*  +21dBm / 150uA / 300uA    */
} Volcano_rf_pa_bias_ctrl_t, *pVolcano_rf_pa_bias_ctrl_t;

typedef enum _Volcano_rf_pa_det_adj_Item_t {
    Volcano_rf_pa_det_adj_1d2_V = 0,		/*  1.2   */
    Volcano_rf_pa_det_adj_1d3_V = 1,		/*  1.3   */
    Volcano_rf_pa_det_adj_1d4_V = 2,		/*  1.4   */
    Volcano_rf_pa_det_adj_1d5_V = 3		/*  1.5  */
} Volcano_rf_pa_det_adj_t, *pVolcano_rf_pa_det_adj_t;

/* Volcano Register RF_PPA_PA_2 0x1A */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_2 = { 0x1A, 0x00, 0x08, 0x00 };
/* rf_pa_ramp_up bit(s): PA ramp_up/down time control when switched ON/OFF either using SPI bit or SWITCH pin */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_2__rf_pa_ramp_up = { 0x1A, 0x04, 0x03, 0x00 };
/* rf_pa_bias_ctrl bit(s): PA 1st and 2nd stage bias current control */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_2__rf_pa_bias_ctrl = { 0x1A, 0x02, 0x02, 0x00 };
/* rf_pa_det_adj bit(s): RF detector level setting (Used to prevent high voltages peak at the output of the PA) */
static const Volcano_BitField_t gVolcano_Reg_RF_PPA_PA_2__rf_pa_det_adj = { 0x1A, 0x00, 0x02, 0x00 };


typedef enum _Volcano_LoChain_ratio_Item_t {
    Volcano_LoChain_ratio_1_2 = 0,		/*  1 / 2   */
    Volcano_LoChain_ratio_no_signal_3 = 1,		/*  no signal / 3   */
    Volcano_LoChain_ratio_2_4 = 2,		/*  2 / 4   */
    Volcano_LoChain_ratio_no_signal_6 = 3,		/*  no signal / 6   */
    Volcano_LoChain_ratio_do_not_use_F_LCvco_max = 4,		/*  do not use / F LCvco max   */
    Volcano_LoChain_ratio_F_QCCO_max_do_not_use = 5,		/*  F QCCO max / do not use   */
    Volcano_LoChain_ratio_do_not_use_1 = 6,		/*  do not use (1)   */
    Volcano_LoChain_ratio_do_not_use_2 = 7,		/*  do not use (2)   */
    Volcano_LoChain_ratio_4_8 = 8,		/*  4 / 8   */
    Volcano_LoChain_ratio_6_12 = 9,		/*  6 / 12   */
    Volcano_LoChain_ratio_8_16 = 10,		/*  8 / 16   */
    Volcano_LoChain_ratio_12_24 = 11,		/*  12 / 24   */
    Volcano_LoChain_ratio_16_32 = 12,		/*  16 / 32   */
    Volcano_LoChain_ratio_24_48 = 13,		/*  24 / 48   */
    Volcano_LoChain_ratio_30_60 = 14,		/*  30 / 60   */
    Volcano_LoChain_ratio_Daisy_in = 15		/*  Daisy in  */
} Volcano_LoChain_ratio_t, *pVolcano_LoChain_ratio_t;

/* Volcano Register LO_CHAIN 0x1B */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN = { 0x1B, 0x00, 0x08, 0x00 };
/* ckdiv2_reset bit(s): Force reset of phase shifter div 2 (used to synchronize Generator and Daisy) */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN__ckdiv2_reset = { 0x1B, 0x07, 0x01, 0x00 };
/* ckdiv2_start bit(s): Allow clock to IQ div2. Used to synchronize Generator and Daisy */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN__ckdiv2_start = { 0x1B, 0x06, 0x01, 0x00 };
/* ckdiv2_invert bit(s): Phase selection of clock to final div 2 (0/180Degre). Used to synchronise Generator and Daisy  */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN__ckdiv2_invert = { 0x1B, 0x05, 0x01, 0x00 };
/* PLL_Div2_Highfreq bit(s): PLL Div 2 High freq biasing current */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN__PLL_Div2_Highfreq = { 0x1B, 0x04, 0x01, 0x00 };
/* LoChain_ratio bit(s): Lochain dividing ratio selection */
static const Volcano_BitField_t gVolcano_Reg_LO_CHAIN__LoChain_ratio = { 0x1B, 0x00, 0x04, 0x00 };

/* Volcano Register SD_1 0x1C */
static const Volcano_BitField_t gVolcano_Reg_SD_1 = { 0x1C, 0x00, 0x08, 0x00 };
/* LO_Int bit(s): PLL integer part N */
static const Volcano_BitField_t gVolcano_Reg_SD_1__LO_Int = { 0x1C, 0x00, 0x08, 0x00 };


/* Volcano Register SD_2 0x1D */
static const Volcano_BitField_t gVolcano_Reg_SD_2 = { 0x1D, 0x00, 0x08, 0x00 };
/* LO_Frac_2 bit(s): PLL sigma-delta modulator  byte 2 <=> K[15:8] */
static const Volcano_BitField_t gVolcano_Reg_SD_2__LO_Frac_2 = { 0x1D, 0x00, 0x07, 0x00 };


/* Volcano Register SD_3 0x1E */
static const Volcano_BitField_t gVolcano_Reg_SD_3 = { 0x1E, 0x00, 0x08, 0x00 };
/* LO_Frac_1 bit(s): PLL sigma-delta modulator  byte 1 <=> K[22:9] */
static const Volcano_BitField_t gVolcano_Reg_SD_3__LO_Frac_1 = { 0x1E, 0x00, 0x08, 0x00 };


typedef enum _Volcano_cp_cursel_Item_t {
    Volcano_cp_cursel_20_uA = 0,		/*  20   */
    Volcano_cp_cursel_150_uA = 1,		/*  150   */
    Volcano_cp_cursel_210_uA = 2,		/*  210   */
    Volcano_cp_cursel_300_uA = 3,		/*  300   */
    Volcano_cp_cursel_420_uA = 4,		/*  420   */
    Volcano_cp_cursel_580_uA = 5,		/*  580   */
    Volcano_cp_cursel_800_uA = 6,		/*  800   */
    Volcano_cp_cursel_1100_uA = 7		/*  1100  */
} Volcano_cp_cursel_t, *pVolcano_cp_cursel_t;

/* Volcano Register CP 0x1F */
static const Volcano_BitField_t gVolcano_Reg_CP = { 0x1F, 0x00, 0x08, 0x00 };
/* ICP_Force_Sink bit(s): Enable / Disable Charge-pump forced sinking current */
static const Volcano_BitField_t gVolcano_Reg_CP__ICP_Force_Sink = { 0x1F, 0x06, 0x01, 0x00 };
/* ICP_Force_Src bit(s): Enable / Disable Charge-pump forced sourcing current */
static const Volcano_BitField_t gVolcano_Reg_CP__ICP_Force_Src = { 0x1F, 0x05, 0x01, 0x00 };
/* cpleak0 bit(s): Charge-pump offset current ON/OFF */
static const Volcano_BitField_t gVolcano_Reg_CP__cpleak0 = { 0x1F, 0x04, 0x01, 0x00 };
/* cpleak05 bit(s): Proportional CP offset current */
static const Volcano_BitField_t gVolcano_Reg_CP__cpleak05 = { 0x1F, 0x03, 0x01, 0x00 };
/* cp_cursel bit(s): Charge-pump current value */
static const Volcano_BitField_t gVolcano_Reg_CP__cp_cursel = { 0x1F, 0x00, 0x03, 0x00 };


/* Volcano Register SD_TEST 0x20 */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST = { 0x20, 0x00, 0x08, 0x00 };
/* LOSD_clock bit(s): PLL Sigma delta modulator clock Enable/disable => freeze modulator output */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_clock = { 0x20, 0x06, 0x01, 0x00 };
/* LOSD_Static_N bit(s): PLL Sigma delta modulator bypass => Integer PLL */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_Static_N = { 0x20, 0x05, 0x01, 0x00 };
/* LOSD_BascD bit(s): Only for test purpose */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_BascD = { 0x20, 0x04, 0x01, 0x00 };
/* LOSD_dpa2 bit(s): Only for test purpose */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_dpa2 = { 0x20, 0x03, 0x01, 0x00 };
/* LOSD_dpa1 bit(s): Only for test purpose */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_dpa1 = { 0x20, 0x02, 0x01, 0x00 };
/* LOSDrstN bit(s): Sigma delta PLL modulator reset NOT */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSDrstN = { 0x20, 0x01, 0x01, 0x00 };
/* LOSD_test_En bit(s): Sigma delta PLL modulator test mode enable */
static const Volcano_BitField_t gVolcano_Reg_SD_TEST__LOSD_test_En = { 0x20, 0x00, 0x01, 0x00 };


typedef enum _Volcano_VCO_currentctrl_Item_t {
    Volcano_VCO_currentctrl_416_uA = 0,		/*  416   */
    Volcano_VCO_currentctrl_400_uA = 1,		/*  400   */
    Volcano_VCO_currentctrl_383_uA = 2,		/*  383   */
    Volcano_VCO_currentctrl_366_uA = 3,		/*  366   */
    Volcano_VCO_currentctrl_350_uA = 4,		/*  350   */
    Volcano_VCO_currentctrl_333_uA = 5,		/*  333   */
    Volcano_VCO_currentctrl_317_uA = 6,		/*  317   */
    Volcano_VCO_currentctrl_300_uA = 7,		/*  300   */
    Volcano_VCO_currentctrl_286_uA = 8,		/*  286   */
    Volcano_VCO_currentctrl_269_uA = 9,		/*  269   */
    Volcano_VCO_currentctrl_252_uA = 10,		/*  252   */
    Volcano_VCO_currentctrl_235_uA = 11,		/*  235   */
    Volcano_VCO_currentctrl_219_uA = 12,		/*  219   */
    Volcano_VCO_currentctrl_202_uA = 13,		/*  202   */
    Volcano_VCO_currentctrl_185_uA = 14,		/*  185   */
    Volcano_VCO_currentctrl_168_uA = 15		/*  168  */
} Volcano_VCO_currentctrl_t, *pVolcano_VCO_currentctrl_t;

typedef enum _Volcano_VCO_Select_Item_t {
    Volcano_VCO_Select_All_OFF = 0,		/*  All OFF   */
    Volcano_VCO_Select_QCCO_ON = 1,		/*  QCCO ON   */
    Volcano_VCO_Select_VCO_MID = 2,		/*  VCO MID   */
    Volcano_VCO_Select_VCO_HIGH = 3		/*  VCO HIGH  */
} Volcano_VCO_Select_t, *pVolcano_VCO_Select_t;

/* Volcano Register VCO 0x21 */
static const Volcano_BitField_t gVolcano_Reg_VCO = { 0x21, 0x00, 0x08, 0x00 };
/* VCO_currentctrl bit(s): VCO bias current control */
static const Volcano_BitField_t gVolcano_Reg_VCO__VCO_currentctrl = { 0x21, 0x03, 0x04, 0x00 };
/* VCO_v2iboost bit(s): VCO/QCCO output buffer current control */
static const Volcano_BitField_t gVolcano_Reg_VCO__VCO_v2iboost = { 0x21, 0x02, 0x01, 0x00 };
/* VCO_Select bit(s): VCO type and Band selection */
static const Volcano_BitField_t gVolcano_Reg_VCO__VCO_Select = { 0x21, 0x00, 0x02, 0x00 };


typedef enum _Volcano_qcco_curboost_Item_t {
    Volcano_qcco_curboost_290_uA = 0,		/*  290   */
    Volcano_qcco_curboost_320_uA = 1,		/*  320   */
    Volcano_qcco_curboost_350_uA = 2,		/*  350   */
    Volcano_qcco_curboost_375_uA = 3		/*  375  */
} Volcano_qcco_curboost_t, *pVolcano_qcco_curboost_t;

/* Volcano Register QCCO 0x22 */
static const Volcano_BitField_t gVolcano_Reg_QCCO = { 0x22, 0x00, 0x08, 0x00 };
/* qcco_curboost bit(s): QCCO bias current control */
static const Volcano_BitField_t gVolcano_Reg_QCCO__qcco_curboost = { 0x22, 0x00, 0x02, 0x00 };


typedef enum _Volcano_RF_Reg_Item_t {
    Volcano_RF_Reg_2_6_V = 0,		/*  2,6   */
    Volcano_RF_Reg_2_8_V = 1,		/*  2,8   */
    Volcano_RF_Reg_3_0_V = 2,		/*  3,0   */
    Volcano_RF_Reg_3_0_2_V = 3		/*  3,0 (2)  */
} Volcano_RF_Reg_t, *pVolcano_RF_Reg_t;

typedef enum _Volcano_Dig_Reg_Item_t {
    Volcano_Dig_Reg_1_6_normal_drive_V = 0,		/*  1,6 normal drive   */
    Volcano_Dig_Reg_1_8_normal_drive_V = 1,		/*  1,8 normal drive   */
    Volcano_Dig_Reg_1_8_high_drive_V = 2,		/*  1,8 high drive   */
    Volcano_Dig_Reg_2_0_normal_drive_V = 3		/*  2,0 normal drive  */
} Volcano_Dig_Reg_t, *pVolcano_Dig_Reg_t;

typedef enum _Volcano_VCO_Reg_Item_t {
    Volcano_VCO_Reg_2_6_V = 0,		/*  2,6   */
    Volcano_VCO_Reg_2_8_V = 1,		/*  2,8   */
    Volcano_VCO_Reg_3_0_V = 2,		/*  3,0   */
    Volcano_VCO_Reg_3_0_2_V = 3		/*  3,0 (2)  */
} Volcano_VCO_Reg_t, *pVolcano_VCO_Reg_t;

/* Volcano Register REG 0x23 */
static const Volcano_BitField_t gVolcano_Reg_REG = { 0x23, 0x00, 0x08, 0x00 };
/* RF_Reg bit(s): RF part + .2.8V. level-shifters regulator output voltage control */
static const Volcano_BitField_t gVolcano_Reg_REG__RF_Reg = { 0x23, 0x04, 0x02, 0x00 };
/* Dig_Reg bit(s): Digital Regulator voltage setting */
static const Volcano_BitField_t gVolcano_Reg_REG__Dig_Reg = { 0x23, 0x02, 0x02, 0x00 };
/* VCO_Reg bit(s): VCO/QCCO regulator output voltage control */
static const Volcano_BitField_t gVolcano_Reg_REG__VCO_Reg = { 0x23, 0x00, 0x02, 0x00 };


/* Volcano Register POWERDOWN 0x28 */
static const Volcano_BitField_t gVolcano_Reg_POWERDOWN = { 0x28, 0x00, 0x08, 0x00 };
/* force_soft_reset bit(s): Enables/Disables the Software reset */
static const Volcano_BitField_t gVolcano_Reg_POWERDOWN__force_soft_reset = { 0x28, 0x01, 0x01, 0x00 };
/* Soft_reset bit(s): trigs a software reset (if force_soft_reset = 0) */
static const Volcano_BitField_t gVolcano_Reg_POWERDOWN__Soft_reset = { 0x28, 0x00, 0x01, 0x00 };


/* Volcano Register PROGR_REG 0x2B */
static const Volcano_BitField_t gVolcano_Reg_PROGR_REG = { 0x2B, 0x00, 0x08, 0x00 };
/* Update_prg bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_PROGR_REG__Update_prg = { 0x2B, 0x00, 0x01, 0x00 };


/* Volcano Register LAUNCH 0x2C */
static const Volcano_BitField_t gVolcano_Reg_LAUNCH = { 0x2C, 0x00, 0x08, 0x00 };
/* atrf_Launch bit(s): ATRF launch */
static const Volcano_BitField_t gVolcano_Reg_LAUNCH__atrf_Launch = { 0x2C, 0x02, 0x01, 0x00 };
/* fvco_max_Launch bit(s): ??? */
static const Volcano_BitField_t gVolcano_Reg_LAUNCH__fvco_max_Launch = { 0x2C, 0x01, 0x01, 0x00 };
/* XtalCal_Launch bit(s): Xtal Cal Launch */
static const Volcano_BitField_t gVolcano_Reg_LAUNCH__XtalCal_Launch = { 0x2C, 0x00, 0x01, 0x00 };


/* Volcano Register RESERVED_0 0x3E */
static const Volcano_BitField_t gVolcano_Reg_RESERVED_0 = { 0x3E, 0x00, 0x08, 0x00 };
/* reserved_0 bit(s): Reserved for digital debug */
static const Volcano_BitField_t gVolcano_Reg_RESERVED_0__reserved_0 = { 0x3E, 0x00, 0x05, 0x00 };


/* Volcano Register RESERVED_1 0x3F */
static const Volcano_BitField_t gVolcano_Reg_RESERVED_1 = { 0x3F, 0x00, 0x08, 0x00 };
/* reserved_1 bit(s): Page selection */
static const Volcano_BitField_t gVolcano_Reg_RESERVED_1__reserved_1 = { 0x3F, 0x00, 0x02, 0x00 };



/*============================================================================*/
/* End of registers definitions:                                              */
/*============================================================================*/

/*============================================================================*/
/* Volcano Types and defines:                                               */
/*============================================================================*/

typedef struct _VolcanoObject_t
{
    tmUnitSelect_t                  tUnit;
    tmUnitSelect_t                  tUnitD;
    IoFunc_t           sIo;
    TimeFunc_t         sTime;
    Volcano_PowerStateMode_t  PowerStateMode;
    UInt32             RF;
    UInt32             FVCO;
    UInt8                RegMap[Volcano_REG_MAP_NB_BYTES];
} VolcanoObject_t, *pVolcanoObject_t, **ppVolcanoObject_t;

/*============================================================================*/
/* Static variables:                                                          */
/*============================================================================*/
/* Driver instance */
static VolcanoObject_t gVolcanoInstance[Volcano_UNITS] = 
{
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* Unit not set */
        {                                   /* sIo */
            Null,
            Null
        },
        {                                   /* sTime */
            Null
        },
        Volcano_standby,   /* state standby */
        0,        /* RF */
        0,        /* FCVCO */
        Volcano_POR /* Reg Map POR */
    },
#if Volcano_UNITS>=2
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* Unit not set */
        {                                   /* sIo */
            Null,
            Null
        },
        {                                   /* sTime */
            Null
        },
        Volcano_standby,   /* state standby */
        0,        /* RF */
        0,        /* FCVCO */
        Volcano_POR /* Reg Map POR */
    },
#endif
#if Volcano_UNITS>=3
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* Unit not set */
        {                                   /* sIo */
            Null,
            Null
        },
        {                                   /* sTime */
            Null
        },
        Volcano_standby,   /* state standby */
        0,        /* RF */
        0,        /* FCVCO */
        Volcano_POR /* Reg Map POR */
    },
#endif
#if Volcano_UNITS>=4
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* Unit not set */
        {                                   /* sIo */
            Null,
            Null
        },
        {                                   /* sTime */
            Null
        },
        Volcano_standby,   /* state standby */
        0,        /* RF */
        0,        /* FCVCO */
        Volcano_POR /* Reg Map POR */
    }
#endif
};

/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/
tmErrorCode_t
iVolcano_AllocInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    ppVolcanoObject_t  ppDrvObject /* I: Driver Object */
);
tmErrorCode_t
iVolcano_GetInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    ppVolcanoObject_t  ppDrvObject /* I: Driver Object */
);
tmErrorCode_t
iVolcano_CalculateNIntKInt(
    pVolcanoObject_t   pObj,    /* I: Driver object */
    UInt32  uFVCO,
    UInt32* NInt, 
    UInt32* KInt
);
tmErrorCode_t
iVolcano_Write(
    pVolcanoObject_t           pObj,       /* I: Driver object */
    const Volcano_BitField_t*  pBitField, /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    BusAccess_t                eBusAccess  /* I: Access to bus */
);
tmErrorCode_t
iVolcano_Read(
    pVolcanoObject_t           pObj,       /* I: Driver object */
    const Volcano_BitField_t*  pBitField, /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    BusAccess_t                eBusAccess  /* I: Access to bus */
);
tmErrorCode_t 
iVolcano_Wait(
    pVolcanoObject_t   pObj,   /* I: Driver object */
    UInt32              Time    /* I: time to wait for */
);
#ifdef __cplusplus
}
#endif


