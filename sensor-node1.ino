// ===== NODE - 01 =====
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(7, 8); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network
const uint16_t this_node = 01; // Address of our node in Octal format ( 04,031, etc)
const uint16_t base_node = 00;
int cnt = 0;
void setup() {
Serial.begin(9600);





SPI.begin();
radio.begin();
network.begin(90, this_node); //(channel, node address)
pinMode(A0,INPUT);
pinMode(A1,INPUT);
radio.setDataRate(RF24_2MBPS);
//Watchdog timer setup
WDTCSR = (24);
WDTCSR = (33);
WDTCSR |= (1<<6);
//ENABLE SLEEP
SMCR |= (1 << 2);
SMCR |= 1;
}
void loop() {
network.update();
int values[4];
int levelSensor = 1;
levelSensor = analogRead(A0);
int moistureSensor = 2;
moistureSensor = analogRead(A1);
int percentage = map(moistureSensor, 683, 315, 0, 100);
values[0] = levelSensor;
values[1] = percentage;





RF24NetworkHeader header(base_node); // (Address where the data is going)
bool ok = network.write(header, &values, sizeof(values)); // Send the data
cnt++;
Serial.println(cnt);
delay(3000);
//===SLEEP===//
if( cnt > 40){ // Sleep after data is sent for 2 minutes
cnt=0;
for(int i = 0; i < 60; i++){ // Sleep for 8 mins i.e 8secs*60 = 480secs
MCUCR |= (3 << 5);
MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); // Disabling brown out detector before sleep
__asm__ __volatile__("sleep");
}
}
}
ISR(WDT_vect){}
