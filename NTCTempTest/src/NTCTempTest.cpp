/* 
 * NTC Temperature Sensor Test
 * Author: Baoshi Sun
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "xlxLogger.h"
#include "xlxSenNTC.h"

// Wait Console input to continue system starting,
/// use this option to observe starting procedure
#define SYS_SERIAL_DEBUG

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

xlSenNTCClass senNTCTemp;

// setup() runs once, when the device is first turned on
void setup() {
  // Open Serial Port
	TheSerial.begin(SERIALPORT_SPEED_DEFAULT);

#ifdef SYS_SERIAL_DEBUG
	// Wait Serial connection so that we can see the starting information
	while(!TheSerial.available()) {
		if( Particle.connected() == true ) { Particle.process(); }
	}
#endif  
  // Put initialization like pinMode and begin functions here
  LOGN(LOGTAG_MSG, F("NTC Temperature Sensor demo starting...SysID=%s Version:%s"), System.deviceID().c_str(), System.version().c_str());

  // Init NTC sensor
  senNTCTemp.InitPins();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");

  senNTCTemp.ReadTemp();
  delay( 5 * 1000 );
}
