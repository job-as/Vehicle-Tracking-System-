#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(7,8);

#define gsmSerial Serial3 //or Serial

#define gpsSerial Serial1

void setup() {

	gsmSerial.begin(9600);
	Serial.begin(9600);
	delay(500);

	gsm_setup();
	delay(500);
	
}

void loop() {

	readSMS();
	delay(5000);
	
}

void gsm_setup() {

	gsmSerial.println("AT");
	delay(1000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CMGF=1");
	delay(1000);
	readGSM();
	delay(1000);

	gsmSerial.println("AT+CNMI=0,0,0,0,0");
	delay(1000);
	readGSM();
	delay(1000);
}

void gps_setup() {

	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	GPS.sendCommand(PGCMD_ANTENNA);

}

void readGSM() {

	if(gsmSerial.available()) {

		while(gsmSerial.available()) {

			Serial.print((char)gsmSerial.read());
		    
		}

	}

}


void readSMS() {

	char inChar[256];
	String inString;
	String cell,dat;
	int count;

	inString.reserve(256);

	gsmSerial.println("AT+CMGR=1");
	delay(3000);


	if(gsmSerial.available()) {

		inString = "";
		count = 0;

    	while(gsmSerial.available()) {

    		inChar[count] = gsmSerial.read();
		    inString.concat(inChar[count]);
		    count++;	

	    }

	    Serial.println(inString);

	    if(inString.indexOf("+CMGR:") != -1) {

	    	cell=parsestr(inChar,3);
	    	dat=parsestr(inChar,5);
	    	dat=dat.substring(0,dat.indexOf(","));

	    	Serial.print("Number: ");
	    	Serial.println(cell);
	    	Serial.print("Date: ");
	    	Serial.println(dat);
	    	cell.trim();
	    	dat.trim();

	    	if(cell.equals("+251932052842")) {

	    		sendsms(cell);
	    	    
	    	}

	    	else {

	    		Serial.println("Wrong Number");
	    		delay(3000);

	    		gsmSerial.println("AT+CMGD=1");
	    		delay(3000);
	    		readGSM();
	    		delay(1000);
	    		
	    	}

        
	    }

		for(int i=0; i< count; i++)  {

			   inChar[i] = NULL;

		}

	}

}


String parsestr(char inputString[], byte pos) {

	char *strings[10];
	char *ptr = NULL; 

	byte index = 0;
    ptr = strtok(inputString, "\""); 
   
    while(ptr != NULL) {
 
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, "\"");

    }

    return strings[pos];

}

void sendsms(String Number) {

	String gpsData = readGPS();
	Serial.println(gpsData);

	String Time = gpsData.substring(0, gpsData.indexOf("Date:"));
	String Date = gpsData.substring(gpsData.indexOf("Date:"), gpsData.indexOf("Coordinate:"));
	String Coor = gpsData.substring(gpsData.indexOf("nate:") + 6);


	gsmSerial.println("AT+CMGS=" + Number);
    delay(5000);
    readGSM();

    gsmSerial.println(Time);
    delay(100);
    gsmSerial.println(Date);
    delay(100);

	gsmSerial.print("http://maps.google.com/maps?q=loc:");   //sending gps data
	delay(100);
	
	gsmSerial.println(Coor);
    delay(1000);
	gsmSerial.println((char)26);
	readGSM();
	delay(5000);

	gsmSerial.println("AT+CMGD=1");
	delay(3000);
	readGSM();
	delay(1000);

	
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

