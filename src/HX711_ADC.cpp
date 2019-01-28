//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU	: Arduino Nano
// IF getNewData does an endless loop its probably the out of range check, try lower gain
//-------------------------------------------------------------------------------------

#include <Arduino.h>
#include <HX711_ADC.h>

HX711_ADC::HX711_ADC(uint8_t dout, uint8_t sck) //constructor
{
	doutPin = dout;
	sckPin = sck;
}

void HX711_ADC::begin()
{
	pinMode(sckPin, OUTPUT);
	pinMode(doutPin, INPUT);
	setGain(128);
	powerUp();
}

void HX711_ADC::begin(uint8_t gain)
{
	pinMode(sckPin, OUTPUT);
	pinMode(doutPin, INPUT);
	setGain(gain);
	powerUp();
}


unsigned long HX711_ADC::getNewData()
{
	unsigned long dataSample;
	do
	{
		while (digitalRead(doutPin))
			yield();
		dataSample = conversion24bit();
	}while(dataSample == 0);  // if it 0 it was out of range, retry
	return dataSample;
}

unsigned long HX711_ADC::conversion24bit() //read 24 bit data and start the next conversion
{
	//conversionTime = micros() - conversionStartTime; //MAYBE comment out for performance
	//conversionStartTime = micros();
	unsigned long data = 0;

	for (uint8_t i = 0; i < (24 + GAIN); i++)
	{						  //read 24 bit data + set gain and start next conversion
		delayMicroseconds(1); // required for faster mcu's?
		digitalWrite(sckPin, 1);
		delayMicroseconds(1);
		if (i < (24))
		{
			data = data << 1;
			if (digitalRead(doutPin))
			{
				data++;
			}
		}
		digitalWrite(sckPin, 0);
	}
	data = data ^ 0x800000; // if out of range (min), change to 0
	return (data);
	
}




void HX711_ADC::powerDown()
{
	digitalWrite(sckPin, LOW);
	digitalWrite(sckPin, HIGH);
}

void HX711_ADC::powerUp()
{
	digitalWrite(sckPin, LOW);
}



//for testing and debugging:
//returns latest conversion time in millis
float HX711_ADC::getConversionTime()
{
	return conversionTime / 1000.0;
}

//for testing and debugging:
//returns the HX711 samples ea seconds based on the latest conversion time.
//The HX711 can be set to 10SPS or 80SPS. For general use the recommended setting is 10SPS.
float HX711_ADC::getSPS()
{
	float sps = 1000000.0 / conversionTime;
	return sps;
}

void HX711_ADC::setGain(uint8_t gain) //value should be 32, 64 or 128*
{
	if (gain < 64)
		GAIN = 2; //32, channel B
	else if (gain < 128)
		GAIN = 3; //64, channel A
	else
		GAIN = 1; //128, channel A
}