/*
  Copyright (C) 2013 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          Volcano.c
 *
 *                %version:%
 *
 * \date          %modify_time%
 *
 * \author        Michael VANNIER
 *                
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 *
 * \section info Change Information
 *
*/

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
//#ifndef WIN32  // LPC env
//#include "lpc_types.h"
//#include "board.h"
//#endif

#include "tmNxTypes.h"
#include "Volcano.h"
#include "Volcano_local.h"
//#include "tmbslFrontEndCfgItem.h"
//#include "tmCompId.h"
//#include "tmFrontEnd.h"
//#include "tmUnitParams.h"
//#include "tmbslFrontEndTypes.h"






/*============================================================================*/
/* Static variables:                                                          */
/*============================================================================*/
typedef struct _PowerPA_Prog_def_
{
    Int16  PAout;  /* in 10*dBm */
    UInt8  PAgain_prog;
    UInt8  PAbias_prog;
} PowerPA_Prog_def;

#define PowerPA_Prog_length  30
static PowerPA_Prog_def  PowerPA_Prog[PowerPA_Prog_length] =
{
{260, 30, 0},
{255, 25, 0},
{250, 22, 0},
{245, 20, 0},
{240, 18, 0},
{235, 16, 0},
{230, 15, 0},
{225, 14, 0},
{220, 13, 0},
{215, 12, 0},
{210, 11, 0},
{205, 10, 0},
{200, 10, 0},
{195, 9, 0},
{185, 8, 0},
{180, 49, 3},
{175, 27, 3},
{170, 20, 3},
{165, 17, 3},
{160, 15, 3},
{155, 13, 3},
{150, 11, 3},
{145, 10, 3},
{140, 9, 3},
{135, 8, 3},
{125, 7, 3},
{115, 6, 3},
{105, 5, 3},
{90, 4, 3},
{70, 3, 3}
};

