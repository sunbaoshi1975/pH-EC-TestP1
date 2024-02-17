//  xliPinMap.h - VegeSense include file: SmartPlant Box MCU Pin Map
#ifndef xliPinMap_h
#define xliPinMap_h

//------------------------------------------------------------------
// MCU Pin Usage
//------------------------------------------------------------------

//------------------------------------------------------------------
// GPIOs
/* ADC采集 */
#define PIN_ADC_EC1                 A0          // EC电极1电压
#define PIN_ADC_EC2                 A1          // EC电极2电压
#define PIN_ADC_PH                  A6          // pH电压
#define PIN_ADC_TEMP                A4          // 热敏电阻电压

/* 数字输出 */
#define PIN_VOL_EC                  D0          // EC测量供电
#define PIN_VOL_TEMP                D1          // 热敏电阻测量供电

//------------------------------------------------------------------
// Soft Serial Port
#define PIN_SS1_TXD	 	            D3
#define PIN_SS1_RXD	 	            D4

//------------------------------------------------------------------
// Available PINs
//#define PIN_D2_GPIO_IN	 	        D2

#endif /* xliPinMap_h */
