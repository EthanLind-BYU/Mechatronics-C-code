 // This program reads from an A/D channel in manual mode.

#include <xc.h>
#ifndef QRD_H
#define QRD_H
#define THRESHOLD 2500
#define QRDL ADC1BUF0 //pin2
#define QRDR ADC1BUF4 //pin6
#define QRDM ADC1BUF1 //pin3
#define QRDB ADC1BUF12 //pin15
#define QRDE ADC1BUF9 //pin18
void qrd_init(void);
#endif
