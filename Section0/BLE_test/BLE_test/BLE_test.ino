#include <CurieBle.h>
/*
This program is an example to use the Nordic UART emulator UUID on a Genuino 101
Here, we receive data from a client, if the data is the character '1' we turn on pin 13
if the data is character '2' we turn off pin 13
*/

const int LED = 13;//led pin
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)


/*
Here we create the BLE Peripheral
The peripheral is used to communicate with the Phone/any other client
We give it a UUID - Universally unique identifier
This UUID is used by the application nRF Toolbox (which should be installed on your phone by now)
as a UART emulator. UART is universal asynchronous receiver transmitter.
Basically a device that can send and receive serial data.
*/
BLEService uartService = BLEService("6E400001B5A3F393E0A9E50E24DCCA9E");

/*
Now we give characteristics to the device.
A characteristic is a way to interact with the device. Either by giving it data, or reading from it.
RX = received data
TX = transmitted data
*/

//Write without response: Client writes data, and doesn't expect response
BLECharacteristic rxCharacteristic = BLECharacteristic("6E400002B5A3F393E0A9E50E24DCCA9E", BLEWriteWithoutResponse, 20); 

//notify: send data to client, don't wait for response
BLECharacteristic txCharacteristic = BLECharacteristic("6E400003B5A3F393E0A9E50E24DCCA9E", BLENotify , 20); 

//the central is the device, and the characteristic is the characteristic that the client wrote into
void writeHandler(BLECentral& central, BLECharacteristic& characteristic);

void setup()
{
	pinMode(LED,OUTPUT);
	/*
	Bluetooth setup:
	*/
	
	//First set the name of the peripheral, I choose this name.
	//Change the name to something more unique
	blePeripheral.setLocalName("BLE LED");
	
	//Then, we set the UUID that the client sees as a UART device
	blePeripheral.setAdvertisedServiceUuid(uartService.uuid());

	/*
	Now, we add some attributes, which are stuff about the device and stuff the device can do.
	uartDevice: the UART emulator part
	rxCharacteristic and txCharacteristic: the ability to send and receive data.
	*/
	blePeripheral.addAttribute(uartService);
	blePeripheral.addAttribute(rxCharacteristic);
	blePeripheral.addAttribute(txCharacteristic);

	/*
	Here is the part that allows to do custom control:
	We tell the genuino, that when it receives data, do something with it.
	In this case, when it receives data, it calls the function writeHandler, which turns on or off the LED.
	*/
	rxCharacteristic.setEventHandler(BLECharacteristicEvent::BLEWritten, writeHandler);

	//Now that we finally finished the configuration, we can startup the device
	blePeripheral.begin();
}

void loop()
{
	//listen to data
	blePeripheral.poll();
}

/*
This function is the function that handles any incoming data.
It receives 2 parameters:
1. BLECentral& central: the device that connected to the genuino
2. BLECharacteristic& characteristic: that characteristic was written into
*/
void writeHandler(BLECentral& central, BLECharacteristic& characteristic)
{
	//the can be an empty string, if it is the value is equal 0
	
	if(characteristic.value())
	{
		/*
		char value will return the first letter of the value. Here is can be '1', '2' or any other letter
		NOTE: '1' is not the number 1, it is equal to something else, the ASCII value of 1.
		For more information about ASCII: http://www.asciitable.com/
		*/

		if(characteristic.charValue() == '1')
		{
			digitalWrite(LED,HIGH);
		}
		else if(characteristic.charValue() == '2')
		{
			digitalWrite(LED,LOW);
		}
	}
}
