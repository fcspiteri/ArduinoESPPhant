#include <SoftwareSerial.h>
#include <stdlib.h>

// Input pins
int tempPin = A0;	// Analog temperature sensor

// Output pins
int softTX = 5;	// TX pin for software serial
int softRX = 6;	// RX pin for software serial

// Phant server info
String phantPubKey = "YOURPHANTPUBLICKEY";
String phantPrivKey = "YOURPHANTPRIVATEKEY";
String phantField = "YOURPHANTFIELD";
String serverIP = "54.86.132.254"; // data.sparkfun.com IP address
String serverPort = "80";

// Setup software serial
SoftwareSerial ser(softRX, softTX);

void setup(){
	// Initilize I/O pins
	pinMode(tempPin, INPUT);
	
	// Initialize arduino serial
	Serial.begin(9600);

	// Initialize software serial
	ser.begin(9600);
}


void loop(){

	// Measure temperature
	String tempC = measureTempF();
        Serial.println(tempC);

	// Concatenate GET string
	String getStr = "GET /input/" + phantPubKey + "/?private_key=" + phantPrivKey + "&" + phantField + "=" + tempC + " HTTP/1.1\\r\\nHost: " + serverIP + "\\r\\nAccept: */*\\r\\n\\r\\n";
	Serial.println(getStr);

	// Create connection to server
	ser.println("conn=net.createConnection(net.TCP, false)");
	Serial.println("conn=net.createConnection(net.TCP, false)");
	delay(500);
	ser.println("conn:on(\"receive\", function(conn, pl) print(pl) end)");
	Serial.println("conn:on(\"receive\", function(conn, pl) print(pl) end)");
        delay(500);
	ser.println("conn:connect(80,\"" + serverIP + "\")");
	Serial.println("conn:connect(80,\"" + serverIP + "\")");
	delay(5000);
        
	// Send data
	ser.println("conn:send(\"" + getStr + "\")");
	Serial.println("conn:send(\"" + getStr + "\")");

	// Delay between updates
	delay(15000);
}

String measureTempF(){
	float val = 0;
	// Take 10 measurements and average
	for (int i = 0; i < 10; i++){
		val += analogRead(tempPin);
		delay(500);
	}
	val /= 10.0;

	// Convert 10-bit temp reading to degrees
	float tempC = (((val*5.0/1024.0)-0.5) * 100);
	float tempF = tempC*9.0/5.0 + 32;

	// Convert values to strings
	char buf[16];
	String strTempC = dtostrf(tempC, 4, 1, buf); // value, width, precision, size
	String strTempF = dtostrf(tempF, 4, 1, buf);

	return strTempF;
}
