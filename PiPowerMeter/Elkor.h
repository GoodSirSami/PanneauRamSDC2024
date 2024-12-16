/*
 * POWER METER ELKOR
 *
 */
 
#ifndef ELKOR_H
#define ELKOR_H

#define ELKOR_ADDR   0x02

/* Registres de mesure du ELKOR */
#define ADD_REG_TOTAL_ENERGY_CONSUMPTION     0x0300
#define ADD_REG_PHASE_A_TOTAL_ENERGY         0x0350
#define ADD_REG_PHASE_B_TOTAL_ENERGY         0x0352
#define ADD_REG_PHASE_C_TOTAL_ENERGY         0x0354
#define ADD_REG_TOTAL_REAL_POWER             0x0302
#define ADD_REG_PHASE_A_REAL_POWER           0x0326
#define ADD_REG_PHASE_B_REAL_POWER           0x0328
#define ADD_REG_PHASE_C_REAL_POWER           0x032E
#define ADD_REG_TOTAL_APPARENT_POWER         0x0306
#define ADD_REG_AVERAGE_VOLTAGE_L_N          0x0308
#define ADD_REG_AVERAGE_VOLTAGE_L_L          0x030A
#define ADD_REG_AVERAGE_COURANT              0x030C
#define ADD_REG_VOLTAGE_A_N                  0x0314
#define ADD_REG_VOLTAGE_B_N                  0x0316
#define ADD_REG_VOLTAGE_C_N                  0x0318
#define ADD_REG_VOLTAGE_A_B                  0x031A
#define ADD_REG_VOLTAGE_B_C                  0x031C
#define ADD_REG_VOLTAGE_A_C                  0x031E
#define ADD_REG_COURANT_A                    0x0320
#define ADD_REG_COURANT_B                    0x0322
#define ADD_REG_COURANT_C                    0x0324
#define ADD_POWER_FACTOR_A                   0x0338
#define ADD_POWER_FACTOR_B                   0x033A
#define ADD_POWER_FACTOR_C                   0x033C

unsigned int calculerCRC(unsigned char* input, unsigned int len);

#endif //ELKOR_H



