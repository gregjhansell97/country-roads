#include <hive_map.hpp>

#include "messages.h"
#include "radio_channel.h"

// locations
#define RELAY_1 11
#define RELAY_2 22
#define RELAY_3 33

// globals
const unsigned int relay_id = RELAY_1;
hmap::Location relay_station(relay_id);
relay::Msg car_msg;
RadioChannel radio_channel;

void on_car_msg(void* raw_msg){
    car::Msg* c_msg = static_cast<car::Msg*>(raw_msg);
    relay::Msg r_msg;
    r_msg.car = c_msg;
    r_msg.relay = relay_id; 
    relay_station.destination(HTTP_REQUESTER).publish(relay_id);
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
