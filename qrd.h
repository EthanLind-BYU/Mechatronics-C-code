 // This program reads from an A/D channel in manual mode.

#include <xc.h>
#ifndef QRD_H
#define QRD_H
#define THRESHOLD 1024
#define QRDL ADC1BUF0
#define QRDR ADC1BUF1
#define QRDM ADC1BUF4
void qrd_int(void);
#endif
