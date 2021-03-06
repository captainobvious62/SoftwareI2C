/*
  SoftwareI2C.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  This is a Software I2C Library, can act as I2c master mode.
  
  @author  Loovee
  @version 1.0
  @date    01-Nov-2013

  @author  Mark Cooke
  @version 1.1
  @date    01-Sep-2017
  @brief   uchar -> uint8_t, standardise interface to better match Wire
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_

#include <stdint.h>
#ifdef __AVR__
#include <util/delay_basic.h>
#endif

class SoftwareI2C
{
private:
    uint8_t _pinSda;
    uint8_t _pinScl;
    
    int16_t _rx_len;
    uint8_t _sda_in_out;

    uint16_t _bit_delay;
    uint16_t _bit_delay_us;

    uint8_t _i2c_address;

    uint8_t _success;
    bool _transmissionBegun;
    
    inline void sdaSet(uint8_t ucDta); 
    inline void sclSet(uint8_t ucDta);                                                                   

    inline void sendStart(void);
    inline void sendStop(void);
    inline uint8_t getAck(void);
    inline void sendByte(uint8_t ucDta);
    inline uint8_t sendByteAck(uint8_t ucDta);  // send byte and get ack
    // Note : tunedDelay should be used to throttle comms by appropriately delaying the SCL toggling
    //        However the I2C devices i work with are tolerant to non-standard rates, so this is not implemented
#ifdef __AVR__
    inline void tunedDelay(uint16_t delay) { _delay_loop_2(delay); }
#else
    inline void tunedDelay(uint16_t delay)
    {
      uint32_t delay_us = (4000000 / F_CPU);
      delay_us *= delay;
      delayMicroseconds(delay_us);
    }
#endif
    
public:
    SoftwareI2C(uint8_t pinSda = 7, uint8_t pinScl = 8) : _pinScl(pinScl), _pinSda(pinSda), _rx_len(0), _sda_in_out(0), _success(0), _transmissionBegun(false), _i2c_address(0x3C) { setClock(100000); }
    void init(uint8_t pinSda = 7, uint8_t pinScl = 8) { _pinSda = pinSda; _pinScl = pinScl; }
    void begin();
    void beginTransmission(uint8_t addr);
    uint8_t endTransmission(bool stopBit = false);
    void setClock(uint32_t baudrate);
    void end() {}
    
    uint8_t write(uint8_t dta);
    uint8_t write(uint8_t *dta, uint8_t len);
    uint8_t requestFrom(uint8_t addr, uint8_t len);
    uint8_t read();
    uint8_t available() { return _rx_len; }
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
