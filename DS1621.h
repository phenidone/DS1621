/* -*- C++ -*- */
/*
    Copyright (C) 2012-2016 William Brodie-Tyrrell
    william@brodie-tyrrell.org
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of   
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef _DS1621_WIBT_
#define _DS1621_WIBT_

#include <stdint.h>

/**
 * Simple driver for DS1621 I2C temperature sensor.
 */
class DS1621 {
public:

    DS1621(uint8_t addr);

    /// set temp-low threshold
    void setTL(short tl);
    /// get temp-low threshold
    short getTL();
    /// set temp-high threshold
    void setTH(short th);
    /// get temp-high threshold
    short getTH();
    /// set config byte
    void setConfig(uint8_t c);
    /// get config byte
    uint8_t getConfig();
    
    /// initiate a conversion; required in one-shot mode
    void convert();
    /// get last conversion
    short getTemp();
    /// wait until the DONE flag is set
    /// @return true on success, false on timeout
    bool waitDone();
    /// wait untilo nvram is finished
    /// @return true on success, false on timeout
    bool waitNv();

    /// format conversion
    static float tempToDegC(short t);
    /// format conversion
    static short tempFromDegC(float c);
    
    // bits in the config byte
    static const uint8_t CFG_DONE=0x80;     ///< conversion complete
    static const uint8_t CFG_THF=0x40;      ///< T > TH
    static const uint8_t CFG_TLF=0x20;      ///< T < TL
    static const uint8_t CFG_NVB=0x10;      ///< nvram busy
    static const uint8_t CFG_CONST=0x80;    ///< bits set in bottom of CFG
    static const uint8_t CFG_POL=0x02;      ///< polarity
    static const uint8_t CFG_1SHOT=0x01;    ///< one-shot
    static const uint8_t CFG_CFG=0x03;      ///< static part of the configuration

    /// returned on failure of getT*, is below abs. zero
    static const short INVALID_TEMP=-1000;

private:

    /// send command
    void write0(uint8_t reg);
    /// write to a register
    void write1(uint8_t reg, uint8_t val);
    /// write to a register
    void write2(uint8_t reg, uint16_t val);
    /// read from a register
    uint32_t read(uint8_t reg, uint8_t bytes);

    /// 7-bit I2C address
    const uint8_t busaddr;

    static const int CONVERT_TIMEOUT=1500;
    static const int WRITE_TIMEOUT=200;

    static const uint8_t CONVERT=0xEE;
    static const uint8_t STOP_CONVERT=0x22;
    static const uint8_t REG_TEMP=0xAA;
    static const uint8_t REG_TH=0xA1;
    static const uint8_t REG_TL=0xA2;
    static const uint8_t REG_CONF=0xAC;
    static const uint8_t REG_COUNTER=0xA8;
    static const uint8_t REG_SLOPE=0xA9;

};

#endif