static double PhaseCoding[256]={ \
		0, \
		1.43, \
		2.86, \
		4.29, \
		5.71, \
		7.14, \
		8.57, \
		10, \
		11.43, \
		12.86, \
		14.29, \
		15.71, \
		17.14, \
		18.57, \
		20, \
		21.43, \
		22.86, \
		24.29, \
		25.71, \
		27.14, \
		28.57, \
		30, \
		31.43, \
		32.86, \
		34.29, \
		35.71, \
		37.14, \
		38.57, \
		40, \
		41.43, \
		42.86, \
		44.29, \
		45.71, \
		47.14, \
		48.57, \
		50, \
		51.43, \
		52.86, \
		54.29, \
		55.71, \
		57.14, \
		58.57, \
		60, \
		61.43, \
		62.86, \
		64.29, \
		65.71, \
		67.14, \
		68.57, \
		70, \
		71.43, \
		72.86, \
		74.29, \
		75.71, \
		77.14, \
		78.57, \
		80, \
		81.43, \
		82.86, \
		84.29, \
		85.71, \
		87.14, \
		88.57, \
		90, \
		90, \
		91.43, \
		92.86, \
		94.29, \
		95.71, \
		97.14, \
		98.57, \
		100, \
		101.43, \
		102.86, \
		104.29, \
		105.71, \
		107.14, \
		108.57, \
		110, \
		111.43, \
		112.86, \
		114.29, \
		115.71, \
		117.14, \
		118.57, \
		120, \
		121.43, \
		122.86, \
		124.29, \
		125.71, \
		127.14, \
		128.57, \
		130, \
		131.43, \
		132.86, \
		134.29, \
		135.71, \
		137.14, \
		138.57, \
		140, \
		141.43, \
		142.86, \
		144.29, \
		145.71, \
		147.14, \
		148.57, \
		150, \
		151.43, \
		152.86, \
		154.29, \
		155.71, \
		157.14, \
		158.57, \
		160, \
		161.43, \
		162.86, \
		164.29, \
		165.71, \
		167.14, \
		168.57, \
		170, \
		171.43, \
		172.86, \
		174.29, \
		175.71, \
		177.14, \
		178.57, \
		180, \
		180, \
		181.43, \
		182.86, \
		184.29, \
		185.71, \
		187.14, \
		188.57, \
		190, \
		191.43, \
		192.86, \
		194.29, \
		195.71, \
		197.14, \
		198.57, \
		200, \
		201.43, \
		202.86, \
		204.29, \
		205.71, \
		207.14, \
		208.57, \
		210, \
		211.43, \
		212.86, \
		214.29, \
		215.71, \
		217.14, \
		218.57, \
		220, \
		221.43, \
		222.86, \
		224.29, \
		225.71, \
		227.14, \
		228.57, \
		230, \
		231.43, \
		232.86, \
		234.29, \
		235.71, \
		237.14, \
		238.57, \
		240, \
		241.43, \
		242.86, \
		244.29, \
		245.71, \
		247.14, \
		248.57, \
		250, \
		251.43, \
		252.86, \
		254.29, \
		255.71, \
		257.14, \
		258.57, \
		260, \
		261.43, \
		262.86, \
		264.29, \
		265.71, \
		267.14, \
		268.57, \
		270, \
		270, \
		271.43, \
		272.86, \
		274.29, \
		275.71, \
		277.14, \
		278.57, \
		280, \
		281.43, \
		282.86, \
		284.29, \
		285.71, \
		287.14, \
		288.57, \
		290, \
		291.43, \
		292.86, \
		294.29, \
		295.71, \
		297.14, \
		298.57, \
		300, \
		301.43, \
		302.86, \
		304.29, \
		305.71, \
		307.14, \
		308.57, \
		310, \
		311.43, \
		312.86, \
		314.29, \
		315.71, \
		317.14, \
		318.57, \
		320, \
		321.43, \
		322.86, \
		324.29, \
		325.71, \
		327.14, \
		328.57, \
		330, \
		331.43, \
		332.86, \
		334.29, \
		335.71, \
		337.14, \
		338.57, \
		340, \
		341.43, \
		342.86, \
		344.29, \
		345.71, \
		347.14, \
		348.57, \
		350, \
		351.43, \
		352.86, \
		354.29, \
		355.71, \
		357.14, \
		358.57, \
		360 };

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    Volcano_Open:                                                 */
/*                                                                            */
/* DESCRIPTION: Opens driver setup environment.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_Open(
     tmUnitSelect_t              tUnit,      /* I: "Main Volcano" device number */
     tmUnitSelect_t              tUnitD,      /* I: "Volcano driver" device  number */
     SysDependency_t*  psSrvFunc   /* I: setup parameters */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Test parameter(s) */
    if(psSrvFunc == Null)
    {
        err = !TM_OK;
    }

    /* get pointer in instance array */
    err = iVolcano_AllocInstance(tUnit , &pObj);
    pObj->tUnitD = tUnitD;

    /* map sys function */
    P_SIO = psSrvFunc->sIo;
    P_STIME = psSrvFunc->sTime;

    return err;
}
/*============================================================================*/
/* FUNCTION:    Volcano_Close                                                 */
/*                                                                            */
/* DESCRIPTION: Opens driver setup environment.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_Close(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);

    pObj->tUnit = -1;

    return err;
}
/*============================================================================*/
/* FUNCTION:    Volcano_HwInit:                                               */
/*                                                                            */
/* DESCRIPTION: Initializes VolcanoHardware.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_HwInit(
    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    Volcano__ModeMS_t    uModeMS /* I:  IC used in Mater or Slave */     
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 nbloop =0;
    UInt8 mapPOR[Volcano_REG_MAP_NB_BYTES] = Volcano_POR; /* Reg Map POR */

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);

    if(err == TM_OK)
    {
        /* enable reset of SPI register to POR*/
        err = iVolcano_Write(pObj, &gVolcano_Reg_RESERVED_0__reserved_0, 0x00, eBus_NoRead);
    }
    if(err == TM_OK)
    {
        iVolcano_Write(pObj, &gVolcano_Reg_POWERDOWN__force_soft_reset, 0x01, eBus_None); 
        err = iVolcano_Write(pObj, &gVolcano_Reg_POWERDOWN__Soft_reset, 0x01, eBus_NoRead); 
    }
    if(err == TM_OK)
    {
        /* update regmap to POR */
        for (nbloop=1; nbloop<=Volcano_REG_MAP_NB_BYTES; nbloop++) pObj->RegMap[nbloop] = mapPOR[nbloop];
        iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__PD_Synthe, 0x00, eBus_None); 
        err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_lochain, 0x00, eBus_NoRead); 
    }
    if(err == TM_OK)
    {
        iVolcano_Wait(pObj, 1);
        err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
        iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
    }

    /* POR overwrite */
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_VCO__VCO_currentctrl, Volcano_VCO_currentctrl_252_uA, eBus_NoRead); 
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_CP__cp_cursel, Volcano_cp_cursel_1100_uA, eBus_NoRead); 
    }
    if(err == TM_OK)
    {
#if (BOARD_NXP_OM15004C2)
        /* fix gain out to 27 - 10 = 17 dBm */
        iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_2__rf_pa_bias_ctrl, PowerPA_Prog[17].PAbias_prog, eBus_None);
