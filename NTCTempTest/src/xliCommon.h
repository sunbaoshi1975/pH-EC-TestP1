//  xlCommon.h - Xlight common definitions header

#ifndef xliCommon_h
#define xliCommon_h

#include "application.h"
#include "xliBitOperation.h"

// Here, common data types have been given alternative names through #define statements
// Common Data Type
#define BOOL                      boolean
#define UC                        uint8_t
#define US                        uint16_t
#define UL                        uint32_t
#define CHAR                      int8_t
#define SHORT                     int16_t
#define LONG                      int32_t

// Device Status
#define STATUS_OFF                0x00        // Power Off
#define STATUS_INIT               0x01        // Initializing
#define STATUS_BMW                0x02        // Basic Mode of Working
#define STATUS_DIS                0x03        // WAN Disconnected
#define STATUS_NWS                0x04        // Normal Working Status
#define STATUS_SLP                0x05        // Sleep
#define STATUS_RST                0x06        // Reseting
#define STATUS_ERR                0x07        // Fatal Error

// Cloud Dependency
#define CLOUD_DISABLE             0x00        // Disable Cloud
#define CLOUD_ENABLE              0x01        // Default, Enable Cloud, use it if available
#define CLOUD_MUST_CONNECT        0x02        // Enable Cloud and get connected anyhow

// Serial Port Speed
#define SERIALPORT_SPEED_LOW      9600
#define SERIALPORT_SPEED_14400    14400
#define SERIALPORT_SPEED_MID      19200
#define SERIALPORT_SPEED_57600    57600
#define SERIALPORT_SPEED_HIGH     115200
#define SERIALPORT_SPEED_DEFAULT  SERIALPORT_SPEED_HIGH

// Maximum length of SysID
#define MAX_LEN_SYS_ID              32
#define MAX_LEN_SYS_VERSION         8
#define MAX_LEN_APP_VERSION         16

#define MAX_MESSAGE_SHORT           128
#define MAX_MESSAGE_LEN             256

// Sensor Read Speed (in seconds)
#define SEN_DHT_SPEED_LOW         30
#define SEN_DHT_SPEED_NORMAL      5
#define SEN_DHT_SPEED_HIGH        2

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On, can have an optional level parameter 
#define DEVICE_SW_TOGGLE            2       // Toggle
#define DEVICE_SW_DUMMY             3       // Don't change, detail followed

// Specify system serial port, could be Serial, USBSerial1, Serial1 or Seria2
#ifndef TheSerial
#define TheSerial       Serial
#endif

#define SERIAL          TheSerial.printf
#define SERIAL_LN       TheSerial.printlnf

//--------------------------------------------------
// Tools & Helpers
//--------------------------------------------------
#ifndef ABS_RETURN
#define ABS_RETURN(x,y)         	((x < y) ? (y-x) : (x-y))
#endif

#ifndef MIN_RETURN
#define MIN_RETURN(x,y)         	((x < y) ? (x) : (y))
#endif

#ifndef MAX_RETURN
#define MAX_RETURN(x,y)         	((x < y) ? (y) : (x))
#endif

uint8_t bitOneCount8(const uint8_t _byte);
int wal_stricmp(const char *a, const char *b);
int wal_strnicmp(const char *a, const char *b, uint8_t len);
uint8_t h2i(const char c);
bool strAllDigits(const char *a);
char* PrintUint64(char *buf, uint64_t value, bool bHex = true);
char* PrintMacAddress(char *buf, const uint8_t *mac, char delim = ':', bool bShort = true);
uint64_t StringToUInt64(const char *strData,bool bforceHex=false);
inline time_t tmConvert_t(US YYYY, UC MM, UC DD, UC hh, UC mm, UC ss)  // inlined for speed
{
    struct tm t;
    t.tm_year = YYYY-1900;
    t.tm_mon = MM - 1;
    t.tm_mday = DD;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;
    t.tm_isdst = 0;  // not used
    time_t t_of_day = mktime(&t);
    return t_of_day;
};

US usCRC16(const UC * pucFrame, US usLen);

#endif /* xliCommon_h */