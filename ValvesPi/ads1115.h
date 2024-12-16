#ifndef ADS1115_H
#define ADS1115_H

#include<sys/types.h>
#include "stdint.h"
#include <unistd.h>

class ADS1115
{
public:
	enum ADS1115_SLAVE_ADDR
	{
		ADS1115_SLAVE_ADDR_A = 0x48,
		ADS1115_SLAVE_ADDR_B = 0x49,
		ADS1115_SLAVE_ADDR_C = 0x4A,
		ADS1115_SLAVE_ADDR_D = 0x4B
	};

	enum ADS1115_Mask
	{
		ADS1115_POINTER_REG_MASK          = 0x03,
		ADS1115_CONFIG_OS_MASK            = 0x8000,
		ADS1115_CONFIG_MUX_MASK           = 0x7000,
		ADS1115_CONFIG_PGA_MASK           = 0x0E00,
		ADS1115_CONFIG_MODE_MASK          = 0x0100,
		ADS1115_CONFIG_DR_MASK            = 0x00E0,
		ADS1115_CONFIG_COMP_MODE_MASK     = 0x0010,
		ADS1115_CONFIG_COMP_POLARITY_MASK = 0x0008,
		ADS1115_CONFIG_COMP_LATCH_MASK    = 0x0004,
		ADS1115_CONFIG_COMP_QUE_MASK      = 0x000
	};

	enum ADS1115_PointerRegister
	{
	    ADS1115_POINTER_CONVERT         =   0x00,
	    ADS1115_POINTER_CONFIG          =   0x01,
	    ADS1115_POINTER_LOWTHRESHOLD    =   0x02,
	    ADS1115_POINTER_HITHRESHOLD     =   0x03
	};

	enum ADS1115_OsConfigRegister
	{
	    ADS1115_OS_SINGLE   =   0x8000,
	    ADS1115_OS_BUSY     =   0x0000,
	    ADS1115_OS_NOTBUSY  =   0x8000
	};

	enum ADS1115_MuxConfigRegister
	{
	    ADS1115_MUX_DIFF_01     =   0x0000,
	    ADS1115_MUX_DIFF_03     =   0x1000,
	    ADS1115_MUX_DIFF_13     =   0x2000,
	    ADS1115_MUX_DIFF_23     =   0x3000,
	    ADS1115_MUX_SINGLE_0    =   0x4000,
	    ADS1115_MUX_SINGLE_1    =   0x5000,
	    ADS1115_MUX_SINGLE_2    =   0x6000,
	    ADS1115_MUX_SINGLE_3    =   0x7000,
	};

	enum ADS1115_PgaConfigRegister
	{
	    ADS1115_PGA_6_144V      =   0x0000,
	    ADS1115_PGA_4_096V      =   0x0200,
	    ADS1115_PGA_2_048V      =   0x0400,
	    ADS1115_PGA_1_024V      =   0x0600,
	    ADS1115_PGA_0_512V      =   0x0800,
	    ADS1115_PGA_0_256V      =   0x0A00
	};

	enum ADS1115_ModeRegister
	{
	    ADS1115_MODE_CONTINUE   =   0x0000,
	    ADS1115_MODE_SINGLE     =   0x0100
	};

	enum ADS1115_DrRegister
	{
	    ADS1115_DR_128SPS   =   0x0000,
	    ADS1115_DR_250SPS   =   0x0020,
	    ADS1115_DR_490SPS   =   0x0040,
	    ADS1115_DR_920SPS   =   0x0060,
	    ADS1115_DR_1600SPS  =   0x0080,
	    ADS1115_DR_2400SPS  =   0x00A0,
	    ADS1115_DR_3300SPS  =   0x00C0
	};

	enum ADS1115_ComparatorModeRegister
	{
	    ADS1115_TRADITIONAL_MODE    =   0x0000,
	    ADS1115_WINDOW_MODE         =   0x0010
	};

	enum ADS1115_CPOL_Register
	{
	    ADS1115_CPOL_ACTIVE_LOW     =   0x0000,
	    ADS1115_CPOL_ACTIVE_HIGH    =   0x0008
	};

	enum ADS1115_CLAT_Register
	{
	    ADS1115_CLAT_NON_LATCHING   =   0x0000,
	    ADS1115_CLAT_LATCHING       =   0x0004
	};

	enum ADS1115_CQUE_Register
	{
	    ADS1115_CQUE_1CONVERSION    =   0x0000,
	    ADS1115_CQUE_2CONVERSION    =   0x0001,
	    ADS1115_CQUE_4CONVERSION    =   0x0002,
	    ADS1115_CQUE_NONE           =   0x0003
	};

	enum Ads1115Gain
	{
	    GAIN_TW0THIRDS  =   ADS1115_PGA_6_144V,
	    GAIN_ONE        =   ADS1115_PGA_4_096V,
	    GAIN_TWO        =   ADS1115_PGA_2_048V,
	    GAIN_FOUR       =   ADS1115_PGA_1_024V,
	    GAIN_EIGHT      =   ADS1115_PGA_0_512V,
	    GAIN_SIXTEEN    =   ADS1115_PGA_0_256V
	};

	enum ADS1115_Channels
	{
	    ADS1115_CHANNEL_A0  =   0,
	    ADS1115_CHANNEL_A1  =   1,
	    ADS1115_CHANNEL_A2  =   2,
	    ADS1115_CHANNEL_A3  =   3
	};

private:
    int8_t _busAddr;
    uint8_t _slaveAddr;
    uint8_t _conversionDelay;
    Ads1115Gain _gain;

    const uint8_t ADS1115_CONVERSION_DELAY = 8;  // ms

    uint16_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint16_t value);

public:
    ADS1115(const char *busName, uint8_t addr = ADS1115_SLAVE_ADDR_A);
    ~ADS1115();

    uint16_t readChannelADC(uint8_t channel);

    void setGain(Ads1115Gain gain);
    Ads1115Gain getGain(void);
};

#endif // ADS1115_H
