/*  BSD-License:

Copyright (c) 2015 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*! @file    iodefs_niboburger.h
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 */

#define TICKS_PER_METER 705
#define TICKS_PER_TURN_SINGLE 520
#define TICKS_PER_TURN 500

/*! LED group */
#define IO_LEDS_PORT  PORTB
#define IO_LEDS_MASK  0x1e
#define IO_LEDS_DDR   DDRB
#define IO_LEDS_BIT_1 1
#define IO_LEDS_BIT_2 2
#define IO_LEDS_BIT_3 3
#define IO_LEDS_BIT_4 4



/*! Motor group */
#define IO_MOTOR_PORT  PORTD
#define IO_MOTOR_PIN   PIND
#define IO_MOTOR_MASK  0xf0
#define IO_MOTOR_DDR   DDRD
#define IO_MOTOR_BIT_PWM_LEFT  4
#define IO_MOTOR_BIT_PWM_RIGHT 5
#define IO_MOTOR_BIT_DIR_LEFT  6
#define IO_MOTOR_BIT_DIR_RIGHT 7


/*! Odometry group */
#define IO_ODO_PORT  PORTD
#define IO_ODO_PIN   PIND
#define IO_ODO_MASK  0x0c
#define IO_ODO_DDR   DDRD
#define IO_ODO_BIT_LEFT  2
#define IO_ODO_BIT_RIGHT 3



/*! Sensor board enable Signals */
#define IO_EN_FL        IO_EN_FL
#define IO_EN_FL_PORT   PORTC
#define IO_EN_FL_BIT    4
#define IO_EN_FL_DDR    DDRC

#define IO_EN_FR        IO_EN_FR
#define IO_EN_FR_PORT   PORTC
#define IO_EN_FR_BIT    5
#define IO_EN_FR_DDR    DDRC

#define IO_EN_BL        IO_EN_BL
#define IO_EN_BL_PORT   PORTC
#define IO_EN_BL_BIT    6
#define IO_EN_BL_DDR    DDRC

#define IO_EN_BR        IO_EN_BR
#define IO_EN_BR_PORT   PORTC
#define IO_EN_BR_BIT    7
#define IO_EN_BR_DDR    DDRC

#define IO_EN_FLL       IO_EN_FLL
#define IO_EN_FLL_PORT  PORTC
#define IO_EN_FLL_BIT   2
#define IO_EN_FLL_DDR   DDRC

#define IO_EN_BC        IO_EN_BC
#define IO_EN_BC_PORT   PORTB
#define IO_EN_BC_BIT    0
#define IO_EN_BC_DDR    DDRB

#define IO_EN_FRR       IO_EN_FRR
#define IO_EN_FRR_PORT  PORTC
#define IO_EN_FRR_BIT   3
#define IO_EN_FRR_DDR   DDRC




/*! I2C SDA */
#define IO_I2C_SDA_PORT  PORTC
#define IO_I2C_SDA_PIN   PINC
#define IO_I2C_SDA_BIT   1
#define IO_I2C_SDA_DDR   DDRC

/*! I2C SCL */
#define IO_I2C_SCL_PORT  PORTC
#define IO_I2C_SCL_PIN   PINC
#define IO_I2C_SCL_BIT   0
#define IO_I2C_SCL_DDR   DDRC



/*! RXD */
#define IO_UART_RXD_PORT  PORTD
#define IO_UART_RXD_PIN   PIND
#define IO_UART_RXD_BIT   0
#define IO_UART_RXD_DDR   DDRD

/*! TXD */
#define IO_UART_TXD_PORT  PORTD
#define IO_UART_TXD_PIN   PIND
#define IO_UART_TXD_BIT   1
#define IO_UART_TXD_DDR   DDRD



/*! MISO */
#define IO_SPI_MISO_PORT  PORTB
#define IO_SPI_MISO_PIN   PINB
#define IO_SPI_MISO_BIT   6
#define IO_SPI_MISO_DDR   DDRB

/*! MOSI */
#define IO_SPI_MOSI_PORT  PORTB
#define IO_SPI_MOSI_PIN   PINB
#define IO_SPI_MOSI_BIT   5
#define IO_SPI_MOSI_DDR   DDRB

/*! SCK */
#define IO_SPI_SCK_PORT   PORTB
#define IO_SPI_SCK_PIN    PINB
#define IO_SPI_SCK_BIT    7
#define IO_SPI_SCK_DDR    DDRB




/*! Analog channels */
#define IO_ANALOG_PORT  PORTA
#define IO_ANALOG_PIN   PINA
#define IO_ANALOG_MASK  0xff
#define IO_ANALOG_DDR   DDRA

#define IO_ANALOG_BIT_SENS_FRR      7        /*!< ADC-PIN sensor outer front right */
#define IO_ANALOG_BIT_SENS_BC       6        /*!< ADC-PIN sensor back/bottom center */
#define IO_ANALOG_BIT_SENS_FLL      5        /*!< ADC-PIN sensor outer front left */
#define IO_ANALOG_BIT_KEYBOARD      4        /*!< ADC-PIN keyboard voltage */
#define IO_ANALOG_BIT_SENS_BR       3        /*!< ADC-PIN sensor back/bottom right */
#define IO_ANALOG_BIT_SENS_BL       2        /*!< ADC-PIN sensor back/bottom left */
#define IO_ANALOG_BIT_SENS_FR       1        /*!< ADC-PIN sensor front right */
#define IO_ANALOG_BIT_SENS_FL       0        /*!< ADC-PIN sensor front left */


