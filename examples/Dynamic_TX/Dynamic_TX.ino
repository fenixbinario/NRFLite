/*

Demonstrates sending data packets of different length.  The receiver will check to see what size of
packet was received and act accordingly.

Radio -> Arduino

CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection in this example
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID             = 1; // Our radio's id.  Can be any 8-bit number (0-255).
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE         = 9;
const static uint8_t PIN_RADIO_CSN        = 10;

struct RadioPacket1
{
	uint8_t FromRadioId; // We'll include our radio's Id so the receiver knows who sent the packet.
	uint8_t Counter;	 
};   

struct RadioPacket2
{
	uint8_t FromRadioId;
	char Message[31];    // Note the max packet size is 32, so 31 is all we can use here.
};

NRFLite _radio;
RadioPacket1 _radioData1;
RadioPacket2 _radioData2;

void setup()
{
	Serial.begin(115200);
	
	if (!_radio.init(RADIO_ID, RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
		Serial.println("Cannot communicate with radio");
		while (1) {} // Wait here forever.
	}
	
    // Assign our radio's id to the 2 packets we'll be sending.
	_radioData1.FromRadioId = RADIO_ID;
	_radioData2.FromRadioId = RADIO_ID;
}

void loop()
{
	// Pick a number from 10,000 - 60,000.
	uint16_t randomNumber = random(10000, 60001); 
	
	// Send either RadioPacket1 or RadioPacket2.
	if (randomNumber > 30000 )
    {
		_radioData1.Counter++;
		
		Serial.print("Sending ");
		Serial.print(_radioData1.Counter);
		
		if (_radio.send(DESTINATION_RADIO_ID, &_radioData1, sizeof(_radioData1)))
        {
			Serial.println("...Success");
		}
		else
        {
			Serial.println("...Failed");
		}
	}
    else
    {
        // Create a message and assign it to the packet we'll be sending.
        // Strings need to be converted to a char array.
		// Convert the string to a char array, writing the array to _radioData2.Message.
		// Note the string cannot be longer than 31 characters since that is the size of _radioData2.Message.
		String msg = "Hello " + String(randomNumber);
		msg.toCharArray(_radioData2.Message, msg.length() + 1);
		Serial.print("Sending '");
		Serial.print(msg);
        Serial.print("'";
		
		if (_radio.send(DESTINATION_RADIO_ID, &_radioData2, sizeof(_radioData2)))
        {
			Serial.println("...Success");
		}
		else
        {
			Serial.println("...Failed");
		}
	}
	
	delay(1000);
}