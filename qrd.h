#include <xc.h>
#ifndef QRD_H
#define QRD_H
#define THRESHOLD 2500
#define QRDL ADC1BUF0 //pin2
#define QRDR ADC1BUF4 //pin6
#define QRDM ADC1BUF1 //pin3
#define QRDB ADC1BUF12 //pin15
#define SAMPLE_SCAN ADC1BUF10 //pin17
#define LASER_DETECT ADC1BUF13 //pin 7
void qrd_init(void);
#endif