#else
        iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_2__rf_pa_bias_ctrl, Volcano_rf_pa_bias_ctrl_p27dBm_250uA_500uA, eBus_None);
#endif
        iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_2__rf_pa_ramp_up, Volcano_rf_pa_ramp_up_0d66_us, eBus_None);
        err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_2__rf_pa_det_adj, Volcano_rf_pa_det_adj_1d5_V, eBus_NoRead); 
    }    
    if(err == TM_OK)
    {
#if (BOARD_NXP_OM15004C2)
    	/* fix gain out to 27 - 10 = 17 dBm */
        err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_1__rf_pa_power, PowerPA_Prog[17].PAgain_prog, eBus_NoRead);
#else
        err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_1__rf_pa_power, Volcano_rf_pa_power_2_5d8_n16d3_dBm, eBus_NoRead); 
#endif
    }     
    if(err == TM_OK)
    {
        err = Volcano_SetPowerStateMode(tUnit, Volcano_standby); /* ensure chip in Standby */
    }

#if (BOARD_NXP_OM15000 || BOARD_NXP_SANGO4RF)
    /* enable Daisy chaining on associated "Volcano driver" */
    if(err == TM_OK)
    {
        UInt8   OpCommand[2] = {0x10,0x00};
        if(P_SIO_WRITE_VALID)
        {
            /* enable Daisy chaining */
            err = P_SIO_WRITE(pObj->tUnitD, 16, &OpCommand[0]);
            if(err == TM_OK)
            {
            	/* change daisy IN & OUT settings to improve output stability */
            	OpCommand[0] = 0x13;
            	OpCommand[1] = 0x0F;
            	err = P_SIO_WRITE(pObj->tUnitD, 16, &OpCommand[0]);
            }
            if(err == TM_OK)
            {
            	/* update PROG  */
            	OpCommand[0] = 0x2B;
            	OpCommand[1] = 0x01;
            	err = P_SIO_WRITE(pObj->tUnitD, 16, &OpCommand[0]);
            }
        }
    } 
