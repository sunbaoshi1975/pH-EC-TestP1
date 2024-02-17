//  xliCommon.cpp - Xlight global variables & functions
#include "xliCommon.h"

//--------------------------------------------------
// Tools & Helpers
//--------------------------------------------------
// Count how many 1(s) in a byte
uint8_t bitOneCount8(const uint8_t _byte) {
    uint8_t lv_cnt = 0;
    uint8_t lv_tmp = _byte;
    while( lv_tmp > 0) {
        if(lv_tmp & 0x01) lv_cnt++;
        lv_tmp >>= 1;
    }
    return lv_cnt;
}

int wal_stricmp(const char *a, const char *b) {
  uint8_t ca, cb;
  do {
     ca = (unsigned char) *a++;
     cb = (unsigned char) *b++;
     ca = toupper(ca);
     cb = toupper(cb);
   } while (ca == cb && ca != '\0');
   return (int)(ca - cb);
}

int wal_strnicmp(const char *a, const char *b, uint8_t len) {
  uint8_t ca, cb,	i = 0;
  do {
     ca = (unsigned char) *a++;
     cb = (unsigned char) *b++;
     ca = toupper(ca);
     cb = toupper(cb);
   } while (ca == cb && ca != '\0' && ++i < len );
   return (int)(ca - cb);
}

bool strAllDigits(const char *a)
{
    uint8_t ca = (unsigned char) *a++;
    for(;ca != '\0';ca = (unsigned char) *a++) {
        if( ca < '0' || ca > '9' ) return false;
    }
    return true;
}

uint8_t h2i(const char c)
{
    UC i;
	if (c <= '9') i = c - '0';
	else if (c >= 'a') i = c - 'a' + 10;
	else i = c - 'A' + 10;
	return i;
}

char* PrintUint64(char *buf, uint64_t value, bool bHex) {
	if (buf != NULL) {
		if (value > 0xFFFFFFFFLL) {
			if (bHex) {
				uint32_t part1 = value >> 32;
				uint32_t part2 = value & 0xFFFFFFFFLL;
				sprintf(buf, "0x%08X%08X", part1, part2);
			}
			else {
				uint8_t digits[20];
				uint64_t tmpV = value;
				int len = 0;
				while (tmpV > 0x00LL) {
					digits[len++] = tmpV % 10;
					tmpV /= 10;
				}
				for (int i = len - 1; i >= 0; i--) {
					buf[len - 1 - i] = digits[i] + '0';
				}
				buf[len] = NULL;
			}
		}
		else {
			if (bHex) {
				sprintf(buf, "0x%04X", value);
			}
			else {
				sprintf(buf, "%d", value);
			}
		}
	}
	return buf;
}

char* PrintMacAddress(char *buf, const uint8_t *mac, char delim, bool bShort)
{
	if (buf != NULL) {
		if( bShort ) {
			// 6 sections, 18 bytes
			sprintf(buf, "%02X%c%02X%c%02X%c%02X%c%02X%c%02X",
					mac[0], delim, mac[1], delim, mac[2], delim, mac[3], delim, mac[4], delim, mac[5]);
		} else {
			// 8 sections, 24 bytes
			sprintf(buf, "%02X%c%02X%c%02X%c%02X%c%02X%c%02X%c%02X%c%02X",
					mac[0], delim, mac[1], delim, mac[2], delim, mac[3], delim, mac[4], delim, mac[5],
					delim, mac[6], delim, mac[7]);
		}
  }

  return buf;
}

uint64_t StringToUInt64(const char *strData,bool bforceHex)
{
  // Hex or decimal
  int i, nLen;
  bool bHex = false;
  nLen = strlen(strData);
  for( i=0; i< nLen; i++ ) {
    if( (strData[i] >= 'a' && strData[i] <= 'f') ||
        (strData[i] >= 'A' && strData[i] <= 'F') ) {
          bHex = true;
    } else if( strData[i] >= '0' && strData[i] <= '9' ) {
      continue;
    } else {
      // Invalid string
      return 0;
    }
  }
  if(bforceHex) bHex = true;
  if( bHex && nLen > 16 ) return 0;

  // Convert charater by Charater
  uint64_t retValue = 0;
  for( i=0; i< nLen; i++ ) {
    if( bHex ) {
      retValue <<= 4;
      retValue += h2i(strData[i]);
    } else {
      retValue *= 10;
      retValue += h2i(strData[i]);
    }
  }

  return retValue;
}

// CRC Calc with CRC Lookup Table. Save CPU Cicles.
#define CRC_LTABLE_CALC
#if defined CRC_LTABLE_CALC
static const uint16_t wCRCTable[] PROGMEM = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

US usCRC16(const UC * pucFrame, US usLen) {
    US crc = 0xFFFF;
    UC tmp;

    while (usLen--) {
        tmp = (*pucFrame++ ^ crc) & 0xFF;
        crc = (crc >> 8) ^ pgm_read_word(wCRCTable + tmp);
    }
    return crc;
}

#else // CRC_LTABLE_CALC

US usCRC16(const UC * pucFrame, US usLen) {
    US i, j;
    US crc = 0xFFFF;
    US tmp;

    // Calculate the CRC.
    for (i = 0; i < usLen; i++) {
        crc = crc ^ pucFrame[i];

        for (j = 0; j < 8; j++) {
            tmp = crc & 0x0001;
            crc = crc >> 1;
            if(tmp) crc = crc ^ 0xA001;
        }
    }
    return crc;
}

#endif  // CRC_LTABLE_CALC
