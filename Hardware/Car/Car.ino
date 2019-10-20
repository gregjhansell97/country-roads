//#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include <hive_map.hpp>

#include "messages.h"
#include "radio_channel.h"

// globals
hmap::Location vehicle(hmap::loc::ANY);
car::Msg car_status_msg;
RadioChannel radio_channel;

void on_car_msg(void* raw_msg){
    car::Msg* msg = static_cast<car::Msg*>(raw_msg);
    Serial.println(msg->car_id);
    Serial.println(msg->color);
    Serial.println(msg->heading);
    Serial.println(msg->gas_level);
    Serial.println(msg->speed); 
}

const byte address[6] = "00001";
void setup() {
  
    radio_channel.setup(address);
    Serial.begin(9600);
    
    //Setup the car characteristics
    car_status_msg.car_id = 22;//33;
    car_status_msg.color = BLUE;//GREEN;
    car_status_msg.heading = 270;
    car_status_msg.gas_level = 14;
    car_status_msg.speed = 35;

    //set up car location
    vehicle.subscribe<car::Msg>(&on_car_msg);
    vehicle.add_channel(radio_channel);
}

const unsigned int cycle_delay = 400;
const unsigned int publish_delay_ratio = 4;
unsigned int publish_count = 0;

void loop() {
    delay(cycle_delay);
    if((publish_count%publish_delay_ratio) == 0){
        vehicle.destinations(hmap::loc::ANY).publish(car_status_msg);
    }
    vehicle.cycle();
    ++publish_count;
}
