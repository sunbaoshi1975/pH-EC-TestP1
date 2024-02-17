//  xlxLogger.h - Xlight Logger library

#ifndef xlxLogger_h
#define xlxLogger_h

#include "xliCommon.h"

// Log Destination
enum {
    LOGDEST_SERIAL = 0,
    LOGDEST_FLASH,
    LOGDEST_SYSLOG,
    LOGDEST_CLOUD,
    LOGDEST_DUMMY
};

// Log Level
enum {
    LEVEL_EMERGENCY = 0,
    LEVEL_ALERT,
    LEVEL_CRITICAL,
    LEVEL_ERROR,
    LEVEL_WARNING,
    LEVEL_NOTICE,
    LEVEL_INFO,
    LEVEL_DEBUG,
};

// Log tags: 3 bytes
#define LOGTAG_STATUS         "STA"
#define LOGTAG_EVENT          "EVT"
#define LOGTAG_ACTION         "ACT"
#define LOGTAG_DATA           "DAT"
#define LOGTAG_MSG            "MSG"

/// LOG Message
#define CLT_ID_LOGMSG         3
#define CLT_NAME_LOGMSG       "xlc-event-log"
#define CLT_TTL_LOGMSG        3600              // 1 hour


//------------------------------------------------------------------
// Xlight Logger Class
//------------------------------------------------------------------
class LoggerClass
{
private:
  UC m_level[LOGDEST_DUMMY];
  char m_SysID[MAX_LEN_SYS_ID];

public:
  LoggerClass();
  void Init(const char *sysid);

  BOOL InitFlash(const UL addr, const UL size);
  BOOL InitSysLog(const char *host, const US port);
  BOOL InitCloud(const char *url, const char *uid, const char *key);

  UC GetLevel(const UC logDest);
  void SetLevel(const UC logDest, const UC logLevel);
  void WriteLog(const UC level, const char *tag, const char *msg, ...);
  bool ChangeLogLevel(char *strMsg);
  char *PrintDestInfo(char *_pstrOutput);
};

//------------------------------------------------------------------
// Function & Class Helper
//------------------------------------------------------------------
extern LoggerClass theLog;
#define LOGA(tag, fmt, ...)       theLog.WriteLog(LEVEL_ALERT, tag, fmt, ##__VA_ARGS__)
#define LOGC(tag, fmt, ...)       theLog.WriteLog(LEVEL_CRITICAL, tag, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...)       theLog.WriteLog(LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...)       theLog.WriteLog(LEVEL_WARNING, tag, fmt, ##__VA_ARGS__)
#define LOGN(tag, fmt, ...)       theLog.WriteLog(LEVEL_NOTICE, tag, fmt, ##__VA_ARGS__)
#define LOGI(tag, fmt, ...)       theLog.WriteLog(LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOGD(tag, fmt, ...)       theLog.WriteLog(LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)

#endif /* xlxLogger_h */
