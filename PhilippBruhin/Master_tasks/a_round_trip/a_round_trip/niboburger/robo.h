/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

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

/*! @file    robo.h
 *  @brief   Hiermit werden alle NIBO burger-Includes eingebunden! 
 *  Für die Haupt-C-Datei steht die Datei <niboburger/robomain.h> zur verfügung in der auch setup() und loop() deklariert werden! 
 *
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2014-10-31
 */

#ifndef _NIBOBURGER_ROBO_H_
#define _NIBOBURGER_ROBO_H_

#include <avr/interrupt.h>
#include "utils.h"
#include "iodefs.h"
#include "analog.h"
#include "base.h"
#include "clock.h"
#include "delay.h"
#include "led.h"
#include "surface.h"
#include "motpid.h"
#include "motpwm.h"
#include "odometry.h"
#include "color.h"
#include "storage.h"
#include "usart.h"
#include "key.h"

#endif
