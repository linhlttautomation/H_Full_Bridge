/*
 * AVC_Variables.h
 *
 *  Created on: 5 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _PV_VARIABLES_H_
#define _PV_VARIABLES_H_

#include <var.h>
//----------------------------------------------------------------------------------------
// CLA ---> CPU
typedef struct
{
    float Udc;

	float Va_G;
	float Vb_G;
	float Vc_G;

	float Ia_inv;
    float Ib_inv;
    float Ic_inv;
    float In_inv;

	float Temp;
	float Theta;
	float datalog1;
	float datalog2;
	float datalog3;
}ADC_VALUE;


typedef struct
{
    float VaGRms;
    float VbGRms;
    float VcGRms;

    float UsRmsPLL;
    float VaGRmsPLL;
    float VbGRmsPLL;
    float VcGRmsPLL;

    float VabGRms;
    float VcbGRms;
    float VabLRms;
    float VcbLRms;

    float IaInvRms;
    float IbInvRms;
    float IcInvRms;

    float VaLRms;
    float VbLRms;
    float VcLRms;

    float IaLRms;
    float IbLRms;
    float IcLRms;

    float Usd;
    float Usq;
    float Usz;

    float Udc_ref;

    float Isd_ref;
    float Isq_ref;
    float Isz_ref;

    float Usd_pll;
    float Usq_pll;

    float Isd_fb;
    float Isq_fb;
    float Isz_fb;

    float Ua;
    float Ub;
    float Uc;
    Uint16 Prism;
	float da;
	float db;
	float dc;
	float dn;
	float Theta1;
    float Freq;
	int32 Currflag;
    int32 Voltflag;
    short SwitchRamp;
}MEASUREMENT_VAULE;

typedef struct
{
	ADC_VALUE ADC_CPU;
	MEASUREMENT_VAULE MEASUARE_CPU;

}CLA_TO_CPU;

//----------------------------------------------------------------------------------------

typedef struct
{
	float UdcTesting;

	float VdTesting;
	float VqTesting;
	float VzTesting;

	float IdRef;
	float IqRef;
	float IzRef;

	float UdRef;
	float UqRef;
	float UzRef;

	float UdcRef;

	float Fref;
	short EnableFlag;
	short EnableADC;
    short ControlMode;

    float ADCoffset_Udc;
    float ADCoffset_VaG;
    float ADCoffset_VbG;
    float ADCoffset_VcG;
    float ADCoffset_Ia_inv;
    float ADCoffset_Ib_inv;
    float ADCoffset_Ic_inv;

    float ADCgain_Udc;
    float ADCgain_VaG;
    float ADCgain_VbG;
    float ADCgain_VcG;
    float ADCgain_VL;
    float ADCgain_Ia_inv;
    float ADCgain_Ib_inv;
    float ADCgain_Ic_inv;


}CPU_TO_CLA;

#endif /* _PV_VARIABLES_H_ */
