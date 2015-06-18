#include <SoftwareSerial.h>
#include <stdlib.h>

// Input pins
int tempPin = A0;	// Analog temperature sensor

// Output pins
int softTX = 5;	// TX pin for software serial
int softRX = 6;	// RX pin for software serial

// Phant server info
String phantPubKey = "YOURPUBLICKEY";
String phantPrivKey = "YOURPRIVATEKEY";
String phantField = "temp";
String serverIP = "54.86.132.254"; // data.sparkfun.com IP address
String serverPort = "80";

// Wifi settings
String ssid="\"YOURSSID\"";
String pwd="\"YOURPASSWORD\"";

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

	// Concatenate GET string
	String getStr = "GET /input/" + phantPubKey + "/?private_key=" + phantPrivKey + "&" + phantField + "=" + tempC + " HTTP/1.1\r\nHost: " + serverIP + "\r\nAccept: */*\"";

    // Get length of string
    String getStrLen = String(getStr.length());
        
    Serial.println(getStrLen);

	// Allow time to connect to Wifi Network
	ser.println("AT+CWJAP=" + ssid + "," + pwd);
	Serial.println("AT+CWJAP=" + ssid + "," + pwd);
	delay(5000);
	
	// Create connection to server
	ser.println("AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + serverPort);
    Serial.println("AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + serverPort);

	delay(500);
	
	delay(5000);
        
	// Send data using CIPSEND. Send string length followed by string contents
	ser.println("AT+CIPSEND=" + getStrLen);
	Serial.println("AT+CIPSEND=" + getStrLen);
	delay(1000);
    ser.println(getStr);
    Serial.println(getStr);
    delay(10000);

	// Close Connection
	ser.println("AT+CWQAP");
	Serial.println("AT+CWQAP");
	
	// Delay between updates
	delay(60000);
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
	float tempC = (((val*3.3/1024.0)-0.33) * 100);
	float tempF = tempC*9.0/5.0 + 32;

	// Convert values to strings
	char buf[16];
	String strTempC = dtostrf(tempC, 4, 1, buf); // value, width, precision, size
	String strTempF = dtostrf(tempF, 4, 1, buf);

	return strTempF;
}
