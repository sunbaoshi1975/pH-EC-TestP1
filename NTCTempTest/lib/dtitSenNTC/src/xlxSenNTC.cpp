/**
 * xlxSenNTC.cpp - Xlight NTC Temperature Sensor Class implementation
 *
 * Created by Baoshi Sun <bs.sun@datatellit.com>
 * Copyright (C) 2015-2024 DTIT
 * Full contributor list:
 *
 * Documentation:
 * Support Forum:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Created by Baoshi Sun <bs.sun@datatellit.com>
 *
 * DESCRIPTION
 *
 * ToDo:
 * 1.
**/

#include "xlxSenNTC.h"
#include "math.h"
#include "xliPinMap.h"

#ifndef PIN_ADC_TEMP
#define PIN_ADC_TEMP                A7          // 热敏电阻电压
#endif

#ifndef PIN_VOL_TEMP
#define PIN_VOL_TEMP                D1          // 热敏电阻测量供电
#endif

/**
 * constants from the datasheet: page 73. In this case I use the 10.000 ohm, R25+/-5%: brown, black, orange
*/
#define DTIT_NTC_A 					0.00335401643468
#define DTIT_NTC_B					2.569850E-04
#define DTIT_NTC_C 					2.62013E-06
#define DTIT_NTC_D 					6.38309E-07

#define DTIT_NTC_Bx					(float)3950	// NTC B constant

#define DTIT_NTC_REF_RESISTOR 		10000		// Ref. Resistor Value (10K)
#define DTIT_NTC_SAMPLES 			1 			// Number of loops


//------------------------------------------------------------------
// Xlight xlSenNTCClass
//------------------------------------------------------------------
xlSenNTCClass::xlSenNTCClass() {
	m_nTemp100 = 0;
}

// Init Sensor Input & Output Resource
void xlSenNTCClass::InitPins() {
	//pinMode(PIN_ADC_TEMP, AN_INPUT);
    pinMode(PIN_VOL_TEMP, OUTPUT);
	// Voltage for measure
	digitalWrite(PIN_VOL_TEMP, HIGH);
	delay(10);
}

LONG xlSenNTCClass::GetCelsius() {
	return m_nTemp100;
}

LONG xlSenNTCClass::GetFahrenheit() {
	LONG tempF = (m_nTemp100 * 9 + 16003) / 5;
	return tempF;
}

LONG xlSenNTCClass::ReadTemp(const bool _bCelsius) {
	// Read ADC
	UL adcVal = ADCRead();
	// Calculate Celsius temperature
	termistor(adcVal);
	return(_bCelsius ? m_nTemp100 : GetFahrenheit());
}

UL xlSenNTCClass::ADCRead() {
	UL adcMV = 0;

	for( UC i = 0; i < DTIT_NTC_SAMPLES; i++ ) {
		int16_t rc = analogRead(PIN_ADC_TEMP);
		adcMV += rc;
		SERIAL_LN(F("Debug - ADC:%d"), rc);
	}
	// Calculates average
	adcMV /= DTIT_NTC_SAMPLES;
	adcMV *= 13;	// 3.3v map to 0.25v
	return(min(adcMV, 4095));
}

/**
 * 
 */
LONG xlSenNTCClass::termistor(const UL rawADC) {
	double tempKelvin;

	// Formula to calculate resistance (Rt)
	double Rt = DTIT_NTC_REF_RESISTOR * rawADC / (4095 - rawADC);
	// Conversion formula to calculate temperature(degrees celsius) from resistance
	double temperature = 75.913 * pow(M_E, -1 * pow(10, -4) * Rt);
	SERIAL_LN(F("Debug - ADC:%d Rt:%0.1f, temp1:%0.2f"), rawADC, Rt, temperature);

	/*
	tempKelvin = log(40960000 / adcVal - DTIT_NTC_REF_RESISTOR);
	tempKelvin = 100 / (0.001129148 + (0.000234125 + 0.0000000876741 * tempKelvin * tempKelvin) * tempKelvin);
	*/

	// T2为25摄氏度，折算为开尔文温度
	// float T2 = 298.15; float Ka = 273.15;
	// tempK = 1 / (1/T2 + log(Rt/Rp)/Bx) - Ka + 0.5;

	//double result = log(rawADC / (4095 - rawADC));		// log(Rt/Rp)
	//tempKelvin = 100 / (DTIT_NTC_A + result / DTIT_NTC_Bx);	
	//double tempKelvin1 = 100 / (DTIT_NTC_A + ((DTIT_NTC_B + DTIT_NTC_C * result) + DTIT_NTC_D * result * result) * result);	

	//m_nTemp100 = (LONG)(tempKelvin - 27314.5); // Kelvin to Celsius
	//SERIAL_LN(F("Debug - ADC:%d Rt:%0.1f, Reslut:%0.8f, temp1:%0.2f, temp2:%0.2f, temp3:%0.2f"), rawADC, Rt, result,  m_nTemp100 / 100, (tempKelvin1 - 27314.5) / 100, temperature);
	
	return(m_nTemp100);
}