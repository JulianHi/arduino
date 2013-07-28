#include "JsonSerial.h"
#include "NinjaPacket.h"
#include "PortManager.h"
#include "Port.h"
#include "NinjaLED.h"
#include "OnBoardManager.h"
#include "Heartbeat.h"
#include "RFPacket.h"

#include "CommonProtocolEncoder.h"
#include "CommonProtocolDecoder.h"

OnBoardManager	onBoardManager;
PortManager		 portManager;
NinjaLED				leds;
volatile unsigned int	cycleCount;

// function prototypes
static void handleSerialData(void);
//

int freeRam () //TODO: rename this
{
	extern int __heap_start, *__brkval;
	int v;

	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
	delay(2000);

	jsonSerial.setup(9600); //TODO: 57600 baudrate once dynamic baud detection is implemented in the client
	leds.setup();
	leds.timerSetup();
	onBoardManager.setup();
}

void loop() {
	handleSerialData();
	heartbeat.check(); //TODO: rename this... what does this do...?????

	// 2. Check hardware ports for changes
	portManager.check();

	// 3. Check onboard components for incoming data
	onBoardManager.check();

	heartbeat.resume();
}

static void handleSerialData(void) {
	static NinjaPacket ninjaPacket; //TODO: determine if this need to be static or not.

	if (jsonSerial.read(&ninjaPacket)) 	{
		if (ninjaPacket.getGuid() > 0) {
			portManager.handle(&ninjaPacket);
		} else {
			onBoardManager.handle(&ninjaPacket);
		}

		/*Serial.print("G=");
		Serial.println(ninjaPacket.getGuid());
		Serial.print("D=");
		Serial.println(ninjaPacket.getDevice());
		Serial.print("DA=");
		Serial.println(ninjaPacket.getData());
		Serial.print("Ram=");
		Serial.println(freeRam());*/
	}
}

ISR(TIMER1_OVF_vect)				// interrupt service routine
{ 
	TCNT1 = 63536;	 // preload timer
	cycleCount++;

	if (cycleCount == leds.m_nDutyCycle) {
		leds.statOff();
	}

	if (cycleCount == leds.m_nPeriod) {
		leds.statOn();
		cycleCount = 0;
	}
}
