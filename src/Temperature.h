#ifndef TEMPERATURE_H
#define TEMPERATURE_H

float Temp(int pin_no, float R, float A, float B, float C);
bool updateTemp(void *);
bool updateTempRate(void *);

extern float meat;
extern float air;
extern float meat_;
extern float air_;
extern int pin_air;
extern int pin_meat;
extern float update_r;
extern int update_rr;
extern float R_m;
extern float A_m;
extern float B_m;
extern float C_m;
extern float R_a;
extern float A_a;
extern float B_a;
extern float C_a;
extern float meat_r;
extern float air_r;
extern unsigned long time_m_;
extern unsigned long time_a_;

#endif