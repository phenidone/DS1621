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

#include <DS1621.h>
#include <Wire.h>
#include <math.h>
#include <Arduino.h>

using namespace std;

DS1621::DS1621(uint8_t addr)
    : busaddr(0x48 | (addr & 0x07))
{
}


void DS1621::convert()
{
    write0(CONVERT);
}

short DS1621::getTemp()
{
    // if(!waitDone()){
    //    return INVALID_TEMP;
    //}
    return (short) read(REG_TEMP, 2);
}

bool DS1621::waitDone()
{
    uint32_t start=millis();

    if((getConfig() & CFG_DONE) != 0){
        return true;
    }

    do {
        delay(5);
        if((getConfig() & CFG_DONE) != 0){
            return true;
        }
    } while(millis() - start < CONVERT_TIMEOUT);

    return false;
}

bool DS1621::waitNv()
{
    uint32_t start=millis();

    if((getConfig() & CFG_NVB) == 0){
        return true;
    }

    do {
        delay(5);
        if((getConfig() & CFG_NVB) == 0){
            return true;
        }
    } while(millis() - start < WRITE_TIMEOUT);

    return false;
}

uint8_t DS1621::getConfig()
{
    return (uint8_t) read(REG_CONF, 1);
}

void DS1621::setConfig(uint8_t cfg)
{
    if(waitNv()){
        write1(REG_CONF, cfg);
    }
}

void DS1621::setTH(short th)
{
    if(waitNv()){
        write2(REG_TH, th);
    }
}

void DS1621::setTL(short tl)
{
    if(waitNv()){
        write2(REG_TL, tl);
    }
}

short DS1621::getTH()
{
    waitNv();
    return (short) read(REG_TH, 2);
}

short DS1621::getTL()
{
    waitNv();    
    return (short) read(REG_TL, 2);
}


void DS1621::write0(uint8_t reg)
{
    Wire.beginTransmission(busaddr);
    Wire.write(reg);
    Wire.endTransmission();
}

void DS1621::write1(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(busaddr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

void DS1621::write2(uint8_t reg, uint16_t val)
{
    Wire.beginTransmission(busaddr);
    Wire.write(reg);
    Wire.write((val >> 8) & 0xFF);
    Wire.write(val & 0xFF);
    Wire.endTransmission();
}


uint32_t DS1621::read(uint8_t reg, uint8_t bytes)
{
    if(bytes > 4)
        bytes=4;

    write0(reg);    
    Wire.requestFrom(busaddr, bytes);
    uint32_t res=0;
    while(Wire.available()){
        res <<= 8;
        res |= Wire.read();
    }

    return res;
}


float DS1621::tempToDegC(short t)
{
    return (t>>7) * 0.5f;
}

short DS1621::tempFromDegC(float c)
{
    return ((short)roundf(c*2.0f)) << 7;
}
