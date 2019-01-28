//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU	: Arduino Nano
//-------------------------------------------------------------------------------------

#ifndef HX711_ADC_h
#define HX711_ADC_h

#include <Arduino.h>





class HX711_ADC
{	
		
	public:
		HX711_ADC(uint8_t dout, uint8_t sck); 	    //constructor
		void setGain(uint8_t gain = 128); 			//value should be 32, 64 or 128*
		void begin();
		void begin(uint8_t gain);
		int startMultiple(unsigned int t); 			//start and tare multiple HX711 simultaniously
		float getConversionTime(); 					//for testing and debugging
		float getSPS();								//for testing and debugging
		void powerDown(); 
		void powerUp(); 
		unsigned long getNewData(); 							


	protected:
		

		unsigned long conversion24bit(); 					//if conversion is ready: returns 24 bit data and starts the next conversion
		uint8_t sckPin; 							//HX711 pd_sck pin
		uint8_t doutPin; 							//HX711 dout pin
		uint8_t GAIN;
		
		unsigned long conversionStartTime;
		unsigned long conversionTime;
	
		
};	

#endif
   