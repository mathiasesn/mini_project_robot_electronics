/**
*
* @file main.c
*
* This file contains a design example using the driver functions
* of the XADC driver. The example here shows the driver/device in polled mode
* to check the voltage on ADC 14. This example is highly inspired by Xilinx
* standard example on how to use the XADC module.
*
* @note
*
*
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who   Date      Changes
* ----- ----  --------  -----------------------------------------------------
* 1.00a jcla  31/03/16  First release based on the Xilinx XADC example driver
*
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xparameters.h"
#include "bram.h"
#include "xadcps.h"
#include "xstatus.h"
#include <unistd.h>


#include "core_portme.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID

/**************************** Marco Definitions ******************************/


/************************** Function Prototypes *****************************/

static int XAdcGetVoltageFromRegisters(u16 XAdcDeviceId, int *voltage, int register_offset);
static int XAdcFractionToInt(float FloatNum);
static int float2Int(float f);


/***************** Macros (Inline Functions) Definitions ********************/

#define printf xil_printf /* Small foot-print printf function */

// converting RAW data from external sourse to voltage
#define XAdcPs_ExternalRawToVoltage(AdcData)\
	((((float)(AdcData))* (1.0f))/65536.0f)

#define V_BAT_EQ 3.9375
#define I_CHARGE_EQ 15.75
#define V_BAT_0 1.1

/************************** Variable Definitions ****************************/

static XAdcPs XAdcInst;      /* XADC driver instance */

/****************************************************************************/


int main(void)
{
	init_platform();

	//BRAM
	initMemory();

	//XADC
	int voltage_reading_A0, voltage_reading_A1 = 0; // Result is in mV going from 0V = 0 to 1000 = 3.3V
	int pwm = 0b00;

	print("Hello XADC!\n");

	int Status;

	// Get user input
	float v_nominal;
	xil_printf("Enter the nominal battery voltage [V]:\r\n");
	scanf("%f", &v_nominal);
	xil_printf("V_nominal = %d.%d [V]\r\n",(int)v_nominal, XAdcFractionToInt(v_nominal));

	int capacity;
	xil_printf("Enter the battery capacity [mAh]:\r\n");
	scanf("%d", &capacity);
	xil_printf("Capacity = %d [mAh]:\r\n", capacity);

	float k;
	xil_printf("Enter the charge speed( k = [0.1, 0.5] ):\r\n");
	scanf("%f", &k);
	xil_printf("k = %d.%d \r\n",(int)k, XAdcFractionToInt(k));

	float c;
	xil_printf("Enter the rate of charge (C):\r\n");
	scanf("%f", &c);
	xil_printf("Rate of charge = %d.%d\r\n",(int)c, XAdcFractionToInt(c));

	Status = XAdcGetVoltageFromRegisters(XADC_DEVICE_ID, &voltage_reading_A0, 1); // read from A0
	XAdcGetVoltageFromRegisters(XADC_DEVICE_ID, &voltage_reading_A1, 9); // read from A1
	if (Status != XST_SUCCESS) {
		printf("Failed reading from ADC");
		return XST_FAILURE;
	}

	// Converts to V
	float v_adc_1 = (voltage_reading_A0*3.3)/1000.0;
	float v_adc_2 = (voltage_reading_A1*3.3)/1000.0;

	float i_charge = I_CHARGE_EQ * (v_adc_1 - v_adc_2) * 1000;
	float v_bat = V_BAT_EQ * v_adc_2;
	float i_target = capacity * c * k;
	float t_left = capacity / i_charge;

	xil_printf("Target current = %d.%d [mA]", (int)i_target, XAdcFractionToInt(i_target));

	int led, v_bat_lvl;

	while (v_bat < v_nominal)
	{
		XAdcGetVoltageFromRegisters(XADC_DEVICE_ID, &voltage_reading_A0, 1); // read from A0
		XAdcGetVoltageFromRegisters(XADC_DEVICE_ID, &voltage_reading_A1, 9); // read from A1

		v_adc_1 = (voltage_reading_A0 * 3.3) / 1000.0;
		v_adc_2 = (voltage_reading_A1 * 3.3) / 1000.0;

		i_charge = I_CHARGE_EQ * (v_adc_1 - v_adc_2) * 1000.0;
		t_left = capacity / i_charge;

		pwm = 0b00;
		if (i_charge > i_target)
		{
			pwm = 0b01; //decrease
		}

		if (i_charge < i_target)
		{
			pwm = 0b10; //increase
		}

		v_bat = V_BAT_EQ * v_adc_2;

		v_bat_lvl = (v_bat - V_BAT_0) *100 / (v_nominal - V_BAT_0);
		if (v_bat_lvl > 20)
			led = 0b1;
		else
			led = 0b0;

		MYMEM_u(0)=CombineForBram(led, pwm); // using memory addr 0

		xil_printf("Vbat: %d.%d, Time_left: %d.%d, itarget: %d.%d, icharge: %d.%d\n\r", (int)v_bat, float2Int(v_bat),(int)t_left, float2Int(t_left), (int)i_target, float2Int(i_target), (int)i_charge, float2Int(i_charge));

		/*xil_printf("PWM --> %d\n\r", CombineForBram(led, pwm));
		xil_printf("VADC1 --> %d.%d\n\r", (int)v_adc_1, float2Int(v_adc_1));
		xil_printf("VADC2 --> %d.%d\n\r", (int)v_adc_2, float2Int(v_adc_2));
		xil_printf("itarget --> %d.%d\n\r", (int)i_target, float2Int(i_target));
		xil_printf("icharge --> %d.%d\n\r", (int)i_charge, float2Int(i_charge));*/
		//Writes to BRAM
		//MYMEM_u(0)=2; BRAM address 0
		//MYMEM_u(1)=1; BRAM address 0x4

	}
	xil_printf("Done charging");
	cleanup_platform();
	return XST_SUCCESS;
}

