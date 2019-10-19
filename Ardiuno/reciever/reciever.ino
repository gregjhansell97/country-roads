
/* Recieving module basic code
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
    .id = HOPS_MSG_ID,
    .bcast_radius = 1, // can go one hop with-out it dieing
    .destination = hmap::loc::ANY
  };
  unsigned char speeds;
  unsigned char heading;
  double gasLevel;
  unsigned char color;  
};

#define BLUE 0
#define RED 1


const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    Serial.println("Recieved...");
    //hmap::HopsMsg msg;
    CarInfo msg;
    radio.read(&msg, sizeof(msg));
    Serial.println(msg.header.id);
    Serial.println(msg.speeds);
    Serial.println(msg.gasLevel);
    Serial.println(msg.color);
  }
}
