/*  Sending Module basic code
 * Defines what the strucures is, including HiveMap required header
 * Need to create channels for read and write functions
 * This code just test basic functionality of the devices
 */
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <hive_map.hpp>
#include <location_messages.h>
  

RF24 radio(7, 8);

struct CarInfo {
  hmap::msg::Header header {
    .type = HOPS_MSG,
    .bcast_radius = 1, // can go one hop with-out it dieing
    .destination = hmap::loc::ANY,
    .size = sizeof(CarInfo)
  };
  unsigned char speeds;
  unsigned char heading;
  double gasLevel;
  unsigned char color;  
};

#define BLUE 0
#define RED 1

CarInfo m;
//hmap::HopsMsg m;

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  m.header.type = 34;
  m.speeds = 200;
  m.gasLevel = 0.4;
  m.color = RED; // red
}
void loop() {
  Serial.println("sending");
  radio.write(&m, sizeof(m));
  delay(500);
}
