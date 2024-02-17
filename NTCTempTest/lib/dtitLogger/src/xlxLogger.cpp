/**
 * xlxLogger.cpp - Xlight logger library for application data logging
 *
 * Created by Baoshi Sun <bs.sun@datatellit.com>
 * Copyright (C) 2015-2016 DTIT
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
 * 1. Define basic interfaces
 * 2. Serial logging
 *
 * ToDo:
 * 1. syslog, refer to psyslog.cpp
 * 2. http/cloud
 * 3. Flash
 * 4. Offline Data Cache in Flash (loop overwrite)
**/

#include "xlxLogger.h"

// the one and only instance of LoggerClass
LoggerClass theLog = LoggerClass();
char strDestNames[][7] = {"serial", "flash", "syslog", "cloud", "all"};
char strLevelNames[][9] = {"none", "alert", "critical", "error", "warn", "notice", "info", "debug"};

//------------------------------------------------------------------
// Xlight Logger Class
//------------------------------------------------------------------
LoggerClass::LoggerClass()
{
    m_level[LOGDEST_SERIAL] = LEVEL_DEBUG;
    m_level[LOGDEST_FLASH] = LEVEL_WARNING;
    m_level[LOGDEST_SYSLOG] = LEVEL_INFO;
    m_level[LOGDEST_CLOUD] = LEVEL_NOTICE;
    memset(m_SysID, 0x00, sizeof(m_SysID));
}

void LoggerClass::Init(const char *sysid)
{
    strncpy(m_SysID, sysid, MAX_LEN_SYS_ID - 1);
}

BOOL LoggerClass::InitFlash(const UL addr, const UL size)
{
    //ToDo:
    return true;
}

BOOL LoggerClass::InitSysLog(const char *host, const US port)
{
    //ToDo:
    return true;
}

BOOL LoggerClass::InitCloud(const char *url, const char *uid, const char *key)
{
    //ToDo:
    return true;
}

UC LoggerClass::GetLevel(const UC logDest)
{
    if( logDest < LOGDEST_DUMMY)
        return m_level[logDest];

    return LEVEL_EMERGENCY;
}

void LoggerClass::SetLevel(const UC logDest, const UC logLevel)
{
    if( logDest < LOGDEST_DUMMY)
    {
        if( m_level[logDest] != logLevel )
            m_level[logDest] = logLevel;
    }
}

void LoggerClass::WriteLog(const UC level, const char *tag, const char *msg, ...)
{
    int nSize;
    char buf[MAX_MESSAGE_LEN];

    // Prepare message
    memset(buf, 0x00, MAX_MESSAGE_LEN);
    int nPos = snprintf(buf, MAX_MESSAGE_LEN, "%02d:%02d:%02d %d %s ",
        Time.hour(), Time.minute(), Time.second(), level, tag);
    va_list args;
    va_start(args, msg);
    nSize = vsnprintf(buf + nPos, MAX_MESSAGE_LEN - nPos, msg, args);
    nPos += nSize;

    // Send message to serial port
    if( level <= m_level[LOGDEST_SERIAL] ) {
        TheSerial.println(buf);
    }

    // Output Log to Particle cloud variable
    if( level <= m_level[LOGDEST_CLOUD] ) {
        if(Particle.connected()) Particle.publish(CLT_NAME_LOGMSG, buf, CLT_TTL_LOGMSG, PRIVATE);
    }

    // ToDo: send log to other destinations
    //if( level <= m_level[LOGDEST_SYSLOG] ) {
    //;}
    //if( level <= m_level[LOGDEST_FLASH] ) {
    //}
}

bool LoggerClass::ChangeLogLevel(char *strMsg)
{
    if( !strMsg ) return false;

    UC lv_Dest, lv_Level;
    char *lv_sDest;
    char *lv_sLevel;
    lv_sDest = strtok_r(strMsg, ":", &lv_sLevel);
    if( !lv_sDest ) {
        // all
        lv_sDest = strDestNames[LOGDEST_DUMMY];
        lv_sLevel = strMsg;
    } else if( !lv_sLevel ) {
        lv_sLevel = strLevelNames[LEVEL_EMERGENCY];
    }

    // Parse Log Level
    for( lv_Level = LEVEL_EMERGENCY; lv_Level <= LEVEL_DEBUG; lv_Level++ ) {
        if( wal_stricmp(lv_sLevel, strLevelNames[lv_Level]) == 0 ) break;
    }
    if( lv_Level > LEVEL_DEBUG ) return false;

    // Parse Log Destination
    for( lv_Dest = LOGDEST_SERIAL; lv_Dest <= LOGDEST_DUMMY; lv_Dest++ ) {
        if( wal_stricmp(lv_sDest, strDestNames[lv_Dest]) == 0 ) break;
    }
    if( lv_Dest > LOGDEST_DUMMY ) return false;

    if( lv_Dest  == LOGDEST_DUMMY ) {
        for( lv_Dest = LOGDEST_SERIAL; lv_Dest < LOGDEST_DUMMY; lv_Dest++ ) {
            SetLevel(lv_Dest, lv_Level);
        }
    } else {
        SetLevel(lv_Dest, lv_Level);
    }

    return true;
}

char *LoggerClass::PrintDestInfo(char *_pstrOutput)
{
    int nPos = 0, nCx;
    UC lv_Dest;

    for( lv_Dest = LOGDEST_SERIAL; lv_Dest < LOGDEST_DUMMY; lv_Dest++ ) {
        SERIAL_LN("LOG: %s - level: %s", strDestNames[lv_Dest], strLevelNames[GetLevel(lv_Dest)]);
        if( lv_Dest != LOGDEST_SERIAL ) {
            if( _pstrOutput && nPos < MAX_MESSAGE_LEN ) {
                nCx = snprintf(_pstrOutput + nPos, MAX_MESSAGE_LEN - nPos, "; ");
                nPos = (nCx < 0 ? MAX_MESSAGE_LEN : nPos + nCx);
            }
        }

        if( _pstrOutput && nPos < MAX_MESSAGE_LEN ) {
            nCx = snprintf(_pstrOutput + nPos, MAX_MESSAGE_LEN - nPos, "%s@%s",
                    strLevelNames[GetLevel(lv_Dest)], strDestNames[lv_Dest]);
            nPos = (nCx < 0 ? MAX_MESSAGE_LEN : nPos + nCx);
        }
    }
    SERIAL_LN("");

    return _pstrOutput;
}
