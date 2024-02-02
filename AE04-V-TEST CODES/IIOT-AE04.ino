
/*
 * 
 * 
 * RTC Check
 * micro SD Card Check CS : 15
 * Ethernet Check      CS : 5
 * RS485
 * SIM800C
 * All Output Turn ON Series
 * All input status serial print
 * 
  Turns ON All Outputs in series
  Serial prints all the input status
  SIM800C External Antenna Test
  
 *   
 * 
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "FS.h"
#include "SD.h"
#include "RTClib.h"

#define ANALOG_PIN_0 32

#define INPUT1 12
#define INPUT2 39
#define INPUT3 34
#define INPUT4 35
#define INPUT5 13
#define INPUT6 21
#define INPUT7 22
#define INPUT8 15

#define OUTPUT1 26
#define OUTPUT2 27

#define RS485_RX 33
#define RS485_TX 2
#define RS485_FC 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;





RTC_DS3231 rtc; 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

unsigned int localPort = 8888;       // local port to listen for UDP packets

const char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

EthernetUDP Udp;// A UDP instance to let us send and receive packets over UDP




int analog_value = 0;

  
int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);

 
  return analog_value                                                                                                ; //Read analog
}



unsigned long int timer1 = 0;

// ================================================ SETUP ================================================
void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello");
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 

  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);   // RS-485 
  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);

  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);

  
  Wire.begin(16,17);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }

  if (!ads2.begin(0x49)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }

  RTC_Check();
  delay(1000);
  SD_CHECK();
  delay(1000);
  
  
  adcAttachPin(32);


  digitalWrite(RS485_FC, HIGH);   // RS-485 


  //
  
}






void loop() {

  int16_t adc0, adc1, adc2, adc3;
 
 
  Serial.print(digitalRead(INPUT1));Serial.print(digitalRead(INPUT2));Serial.print(digitalRead(INPUT3));Serial.println(digitalRead(INPUT4));
  Serial.print(digitalRead(INPUT5));Serial.print(digitalRead(INPUT6));Serial.print(digitalRead(INPUT7));Serial.println(digitalRead(INPUT8));
  Serial.println(""); 

  Serial.println(""); 
  Serial.print("Push button  ");Serial.println(readSwitch());
  Serial.println(""); 

  adc0 = ads1.readADC_SingleEnded(0);
  adc1 = ads1.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(2);
  adc3 = ads1.readADC_SingleEnded(3);

  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN1: "); Serial.print(adc0); Serial.println("  ");
  Serial.print("AIN2: "); Serial.print(adc1); Serial.println("  ");
  Serial.print("AIN3: "); Serial.print(adc2); Serial.println("  ");
  Serial.print("AIN4: "); Serial.print(adc3); Serial.println("  ");

  adc0 = ads2.readADC_SingleEnded(0);
  adc1 = ads2.readADC_SingleEnded(1);
  adc2 = ads2.readADC_SingleEnded(2);
  adc3 = ads2.readADC_SingleEnded(3);

  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN4: "); Serial.print(adc0); Serial.println("  ");
  Serial.print("AIN5: "); Serial.print(adc1); Serial.println("  ");
  Serial.print("AIN6: "); Serial.print(adc2); Serial.println("  ");
  Serial.print("AIN7: "); Serial.print(adc3); Serial.println("  ");




  
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(500);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  delay(500);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  delay(500);
  
  
  Serial1.println("Hello RS-485");
   
 //Serial2.println("AT");
 delay(1000);
 
 
}


void displayTime(void) {
  DateTime now = rtc.now();
     
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(1000);

}

void RTC_Check(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
 else{
 if (rtc.lostPower()) {
  
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }

   
  int a=1;
  while(a<6)
  {
  displayTime();   // printing time function for oled
  a=a+1;
  }
 }
}



void SD_CHECK(){
  uint8_t cardType = SD.cardType();
  
    if(SD.begin(5))
 {
  Serial.println("Card Mount: success");
  Serial.print("Card Type: ");

    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("Unknown");
    }

  int cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("Card Size: %lluMB\n", cardSize);

  }

  if(!SD.begin(5))
  {
  Serial.println("NO SD card");            
  }

}

void ETHERNET_CHECK(){
  Ethernet.init(26);  // ESP32 with Adafruit Featherwing Ethernet

  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");

    // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
  Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
   Serial.println("Ethernet cable is not connected.");
  }
  }
  
 else{
  Udp.begin(localPort);
 
  sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
   Serial.print("The UTC time is ");
   Serial.print((epoch  % 86400L) / 3600);
    // print the hour (86400 equals secs per day)
   Serial.print(':');
   if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
   }
    
   Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
   Serial.print(':');
   if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
   }
   
   Serial.println(epoch % 60); // print the second
   }
  // wait ten seconds before asking for the time again
  delay(3000);
  Ethernet.maintain();
 }
}


void sendNTPpacket(const char * address) 
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket(); 
}
