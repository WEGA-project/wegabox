// BMx280_otherInterfaces.ino
//
// shows how to use the BMx280 library using the SoftWire Library https://github.com/stevemarple/SoftWire . 
//
// Copyright (c) 2021 Gregor Christandl
//
// connect the BMx280 to the Arduino like this:
//
// Arduino - BMx280
// 3.3V ---- VCC
// GND ----- GND
// D10 ---- SDA/SDI
// D11 ---- SKC/SCL
// some BMP280/BME280 modules break out the CSB and SDO pins as well: 
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)
// other pins can be left unconnected.

#include <Arduino.h>

#include <AsyncDelay.h>		//not needed in this example code, but is required by SoftWire. 
#include <SoftWire.h>

#include <BMx280MI.h>

#define I2C_ADDRESS 0x76

//set your desired I2C Pins here
uint8_t sdaPin = 10;
uint8_t sclPin = 11;
SoftWire sw(sdaPin, sclPin);

//class derived from BMx280MI that implements communication via SoftWire. I used low level commands because high
//level commands showed unexpected behaviour (SoftWire V 2.0.0) 
class BMx280SoftWire : public BMx280MI
{
	public:	
		//constructor of the derived class. 
		//@param address i2c address of the sensor.
		BMx280SoftWire(uint8_t i2c_address):
		address_(i2c_address)	//initialize the BMx280sw classes private member address_ to the i2c address provided
		{
			//nothing else to do here...
		}
	
	private:
		//this function must be implemented by derived classes. it is used to initialize the interface or check the sensor for example. 
		//@return true on success, false otherwise. 
		bool beginInterface()
		{
			return true;
		}

		//this function must be implemented by derived classes. this function is responsible for reading data from the sensor. 
		//@param reg register to read. 
		//@return read data (1 byte).
		uint8_t readRegister(uint8_t reg)
		{
			uint8_t data;

			sw.startWait(address_, SoftWire::writeMode);
			sw.llWrite(reg);

			sw.repeatedStart(address_, SoftWire::readMode);
			sw.readThenNack(data);  // Read 1 byte and then send ack
			sw.stop();

			return data;
		}
		
		//this function can be implemented by derived classes. implementing this function is optional, but readings may be incorrect if 
		//it is not (see BMP280 / BME280 datasheet). 
		//@param reg register to read. 
		//@param length number of registers to read (max: 4)
		//@return read data. LSB = last register read. 
		uint32_t readRegisterBurst(uint8_t reg, uint8_t length)
		{
			if (length > 4 || length == 0)
				return 0L;

			uint32_t data = 0L;
			uint8_t curr_byte = 0;

			sw.startWait(address_, SoftWire::writeMode);
			sw.llWrite(reg);

			sw.repeatedStart(address_, SoftWire::readMode);

			uint8_t i = 0;
			for (; i < length - 1; i++){
				sw.readThenAck(curr_byte);  // Read 1 byte and then send ack
				data = (data << 8) | curr_byte;
			}

			sw.readThenNack(curr_byte);  // Read 1 byte and then send ack
			data = (data << 8) | curr_byte;

			sw.stop();

			return data;
		}

		//this function must be implemented by derived classes. this function is responsible for sending data to the sensor. 
		//@param reg register to write to.
		//@param data data to write to register.
		void writeRegister(uint8_t reg, uint8_t data)
		{
			sw.startWait(address_, SoftWire::writeMode);
			sw.llWrite(reg);
			sw.llWrite(data);
			sw.stop();
		}
		
		uint8_t address_;		//i2c address of sensor
};

//create an BMx280 object using the SoftWire Interface
BMx280SoftWire bmx280(I2C_ADDRESS);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);

	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);
	
	//SoftWire setup
	sw.setDelay_us(5);
	sw.setTimeout_ms(100);
	sw.begin();

	//begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
	//and reads compensation parameters.
	if (!bmx280.begin())
	{
		Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
		while (1);
	}

	//reset sensor to default parameters.
	bmx280.resetToDefaults();

	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
		bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
}

void loop() {
	// put your main code here, to run repeatedly:

	delay(1000);

	//start a measurement
	if (!bmx280.measure())
	{
		Serial.println("could not start measurement, is a measurement already running?");
		return;
	}

	//wait for the measurement to finish
	do
	{
		delay(100);
	} while (!bmx280.hasValue());

	//important: measurement data is read from the sensor in function hasValue() only. 
	//make sure to call get*() functions only after hasValue() has returned true. 
	Serial.print("Pressure: "); Serial.println(bmx280.getPressure());
	Serial.print("Pressure (64 bit): "); Serial.println(bmx280.getPressure64());
	Serial.print("Temperature: "); Serial.println(bmx280.getTemperature());

	if (bmx280.isBME280())
	{
		Serial.print("Humidity: "); 
		Serial.println(bmx280.getHumidity());
	}
}