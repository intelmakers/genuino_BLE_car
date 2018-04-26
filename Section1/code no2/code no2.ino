#include <CurieBLE.h>
#define DEVICE_NAME "GENUINO CAR"
//motor control pins
const int m1 = 8; 
const int m2 = 9;
const int m3 = 10;
const int m4 = 11;
const int ledPin1 =	13;// set ledPin to use on-board LED
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)

// ====  create Nordic Semiconductor UART service =========
BLEService uartService = BLEService("6E400001B5A3F393E0A9E50E24DCCA9E");

// create characteristics
//write to motor, to change direction
BLECharacteristic rxCharacteristic = BLECharacteristic("6E400002B5A3F393E0A9E50E24DCCA9E", BLEWriteWithoutResponse, 20);  // == TX on central (android app)
BLECharacteristic txCharacteristic = BLECharacteristic("6E400003B5A3F393E0A9E50E24DCCA9E", BLENotify , 20); // == RX on central (android app)

//function prototypes
/*
Function called when device connects
*/
void connectHandler(BLECentral& central);

/*
Function called when device disconnects
*/
void disconnectHandler(BLECentral& central);

/*
Function to change the motor movement
*/
void motorState(byte state);

/*
Function called when user updates the motor state
*/
void writeHandler(BLECentral& central, BLECharacteristic& characteristic);

//main functions, setup and loop
void setup()
{
	Serial.begin(9600);

	pinMode(ledPin1, OUTPUT); // use the LED on pin 13 as an output
	//motor pins output
	pinMode(m1, OUTPUT);
	pinMode(m2, OUTPUT);
	pinMode(m3, OUTPUT);
	pinMode(m4, OUTPUT);
	/*
	Bluetooth setup:
	*/
	// set advertised local name and service UUID:
	blePeripheral.setLocalName(DEVICE_NAME);
	blePeripheral.setAdvertisedServiceUuid(uartService.uuid());

	// add service, rx and tx characteristics:
	blePeripheral.addAttribute(uartService);
	blePeripheral.addAttribute(rxCharacteristic);
	blePeripheral.addAttribute(txCharacteristic);

	// assign event handlers for connected, disconnected to peripheral
	blePeripheral.setEventHandler(BLEConnected, connectHandler);
	blePeripheral.setEventHandler(BLEDisconnected, disconnectHandler);

	// assign event handler for characteristic
	rxCharacteristic.setEventHandler(BLEWritten, writeHandler);

	// advertise the service
	blePeripheral.begin();
}

void loop()
{
	// poll ble peripheral
	blePeripheral.poll();  
}

//function implementations

/*
Connect and disconnect handlers.
if device is connected, turn on LED on pin 13.
If disconnected, turn off LED on pin 13
*/
void connectHandler(BLECentral& central)
{
	// central connected event handler
	Serial.print("Connected event, central: ");
	Serial.println(central.address());
	//Serial.println("LED on");
	digitalWrite(ledPin1, HIGH);

}

void disconnectHandler(BLECentral& central)
{
// central disconnected event handler
	Serial.print("Disconnected event, central: ");
	Serial.println(central.address());
	//Serial.println("LED off");
	digitalWrite(ledPin1, LOW);
	motorState(0);

}

/*
Function called when user updates the motor state
*/
void writeHandler(BLECentral& central, BLECharacteristic& characteristic)
{
	// central wrote new value to characteristic, update LED
	Serial.print("Characteristic event, written: ");
	if (characteristic.value())
	{		
		int state = *characteristic.value();
		Serial.println((char)state);      //print out the character to the serial monitor
		//change motor state
		motorState(state);

	}

}
/*
This function changes the motor state. 
Meaning if the car goes forward, backward, left or right
*/
void motorState(byte state)
{
	static byte lastState = 0;
	if(state != lastState)
	{
		lastState = state;
		//choose which number will change direction, assuming that 1 will be forward:
		digitalWrite(m1, LOW);			
		digitalWrite(m2, LOW);
		digitalWrite(m3, LOW);
		digitalWrite(m4, LOW);
		delay(100);
		if (state == '1')
		{
			digitalWrite(m1, HIGH);
			digitalWrite(m2, LOW);
			digitalWrite(m3, HIGH);
			digitalWrite(m4, LOW);
		}
		else if (state == '2')
		{
			digitalWrite(m1, LOW);
			digitalWrite(m2, HIGH);
			digitalWrite(m3, HIGH);
			digitalWrite(m4, LOW);
		}
		else if (state == '3')
		{
			digitalWrite(m1, HIGH);			
			digitalWrite(m2, LOW);
			digitalWrite(m3, LOW);
			digitalWrite(m4, HIGH);
		}
		else if (state == '4')
		{
			digitalWrite(m1, LOW);			
			digitalWrite(m2, HIGH);
			digitalWrite(m3, LOW);
			digitalWrite(m4, HIGH);
		}
	}			
}
