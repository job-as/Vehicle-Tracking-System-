#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(7,8);

#define gsmSerial Serial3

#define gpsSerial Serial1

Adafruit_GPS GPS(&gpsSerial);

void setup() {

	gsmSerial.begin(9600);
	gpsSerial.begin(9600);
	Serial.begin(9600);
	delay(500);

	gsm_setup();
	delay(500);

	gps_setup();
	delay(500);

}

void loop() {

	String gpsData=readGPS();
	delay(1000);

	sendingToserver(gpsData);
	delay(1000);

	readSMS();
	delay(5000);

}

void gsm_setup() {

	gsmSerial.println("AT");
	delay(1000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CIPSHUT");
	delay(1000);
	readGSM();
	delay(1000);

}

void gps_setup() {

	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	GPS.sendCommand(PGCMD_ANTENNA);

}

void sendingToserver(String gpsData) {

	gsmSerial.println("AT+CIPMUX=0");
	delay(4000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CGATT=1");
	delay(4000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CSTT=\"et.com\", \"\", \"\"");
	delay(4000);
	readGSM();
	delay(1000);


	gsmSerial.println("AT+CIICR");
	delay(4000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CIFSR");
	delay(4000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CIPSTART=\"TCP\",\"tcp://0.tcp.ngrok.io\",13429");
	delay(3000);
	readGSM();
	delay(3000);

	gsmSerial.println("AT+CIPSEND");
	delay(2000);
	readGSM();
	delay(2000);

	sendData(gpsData);

	gsmSerial.println("AT+CIPCLOSE");
	delay(4000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CIPSHUT");
	delay(4000);
	readGSM();
	delay(1000);

}

void sendData(String gpsdata) {

	gsmSerial.println(gpsdata);
	gsmSerial.println((char)26);
	delay(3000);

}

void readGSM() {

	if(gsmSerial.available()) {

		while(gsmSerial.available()) {

			Serial.print((char)gsmSerial.read());
		    
		}

	}

}

String readGPS() {

	String times, date, coordinate, gpsinfo;
	String hour, minute, seconds;
	bool flag=true;

	while(flag) {

		char c=GPS.read();

	  	if (GPS.newNMEAreceived()) {

	  		if(!GPS.parse(GPS.lastNMEA()))

	  			flag = true;

	  		else

	  			flag = false;

	    }

	}

    if(GPS.hour<10)

    	hour="0"+String(GPS.hour);

    else

    	hour=String(GPS.hour);

    if(GPS.minute<10)

    	minute="0"+String(GPS.minute);

    else
    	minute=String(GPS.minute);

    if(GPS.seconds<10)

    	seconds="0"+String(GPS.seconds);

    else

    	seconds=String(GPS.seconds);

    String deg_lat = convertTondegree(GPS.latitude);
    String deg_log = convertTondegree(GPS.longitude);
    
    times = hour + ":" + minute + ":" +seconds;
    date = String(GPS.day) + "/" + String(GPS.month) + "/20" + String(GPS.year);
    coordinate = deg_lat + String(GPS.lat) + "," + deg_log + String(GPS.lon);
    gpsinfo = "Time: " + times + " Date: " + date + " Coordinate: " + coordinate;

    return gpsinfo;

}

void convertTondegree(float loc) {

	int d = loc/100;
	float m = loc - d*100;

	String d_m = String(d) + " " + String(m,4);

	return d_m;

}