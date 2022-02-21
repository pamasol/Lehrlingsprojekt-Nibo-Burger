
void setup_comm();

void loop_comm();
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

/*
    Serial Protocol
    ===============

== stream commands ==   
<*> [ESC]c - clear display
< > [ESC]r - repeat begin
< > [ESC]s - repeat start
<*> [ESC]b{n} - set matrix brightness to n=[0-9: 0-90%, *: 100%]
<*> [ESC]d{n} - dimm matrix brightness to n=[0-9: 0-90%, *: 100%]
< > [ESC]o{x} - set matrix orientation x=[n,w,e,s: North, West, East, South]
<*> [ESC]t{c} - transmit back char (synchronisation) c=[ASCII char]
<*> [ESC]l - load next matrix bytes without delay
    font modifiers:
< > [ESC]e1 - economy font size (3×6)
< > [ESC]e0 - normal font size (5×8)
< > [ESC]i1 - inverted output
< > [ESC]i0 - noninverted output
< > [ESC]p1 - proportional mode
< > [ESC]p0 - fixed width mode


<*> [ESC]B{xxxxxxxx}[SPACE] - Bargraph mode, every char is a colon, terminated by space
<*> [ESC]G{xxxxxxxx}[SPACE] - Grafic mode, bytes are hexadecimal encoded, terminated by space
<*> [ESC]P{xxx}[SPACE] - Pause given time in miliseconds
<*> [ESC]S{xxx}[SPACE] - set shift delay time (ms)
<*> [ESC]D{xxx}[SPACE] - set matrix dimm transition time (ms)
<*> [ESC]T{n}{xxx}[SPACE] - matrix dimm transition value and time
< > [ESC]X{xxx}[SPACE] - hexadecimal to decimal conversion


== immediate commands ==
<*> [DLE]c - clear and reset display
<*> [DLE]d{n} - set matrix dimm value to n=[1-9: 10-90%, 0: 100%]
< > [DLE]o{x} - set matrix orientation x=[n,w,e,s: North, West, East, South]
 
 
== examples== 

display chess pattern without scrolling
  short: "\33l\33Gaa55aa55aa55aa55 "
  alt:   MAROON_LOAD() MAROON_GFX("aa55aa55aa55aa55")

fade in question mark over 1 second, wait 1 second, fade out 1 second  
  short: "\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*"
  alt:   MAROON_DIM(0) MAROON_LOAD() "?" MAROON_TRANSITION(*, 100) MAROON_PAUSE(1000) MAROON_TRANSITION (0,100) MAROON_DIM (*)
  
print "hello" and send back "!" over uart
  short: "hello\33t!"
  
  
== Bargraph mode ==
'0'-'8' single dot
'a'-'i' bar ascending
'i'-'q' bar descending
'A'-'E' center bar fading in
'E'-'I' center bar fading out
*/

#define MAROON_CLEAR() "\33c"
#define MAROON_BRIGHT(x) "\33b" #x
#define MAROON_DIM(x) "\33d" #x
#define MAROON_TXBACK(c) "\33t" c
#define MAROON_LOAD() "\33l"

#define MAROON_FONT_ECO() "\33e1"
#define MAROON_FONT_NORM() "\33e0"
#define MAROON_FONT_FIXED() "\33p0"
#define MAROON_FONT_PROP() "\33p1"

#define MAROON_BAR(bar) "\33B" bar " "
#define MAROON_GFX(gfx) "\33G" gfx " "
#define MAROON_PAUSE(x) "\33P" #x " "
#define MAROON_STIME(x) "\33S" #x " "
#define MAROON_DTIME(x) "\33D" #x " "
#define MAROON_TRANSITION(n, x) "\33T" #n #x " "

#define MAROON_IMM_CLEAR() "\20c"
#define MAROON_IMM_DIM(x) "\20d" #x
