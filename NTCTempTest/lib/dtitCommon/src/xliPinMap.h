//  xliPinMap.h - VegeSense include file: SmartPlant Box MCU Pin Map
#ifndef xliPinMap_h
#define xliPinMap_h

//------------------------------------------------------------------
// MCU Pin Usage
//------------------------------------------------------------------

//------------------------------------------------------------------
// GPIOs
/* ADC�ɼ� */
#define PIN_ADC_EC1                 A0          // EC�缫1��ѹ
#define PIN_ADC_EC2                 A1          // EC�缫2��ѹ
#define PIN_ADC_PH                  A6          // pH��ѹ
#define PIN_ADC_TEMP                A4          // ���������ѹ

/* ������� */
#define PIN_VOL_EC                  D0          // EC��������
#define PIN_VOL_TEMP                D1          // ���������������

//------------------------------------------------------------------
// Soft Serial Port
#define PIN_SS1_TXD	 	            D3
#define PIN_SS1_RXD	 	            D4

//------------------------------------------------------------------
// Available PINs
//#define PIN_D2_GPIO_IN	 	        D2

#endif /* xliPinMap_h */
