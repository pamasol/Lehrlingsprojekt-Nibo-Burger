#ifndef _MTHREAD_CONF_H_
#define _MTHREAD_CONF_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define MTHREAD_PRIO_MAX      4
#define MTHREAD_FREQ          100

// Use Timer 2 for preemtive scheduler
#if defined (OCR2)
// ATmega 16, 32
#define MTHREAD_TMR_vect       TIMER2_COMP_vect
#define MTHREAD_TMR_OCR        OCR2
#define MTHREAD_TMR_PS         1024
#define MTHREAD_TMR_TIMSK      TIMSK
#define MTHREAD_TMR_TIMSK_FLG  _BV(OCIE2) 
#define MTHREAD_TMR_TCCR       TCCR2
#define MTHREAD_TMR_TCCR_INIT  (_BV(WGM21) | _BV(CS22) | _BV(CS21) | _BV(CS20))
#endif

#if defined (OCR2A)
// ATmega 164, 324, 644, 1284
#define MTHREAD_TMR_vect       TIMER2_COMPA_vect
#define MTHREAD_TMR_OCR        OCR2A
#define MTHREAD_TMR_PS         1024
#define MTHREAD_TMR_TIMSK      TIMSK2
#define MTHREAD_TMR_TIMSK_FLG  _BV(OCIE2A) 
#define MTHREAD_TMR_TCCRA      TCCR2A
#define MTHREAD_TMR_TCCRA_INIT (_BV(WGM21))
#define MTHREAD_TMR_TCCRB      TCCR2B
#define MTHREAD_TMR_TCCRB_INIT (_BV(CS22) | _BV(CS21) | _BV(CS20))
#endif


#endif