#endif

    if(err == TM_OK)
    {
       err = iVolcano_Write(pObj, &gVolcano_Reg_XT_CAL_2__XT_In_Force, 1, eBus_NoRead);
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    Volcano_SetRF:                                                */
/*                                                                            */
/* DESCRIPTION: Tunes to a RF.                                                */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_SetRF(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     UInt32          uLO,     /* I: LO frequency in kHz */
     Bool            bBlanking /* PA in Power down during SetRF */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
	UInt32 lodiv = 0;
    UInt32 NInt = 0;
	UInt32 KInt = 0;
    UInt8 vco_select = 0;
    UInt8 pll_div2_highfreq = 0;
    UInt8 lochain_ratio = 0;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        pObj->RF = uLO;

        if ((uLO/1000) > 1666)
		    lodiv = 4;
	    else if ((uLO/1000) > 1250)
		    lodiv = 6;
	    else if ((uLO/1000) > 833)
		    lodiv = 8;
	    else if ((uLO/1000) > 625)
		    lodiv = 12;
	    else if ((uLO/1000) >416)
		    lodiv = 16;
	    else if ((uLO/1000) > 312)
		    lodiv = 24;
	    else if ((uLO/1000) > 208)
		    lodiv = 32;
	    else if ((uLO/1000) > 156)
		    lodiv = 48;
        else 
            lodiv = 64;
        pObj->FVCO = lodiv * uLO;

        if(err == TM_OK)
        {
            err = iVolcano_CalculateNIntKInt(pObj, pObj->FVCO, &NInt, &KInt); 
        }

	    /* VCO Selection: fixed valu => should be replaced by result of LOcalib <=> new parameter to this function to be added */
	    if ((uLO*lodiv/1000) <= 8500)
		    vco_select = Volcano_VCO_Select_VCO_MID;
	    else
		    vco_select = Volcano_VCO_Select_VCO_HIGH;
	    /* PLL Div2 Highfreq */
	    if ((uLO*lodiv/1000) <= 8000)
		    pll_div2_highfreq = 0;
	    else
		    pll_div2_highfreq = 1;
	    /* LO Chain ratio */
	    switch(lodiv)
	    {
		    case 4:
			    lochain_ratio = 0;
		    break;
		    case 6:
			    lochain_ratio = 1;
		    break;
		    case 8:
			    lochain_ratio = 2;
		    break;
		    case 12:
			    lochain_ratio = 3;
		    break;
		    case 16:
			    lochain_ratio = 8;
		    break;
		    case 24:
			    lochain_ratio = 9;
		    break;
		    case 32:
			    lochain_ratio = 10;
		    break;
		    case 48:
			    lochain_ratio = 11;
		    break;
		    case 64:
			    lochain_ratio = 12;
		    break;
		    default:
			    lochain_ratio = 15;
		    break;
	    } 

        if(err == TM_OK && bBlanking==True)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_pa, 0x01, eBus_NoRead);

            if(err == TM_OK)
            {
                err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
                iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None);
            }
        }

        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_VCO__VCO_Select, vco_select, eBus_NoRead); 
        }
        if(err == TM_OK)
        {
            iVolcano_Write(pObj, &gVolcano_Reg_CP__ICP_Force_Src, 0x00, eBus_None); 
            err = iVolcano_Write(pObj, &gVolcano_Reg_CP__ICP_Force_Sink, 0x00, eBus_NoRead); 
        }  
        if(err == TM_OK)
        {
            iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__PLL_Div2_Highfreq, pll_div2_highfreq, eBus_None); 
            err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__LoChain_ratio, lochain_ratio, eBus_NoRead); 
        }  
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_SD_TEST__LOSD_Static_N, 0x00, eBus_NoRead); 
        }
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_SD_2__LO_Frac_2, (UInt8)((KInt >> 16) & 0xFF), eBus_NoRead);
        }
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_SD_3__LO_Frac_1, (UInt8)((KInt >> 8) & 0xFF), eBus_NoRead);
        }
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_SD_1__LO_Int, (UInt8)(NInt & 0xFF), eBus_NoRead);
        }
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
            iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
        }
        if(err == TM_OK && bBlanking==True)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_pa, 0x00, eBus_NoRead);
            if(err == TM_OK)
            {
                err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
                iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
            }
        }
    }

    return err;
}
/*============================================================================*/
/* FUNCTION:         Volcano_ResetSynchro                                     */
/*                                                                            */
/* DESCRIPTION: Activate LO_CHAIN  synchro                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_ResetSynchro(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_start, 0x00, eBus_None);	
        err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_reset, 0x01, eBus_NoRead);	
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
            iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_SetSynchroMaster                                 */
/*                                                                            */
/* DESCRIPTION: Activate LO_CHAIN  synchro  for Master                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_SetSynchroMaster(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_reset, 0x00, eBus_NoRead);	
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
        iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_start, 0x01, eBus_NoRead);	
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
        iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_SetSynchroSlave                                  */
