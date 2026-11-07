 // This program reads from an A/D channel in manual mode.

#include <xc.h>
#ifndef QRD_H
#define QRD_H
#define THRESHOLD 2000
#define QRDL ADC1BUF0
#define QRDR ADC1BUF4
#define QRDM ADC1BUF1
void qrd_int(void);
#endif
