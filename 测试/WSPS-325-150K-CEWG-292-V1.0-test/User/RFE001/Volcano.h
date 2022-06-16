/*
  Copyright (C) 2013 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          Volcano.h
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
/* Types and defines:                                                         */
/*============================================================================*/
#define TM_OK               0         /* Global success return status */

typedef struct _IoFunc_t
{		tmErrorCode_t   (*Read)(tmUnitSelect_t tUnit, UInt32 ReadBitsLen, UInt8* pData);
		/** SPI Write hardware register */
		tmErrorCode_t   (*Write)(tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pData);
		/** SPI Write followed by Read hardware register */
		tmErrorCode_t   (*WriteRead)(tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pDataWrite, UInt32 ReadBitsLen, UInt8* pDataRead);
} IoFunc_t, *pIoFunc_t;

typedef struct _TimeFunc_t
{
     tmErrorCode_t   (*Wait)(UInt32 tms);
} TimeFunc_t, *pTimeFunc_t;

typedef struct _SysDependency_t
{
    /** Hardware access to FrontEnd */
    IoFunc_t       sIo;
    /** Time services (wait, getTime, ...) */
    TimeFunc_t     sTime;
} SysDependency_t, *pSysDependency_t;


/* Register Bit-Field Definition */
typedef struct _Volcano_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
Volcano_BitField_t, *pVolcano_BitField_t;

typedef enum _Volcano_PowerStateMode_Item_t {
    Volcano_standby = 0,            /* standby  */
    Volcano_generator_no_daisy = 1, /*  Generator no daisy   */
    Volcano_generator_daisy = 2,    /*  Generator daisy   */
    Volcano_slave_daisy  = 3,	    /* Slave daisy  */
    Volcano_slave_no_daisy = 4,      /* Slave no daisy  */
    Volcano_generator_no_daisy_PPA = 5, /*  Generator no daisy   */
    Volcano_generator_daisy_PPA = 6,    /*  Generator daisy   */
    Volcano_slave_daisy_PPA  = 7,	    /* Slave daisy  */
    Volcano_slave_no_daisy_PPA = 8      /* Slave no daisy  */
} Volcano_PowerStateMode_t, *pVolcano_PowerStateMode_t;

typedef enum _Volcano_ModeMS_Item_t {
    Volcano_Master = 0,
    Volcano_Slave= 1
} Volcano__ModeMS_t, *pVolcano__ModeMS_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
Volcano_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmUnitSelect_t              tUnitD,      /* I: "Volcano driver" device  number */
    SysDependency_t*  psSrvFunc   /*  I: setup parameters */
);

tmErrorCode_t
Volcano_HwInit(
    tmUnitSelect_t tUnit,    /* I: Unit number */
    Volcano__ModeMS_t  /* I:  IC used in Mater or Slave */
);

tmErrorCode_t
Volcano_SetRF(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     UInt32          uLO,     /* I: LO frequency in hertz */
     Bool            bBlanking /* PA in Power down during SetRF */
);

tmErrorCode_t
Volcano_ResetSynchro(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
);

tmErrorCode_t
Volcano_SetSynchroMaster(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
);

tmErrorCode_t
Volcano_SetSynchroSlave(
     tmUnitSelect_t              tUnit      /* I: FrontEnd unit number */
);

tmErrorCode_t
Volcano_SetPowerStateMode(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    Volcano_PowerStateMode_t PowerStateMode /* I: power state mode */
    );

tmErrorCode_t
Volcano_SetRFPhase(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     UInt16    Phase               /* I: Phase in degres */
    );

tmErrorCode_t
Volcano_SetRFAtten(
     tmUnitSelect_t tUnit,      /* I: FrontEnd unit number */
     UInt16    Atten            /* I: RF atten in 1/256 dB steps */
     );

tmErrorCode_t
Volcano_GetPowerStateMode(
     tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
     Volcano_PowerStateMode_t*  pPowerStateMode  /* O: power state mode */
    );

tmErrorCode_t
Volcano_GetAtrfPhase (
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    UInt32*  pATRF_Phase  /* O: atrf_phase */
                  );

tmErrorCode_t
Volcano_GetThermo (
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    UInt8*          pThermo     /* O: temperature in °C */
                  );

tmErrorCode_t
Volcano_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
);

tmErrorCode_t
Volcano_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const Volcano_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData      /* I: Data to write */
);

tmErrorCode_t
Volcano_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const Volcano_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData     /* I: Data to read */
);
tmErrorCode_t
Volcano_ReadRegMap(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    UInt8               uAddress,   /* I: Data to read */
    UInt32              uReadLen    /* I: Number of data to read */
);

#ifdef __cplusplus
}
#endif