/*                                                                            */
/* DESCRIPTION: Activate synchro for Slave                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_SetSynchroSlave(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_reset, 0x00, eBus_NoRead);	
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
        iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
    }
    return err;
}
/*============================================================================*/
/* FUNCTION:         Volcano_SetPowerStateMode                                */
/*                                                                            */
/* DESCRIPTION: Handle block PD according desired state mode                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/

tmErrorCode_t
Volcano_SetPowerStateMode(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     Volcano_PowerStateMode_t  PowerStateMode/* I: power state mode */
    )
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
		switch(PowerStateMode)
		{
			case Volcano_standby:  /* standby */
                iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_start, 0x00, eBus_None);	
                err = iVolcano_Write(pObj, &gVolcano_Reg_LO_CHAIN__ckdiv2_reset, 0x01, eBus_NoRead);	
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1, 0xFF, eBus_NoRead);	
                }
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28, 0x00, eBus_None);	
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_out, 0x01, eBus_None);	
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_in, 0x01, eBus_NoRead);	
                }
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy, 0x00, eBus_NoRead);
                }
			break;
			case Volcano_generator_no_daisy:  /* Generator_no_daisy */
			case Volcano_generator_no_daisy_PPA:  /* Generator_no_daisy with PPA*/
               // iVolcano_Write(pObj, &gVolcano_Reg_XT_CAL_2__XT_Cal_Bypass, 0x01, eBus_None);				
              //  err = iVolcano_Write(pObj, &gVolcano_Reg_XT_CAL_2__XT_Dac_Force, Volcano_XT_Dac_Force_15, eBus_NoRead);
              ///  {
               //     iVolcano_Wait(pObj, 100);
             //   }
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1, 0x00, eBus_None);				
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_lotest, 0x01, eBus_None);								
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__PD_adc_vtune, 0x01, eBus_None);								
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_ppa, (PowerStateMode==Volcano_generator_no_daisy_PPA ? 0x00 : 0x01), eBus_None);								
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_atrf, 0x01, eBus_NoRead);	
                }
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28, 0x00, eBus_None);	
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_out, 0x01, eBus_None);	
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_in, 0x01, eBus_NoRead);
                }
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy, 0x00, eBus_NoRead);
                }
			break;
			case Volcano_generator_daisy:  /* Generator_daisy */
			case Volcano_generator_daisy_PPA:  /* Generator_daisy with PPA */
                 if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1, 0x00, eBus_None);				
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_lotest, 0x01, eBus_None);								
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__PD_adc_vtune, 0x01, eBus_None);								
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_ppa, (PowerStateMode==Volcano_generator_daisy_PPA ? 0x00 : 0x01), eBus_None);								
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_atrf, 0x01, eBus_NoRead);
                }
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28, 0x00, eBus_None);	
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_out, 0x00, eBus_None);	
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_in, 0x01, eBus_NoRead);	
                }
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy, 0x01, eBus_NoRead);
                }
			break;
			case Volcano_slave_daisy: /* Slave_daisy */
			case Volcano_slave_daisy_PPA: /* Slave_daisy with PPA */
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1, 0xFF, eBus_None);				
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_pa, 0x00, eBus_None);								
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_lochain, 0x00, eBus_None);								
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_ppa, (PowerStateMode==Volcano_slave_daisy_PPA ? 0x00 : 0x01), eBus_None);								
                err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_tx, 0x00, eBus_NoRead);	
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28, 0x00, eBus_None);	
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_out, 0x00, eBus_None);	
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_in, 0x00, eBus_NoRead);
                }
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy, 0x01, eBus_NoRead);
                }
			break;
			case Volcano_slave_no_daisy: /* Slave_no_daisy */
			case Volcano_slave_no_daisy_PPA: /* Slave_no_daisy with PPA */
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1, 0xFF, eBus_None);				
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_pa, 0x00, eBus_None);								
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_lochain, 0x00, eBus_None);								
                iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_ppa, (PowerStateMode==Volcano_slave_no_daisy_PPA ? 0x00 : 0x01), eBus_None);								
                err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_rf_tx, 0x00, eBus_NoRead);
                if(err == TM_OK)
                {
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_rf_reg28, 0x00, eBus_None);	
                    iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_out, 0x01, eBus_None);	
                    err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_2__pd_daisy_in, 0x00, eBus_NoRead);
                }
                if(err == TM_OK)
                {
                    err = iVolcano_Write(pObj, &gVolcano_Reg_DAISY_CHAIN_1__lomux_to_daisy, 0x00, eBus_NoRead);
                }
			break;
            default:
                err= !TM_OK;
			break;
        }
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x01, eBus_NoRead);
            iVolcano_Write(pObj, &gVolcano_Reg_PROGR_REG__Update_prg, 0x00, eBus_None); 
        }

        if (err==TM_OK)
        {
            pObj->PowerStateMode = PowerStateMode;
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_SetRFPhase                                       */
/*                                                                            */
/* DESCRIPTION: Set RF Phase                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_SetRFPhase(
     tmUnitSelect_t  tUnit,      /* I: FrontEnd unit number */
     UInt16    Phase               /* I: Phase in ° */
    )
{
    UInt8 index=0;

    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        /* seach lookup table */
        while (PhaseCoding[index] < Phase && index < 255) index = index +1;
        if (index!=0)
        {
    	    /* choose nearest value to requested Phase */
    	    if (  Phase - PhaseCoding[index-1] <  PhaseCoding[index] - Phase) index = index -1;
        }
        err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PHASE_SHIFTER_2__rf_phase_selector, index, eBus_NoRead);
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_SetRFAtten                                       */
/*                                                                            */
/* DESCRIPTION: Set RF PA power in respect of Atten request                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_SetRFAtten(
     tmUnitSelect_t tUnit,      /* I: FrontEnd unit number */
     UInt16    Atten            /* I: RF atten in 1/256 dB steps */
     )
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 index=0;
    Int16 PA_out_dBm_base_per10 = 0;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    /* convert Atten 1/256dB steps to Gain PA in dBm * 10 */
    PA_out_dBm_base_per10 = 270  -(Atten * 10 / 256);

    /* search for PA prog in lookup table*/
    if(err == TM_OK)
    {
        while ( PowerPA_Prog[index].PAout > PA_out_dBm_base_per10 && index < PowerPA_Prog_length-1) index = index +1;
        if (index!=0)
        {
    	    /* choose nearest value to requested gain */
    	    if (  PA_out_dBm_base_per10 - PowerPA_Prog[index].PAout >  PowerPA_Prog[index-1].PAout - PA_out_dBm_base_per10) index = index -1;
        }
        err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_1__rf_pa_power, PowerPA_Prog[index].PAgain_prog, eBus_NoRead);
        if(err == TM_OK)
        {
            err = iVolcano_Write(pObj, &gVolcano_Reg_RF_PPA_PA_2__rf_pa_bias_ctrl, PowerPA_Prog[index].PAbias_prog, eBus_NoRead);
        }    
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_GetPowerStateMode                                */
/*                                                                            */
/* DESCRIPTION: Get current Power State Mode                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_GetPowerStateMode(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     Volcano_PowerStateMode_t*  pPowerStateMode  /* O: power state mode */
    )
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);    
    
    if(err == TM_OK)
    {
        *pPowerStateMode = pObj->PowerStateMode;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_GetAtrfPhase                                     */
/*                                                                            */
/* DESCRIPTION: launch and get back ATRF phase in steps of 1.4                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_GetAtrfPhase (
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    UInt32*  pATRF_Phase  /* O: atrf_phase */
                  )
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uVal1 = 0;
    UInt8 uVal2 = 0;
    UInt8 nbloop = 0;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);
    
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_IRQ_clear__atrf_Clear, 0x01, eBus_NoRead);
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_POWER_DOWN_1__pd_atrf, 0x00, eBus_NoRead);
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_LAUNCH__atrf_Launch, 0x01, eBus_NoRead); 
    }
    while ( (err==TM_OK) && (uVal1==0) && (nbloop != 100))
    {
		nbloop ++;
        iVolcano_Wait(pObj, 100);
        err = iVolcano_Read(pObj, &gVolcano_Reg_IRQ_status__atrf_End, &uVal1, eBus_NoWrite);
	}
    if(err == TM_OK)
    {
        err = ( uVal1==0 ? !TM_OK : TM_OK );
    }
    if(err == TM_OK)
    {
        err = Volcano_ReadRegMap( tUnit, gVolcano_Reg_VCO_LOG__atrf_phase_read_1.Address, 2 );
        iVolcano_Read(pObj, &gVolcano_Reg_ATRF_PHASE_SHIFTER_1__atrf_phase_read_2, &uVal2, eBus_None);
        iVolcano_Read(pObj, &gVolcano_Reg_VCO_LOG__atrf_phase_read_1, &uVal1, eBus_None);
    }
    *pATRF_Phase = (uVal1 << 8) + uVal2;

    return err;
}
/*============================================================================*/
/* FUNCTION:    Volcano__GetThermo:                                           */
/*                                                                            */
/* DESCRIPTION: returns IC temperature in  °C                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_GetThermo(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pThermo     /* O: temperature in °C */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);

    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_THERMO_2__TM_ON, 0x01, eBus_NoRead);
    }
    if(err == TM_OK)
    {
        err = iVolcano_Read(pObj, &gVolcano_Reg_THERMO_1__TM_D, pThermo, eBus_NoWrite);
    }
    if(err == TM_OK)
    {
        err = iVolcano_Write(pObj, &gVolcano_Reg_THERMO_2__TM_ON, 0x00, eBus_NoRead);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:         Volcano_Write                                            */
/*                                                                            */
/* DESCRIPTION: Writes in Volcano hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const Volcano_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData      /* I: Data to write */
)
{
    return iVolcano_Write(&gVolcanoInstance[tUnit], pBitField, uData, eBus_NoRead);
}

