// ====== BASE STATION ======
// ===== NODE - 00 =====

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

void updateDisp();
void updateStatus();

Adafruit_SSD1306 display(OLED_RESET);

RF24 radio(7, 8); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network
const uint16_t this_node = 00; // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01; // Address of the other nodes in Octal format
const uint16_t node02 = 02;
const uint16_t node03 = 03;





//===initial sensor values===//
int s1[2]={0,0};
int s2[2]={0,0};
int s3[2]={0,0};
String ok = "";
String watering_status = "";
void setup() {
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
Serial.begin(9600);
SPI.begin();
radio.begin();
network.begin(90, this_node); //(channel, node address)
radio.setDataRate(RF24_2MBPS);
}
void loop() {
network.update();
//===== Receiving =====//
while ( network.available() ) { // Is there any incoming data?
RF24NetworkHeader header;
int incomingData[4];
network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data





if(header.from_node == 01) {
Serial.println("FROM NODE 01");
Serial.println(incomingData[0]);
Serial.println(incomingData[1]);
s1[0] = incomingData[0];
s1[1] = incomingData[1];
//====values displaying to OLED====//
updateDisp();
}
if(header.from_node == 02) {
Serial.println("FROM NODE 02");
Serial.println(incomingData[0]);
Serial.println(incomingData[1]);
Serial.println("From NODE 03");
Serial.println(incomingData[2]);
Serial.println(incomingData[3]);
s2[0] = incomingData[0];
s2[1] = incomingData[1];
s3[0] = incomingData[2];
s3[1] = incomingData[3];
//====values displaying to OLED====//
updateDisp();
}
}





//===updating status variables of water level and moisture content===//
updateStatus();
}

void updateDisp() {
updateStatus();
display.clearDisplay();
display.setCursor(0,0);
display.print("status:");
display.print(watering_status);
display.println("");
display.print("Level:");
display.print(ok);
display.display();
delay(1000);
}
void updateStatus() {
//===updating status variables of water level and moisture content===//
if(s1[0] > 100 && s2[0] > 100 && s3[0] > 100) {
ok = "OK";
}
else{
ok = "Less";





}
if(ok=="OK"){
watering_status = "NRQ";
}
else if(s1[1] >= 80 && s2[1] >= 80 && s3[1] >= 80) {
watering_status = "REQ";
}
else{
watering_status = "NRQ";
}
}
