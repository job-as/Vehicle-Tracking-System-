#include <Adafruit_GPS.h>

#define gpsSerial Serial1

Adafruit_GPS GPS(&gpsSerial);

void setup() {
  
  
  
  gpsSerial.begin(9600);
  Serial.begin(9600);
  GPS.begin(9600);
  delay(500);

  Serial.println("Read GPS");

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); 
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

}


void loop() {

  readGPS();
  delay(10000);
  
}



void readGPS() {

  
  String h_m_s, d_m_y, location;
  String hour, minute, second;
  String deg_lat, deg_lon; 
  String gpsinfo;
  boolean flag;

  uint32_t timer = millis();

  flag = true;

  
  while(flag) {

    if (GPS.newNMEAreceived()) {
    
      if (!GPS.parse(GPS.lastNMEA())) 

        flag = true;

      else
        
        flag = false;
      
    }

  }

  if (timer > millis())  timer = millis();

  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer

    
    if(GPS.hour + 3 < 10) 
      hour = "0" + String(GPS.hour + 3); 

    else 
      hour = String(GPS.hour + 3);

    if(GPS.minute < 10)
      minute = "0" + GPS.minute;

    else
      minute = "0" + GPS.minute;

    if(GPS.seconds < 10)
      second = "0" + GPS.minute;

    else
      second = "0" + GPS.minute;

    h_m_s = hour + minute + second;

    d_m_y = String(GPS.day) + "/" + String(GPS.month) + "/" + "20" + String(GPS.year);

    //Serial.print("\nTime: ");
    //Serial.print(GPS.hour, DEC); Serial.print(':');
    //Serial.print(GPS.minute, DEC); Serial.print(':');
    //Serial.print(GPS.seconds, DEC); Serial.print('.');
    //Serial.println(GPS.milliseconds);
    //Serial.print("Date: ");
    //Serial.print(GPS.day, DEC); Serial.print('/');
    //Serial.print(GPS.month, DEC); Serial.print("/20");
    //Serial.println(GPS.year, DEC);
    //Serial.print("Fix: "); Serial.print((int)GPS.fix);
    //Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 

    if (GPS.fix) {


      deg_lat = convertTondegree(GPS.latitude);
      deg_lon = convertTondegree(GPS.longitude);

      location = deg_lat + String(GPS.lat) + "," + deg_log + String(GPS.lon);

      gpsinfo = "Time: " + h_m_s + " Date: " + d_m_y + " Location: " + location;

      Serial.println(gpsinfo);

      
      //Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      //Serial.print("Angle: "); Serial.println(GPS.angle);
      //Serial.print("Altitude: "); Serial.println(GPS.altitude);
      //Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

    }

  }

}

void convertTondegree(float loc) {

  int d = loc/100;
  float m = loc - d*100;

  String d_m = String(d) + " " + String(m,4);

  return d_m;

}