/*============================================================================*/
/* FUNCTION:    Volcano_Read                                                  */
/*                                                                            */
/* DESCRIPTION: Reads in Volcano hardware                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const Volcano_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData     /* I: Data to read */
)
{
    return iVolcano_Read(&gVolcanoInstance[tUnit], pBitField, puData, eBus_NoWrite);
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    iVolcano_AllocInstance:                                       */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iVolcano_AllocInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    ppVolcanoObject_t  ppDrvObject /* I: Driver Object */
)
{
    tmErrorCode_t       err = !TM_OK;
    pVolcanoObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<Volcano_UNITS; uLoopCounter++)
    {
        pObj = &gVolcanoInstance[uLoopCounter];
        if (pObj->tUnit == -1)
        {
            err = TM_OK;

            pObj->tUnit = tUnit;

            if(err == TM_OK)
            {
                *ppDrvObject = pObj;
            }
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iVolcano_GetInstance:                                         */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iVolcano_GetInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    ppVolcanoObject_t  ppDrvObject /* I: Driver Object */
)
{
    tmErrorCode_t       err = !TM_OK;
    pVolcanoObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Get instance */
    for (uLoopCounter = 0; uLoopCounter<Volcano_UNITS; uLoopCounter++)
    {
        pObj = &gVolcanoInstance[uLoopCounter];
        if (pObj->tUnit == tUnit)
        {
            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iVolcano_CalculateNIntKInt                                    */
/*                                                                            */
/* DESCRIPTION: Calculate PLL N & K values                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iVolcano_CalculateNIntKInt(
    pVolcanoObject_t   pObj,    /* I: Driver object */
    UInt32  uFVCO,
    UInt32* NInt, 
    UInt32* KInt
)
{
    tmErrorCode_t err = TM_OK;

    /* Algorithm that calculates N_K */
    UInt32 N_K_prog = 0;

    /* Algorithm that calculates N, K corrected */
    UInt32 Nprime = 0;
    UInt32 KforceK0_1 = 0;
    UInt32 K2msb = 0;
    UInt32 N0 = 0;
    UInt32 Nm1 = 0;

    /* Calculate N_K_Prog */
    N_K_prog = (uFVCO * 128) / 125;

    /* Calculate N & K corrected values */
    Nprime = N_K_prog & 0xFF0000;

    /* Force LSB to 1 */
    KforceK0_1 = 2*(((N_K_prog - Nprime) << 7) / 2) + 1;

    /* Check MSB bit around 2 */
    K2msb = KforceK0_1 >> 21;
    if (K2msb < 1)
    {
        N0 = 1;
    }
    else
    {
        if (K2msb >= 3)
        {
            N0 = 1;
        }
        else
        {
            N0 = 0;
        }
    }
    if (K2msb < 1)
    {
        Nm1 = 1;
    }
    else
    {
        Nm1 = 0;
    }

    /* Calculate N */
    *NInt = (2 * ((Nprime >> 16) - Nm1) + N0) - 128;

    /* Calculate K */
    if (K2msb < 1)
    {
        *KInt = KforceK0_1 + (2 << 21);
    }
    else
    {
        if (K2msb >= 3)
        {
            *KInt = KforceK0_1 - (2 << 21);
        }
        else
        {
            *KInt = KforceK0_1;
        }
    }

    /* K (23bits ) to LO_Frac =  [Frac_2 :7bits][Frac_1 :8bits][ 8 bits left] */

    /* Force last 5 bits of Frac_1 to 0x11 for spurs optimization */
    *KInt &= 0xFFFFE000;
    *KInt |= 0x1100;

    return err;
}


/*============================================================================*/
/* FUNCTION:    iVolcano_Write                                                */
/*                                                                            */
/* DESCRIPTION: Writes in Volcano hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iVolcano_Write(
    pVolcanoObject_t           pObj,       /* I: Driver object */
    const Volcano_BitField_t*  pBitField, /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    BusAccess_t                eBusAccess  /* I: Access to bus */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8           RegAddr = 0;
    UInt8           RegData = 0;
    pUInt8          pRegData = Null;
    UInt32          RegMask = 0;
    UInt8           OpCommand[2] = {0,0};

    if(pBitField == Null)
    {
        err = !TM_OK;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < Volcano_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&eBus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from Volcano  :  read cmd 0x01 + adr register to read */
		    OpCommand[0] = (0x1 << 6) + RegAddr;
            err = P_SIO_WRITEREAD(pObj->tUnit, 8, &OpCommand[0],8, pRegData);

        }

        if(err == TM_OK)
        {
            RegMask = ( (1 << pBitField->WidthInBits) - 1);
            /* Limit uData to WidthInBits */
            uData &= RegMask;

            /* Set Data */
            RegMask = RegMask << pBitField->PositionInBits;
            *pRegData &= (UInt8)(~RegMask);
            *pRegData |= uData << pBitField->PositionInBits;

            if( (eBusAccess&eBus_NoWrite) == False && P_SIO_WRITE_VALID)
            {
                /* Write data to Volcano*/
		        OpCommand[0] = (0x0 << 6) + RegAddr;
		        OpCommand[1] = *pRegData;
                err = P_SIO_WRITE(pObj->tUnit, 16, &OpCommand[0]);
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iVolcano_Read                                                 */
/*                                                                            */
/* DESCRIPTION: Reads in Volcanohardware                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iVolcano_Read(
    pVolcanoObject_t           pObj,       /* I: Driver object */
    const Volcano_BitField_t*  pBitField, /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    BusAccess_t                eBusAccess  /* I: Access to bus */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8           RegAddr = 0;
    UInt8           RegData = 0;
    pUInt8          pRegData = Null;
    UInt32          RegMask = 0;
    UInt8           OpCommand = 0;

   if(pBitField == Null)
    {
        err = !TM_OK;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < Volcano_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&eBus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from Volcano  :  read cmd 0x01 + adr register to read */
		    OpCommand = (0x1 << 6) + RegAddr;
            err = P_SIO_WRITEREAD(pObj->tUnit, 8, &OpCommand,8, pRegData);
        }

        if(err == TM_OK && puData != Null)
        {
            /* Copy Raw Data */
            *puData = *pRegData;

            /* Get Data */
            RegMask = ( (1 << pBitField->WidthInBits) - 1) << pBitField->PositionInBits;
            *puData &= (UInt8)RegMask;
            *puData = (*puData) >> pBitField->PositionInBits;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iVolcano_Wait                                                 */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iVolcano_Wait(
    pVolcanoObject_t   pObj,   /* I: Driver object */
    UInt32              Time    /* I: time to wait for */
)
{
    tmErrorCode_t   err = !TM_OK;


    if(P_STIME_WAIT_VALID)
    {
        /* Wait Time ms */
        err = P_STIME_WAIT(Time);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    Volcano_ReadRegMap                                            */
/*                                                                            */
/* DESCRIPTION: Reads driver RegMap cached data from Volcano hardware.        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
Volcano_ReadRegMap(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    UInt8               uAddress,   /* I: Data to read */
    UInt32              uReadLen    /* I: Number of data to read */
)
{
    pVolcanoObject_t   pObj = Null;
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;
    UInt8           OpCommand = 0;

    /* get pointer in instance array */
    err = iVolcano_GetInstance(tUnit, &pObj);
    
    if( err == TM_OK && uAddress < Volcano_REG_MAP_NB_BYTES &&
       (uAddress + uReadLen) <= Volcano_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = !TM_OK;
    }

    if(err == TM_OK && P_SIO_READ_VALID)
    {
        /* Read data from Volcano  :  read cmd 0x01 + adr register to read */
	    OpCommand = (0x1 << 6) + uAddress;
        err = P_SIO_WRITEREAD(pObj->tUnit, 8, &OpCommand,uReadLen<<3, pRegData);
    }

    return err;
}
