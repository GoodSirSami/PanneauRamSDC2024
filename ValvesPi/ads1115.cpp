#include "ads1115.h"
#include "string.h"

extern "C"
{
    #include "i2c.h"
}

ADS1115::ADS1115(const char *busName, uint8_t addr)
 {

 	_busAddr = i2c_open(busName);
 	if(_busAddr >= 0)
 	{
		_slaveAddr = addr;
		_conversionDelay = ADS1115_CONVERSION_DELAY;
		_gain = GAIN_ONE;
 	}
 }

ADS1115::~ADS1115()
{
	if(_busAddr >= 0)
		i2c_close(_busAddr);
}

uint16_t ADS1115::readRegister(uint8_t reg)
{
    uint8_t buf[10] = {0};
    uint16_t ret = 0;
    I2CDevice i2c;

 	if(_busAddr >= 0)
 	{
		i2c_init_device(&i2c);
		i2c.bus = _busAddr;
		i2c.addr = _slaveAddr;
		i2c.iaddr_bytes = 1;
		i2c.page_bytes = 2;

		i2c_read(&i2c, reg, buf, 2);

		ret = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
 	}

    return ret;
}

void ADS1115::writeRegister(uint8_t reg, uint16_t value)
{
    uint8_t buf[10] = {0};

    I2CDevice i2c;

 	if(_busAddr >= 0)
 	{
		i2c_init_device(&i2c);
		i2c.bus = _busAddr;
		i2c.addr = _slaveAddr;
		i2c.iaddr_bytes = 1;
		i2c.page_bytes = 2;

		buf[0] = (uint8_t)(value >> 8);
		buf[1] = (uint8_t)(value & 0x00FF);

		i2c_write_bytes(&i2c, reg, buf, 2);
 	}
}

uint16_t ADS1115::readChannelADC(uint8_t channel)
{
    if(channel > ADS1115_CHANNEL_A3)
    {
        return 0;
    }

    uint16_t config =   ADS1115_CQUE_NONE           |
                        ADS1115_CLAT_NON_LATCHING   |
                        ADS1115_CPOL_ACTIVE_LOW     |
                        ADS1115_TRADITIONAL_MODE    |
                        ADS1115_DR_1600SPS          |
                        ADS1115_MODE_SINGLE;

    config |= _gain;

    switch (channel)
    {
        case ADS1115_CHANNEL_A0:
        {
            config |= ADS1115_MUX_SINGLE_0;
            break;
        }
        case ADS1115_CHANNEL_A1:
        {
            config |= ADS1115_MUX_SINGLE_1;
            break;
        }
        case ADS1115_CHANNEL_A2:
        {
            config |= ADS1115_MUX_SINGLE_2;
            break;
        }
        case ADS1115_CHANNEL_A3:
        {
            config |= ADS1115_MUX_SINGLE_3;
            break;
        }
    }

    config |= ADS1115_OS_SINGLE;

    //qDebug() << "ADC Config: " << config << endl;

    writeRegister(ADS1115_POINTER_CONFIG, config);

    usleep(_conversionDelay * 1000);

    uint16_t temp = readRegister(ADS1115_POINTER_CONVERT);

    //qDebug() << "Convert: " << temp << endl;

    return temp;
}

void ADS1115::setGain(Ads1115Gain gain)
{
    _gain = gain;
}

ADS1115::Ads1115Gain ADS1115::getGain()
{
    return _gain;
}
