// xlxSenNTC.h - Xlight NTC Temperature Sensor

#ifndef __XLX_SEN_NTC_CLASS_H__
#define __XLX_SEN_NTC_CLASS_H__

#include "xliCommon.h"

//------------------------------------------------------------------
// Xlight xlRelayClass for action item
//------------------------------------------------------------------
class xlSenNTCClass
{
public:
    // Public functions
    xlSenNTCClass();
    void InitPins();

    LONG ReadTemp(const bool _bCelsius = true);
    LONG GetCelsius();
    LONG GetFahrenheit();

protected:
    UL ADCRead();
    LONG termistor(const UL rawADC);

private:
    // 100 times of real temperature value
    LONG m_nTemp100;
};

#endif /* __XLX_SEN_NTC_CLASS_H__ */