/****************************************************************************/
/**								Reading voltage
****************************************************************************/
int XAdcGetVoltageFromRegisters(u16 XAdcDeviceId, int *voltage, int register_offset)
{
	int Status;
	XAdcPs_Config *ConfigPtr;
	u32 VccPdroRawData;
	float VccPintData;
	XAdcPs *XAdcInstPtr = &XAdcInst;

	/*
	 * Initialize the XAdc driver.
	 */
	ConfigPtr = XAdcPs_LookupConfig(XAdcDeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

	/*
	 * Self Test the XADC/ADC device
	 */
	Status = XAdcPs_SelfTest(XAdcInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Read the A1 input Voltage Data from the
	 * ADC data registers.
	 */
	VccPdroRawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_CH_AUX_MIN+register_offset);
	VccPintData = XAdcPs_ExternalRawToVoltage(VccPdroRawData);


	(*voltage) = XAdcFractionToInt(VccPintData);

	return XST_SUCCESS;
}

/****************************************************************************/
/*
*
* This function converts the fraction part of the given floating point number
* (after the decimal point)to an integer.
*
* @param	FloatNum is the floating point number.
*
* @return	Integer number to a precision of 3 digits.
*
* @note
* This function is used in the printing of floating point data to a STDIO device
* using the xil_printf function. The xil_printf is a very small foot-print
* printf function and does not support the printing of floating point numbers.
*
*****************************************************************************/
int float2Int(float f)
{
	return XAdcFractionToInt(f);
}
int XAdcFractionToInt(float FloatNum)
{
	float Temp;

	Temp = FloatNum;
	if (FloatNum < 0)
	{
		Temp = -(FloatNum);
	}

	return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}

/****************************************************************************/
/**				     	Combining data for bram
****************************************************************************/
int CombineForBram(int battery_thresh, int pwm)
{
	int result_pwm = 0b011;
	int result_bat = 0b001;
	int result;
	result_pwm = result_pwm & pwm;

	result_bat = result_bat & battery_thresh;
	result_bat = result_bat << 2;

	result = result_pwm | result_bat;

	return result;
}

