#include <hive_map.hpp>

#include "messages.h"
#include "radio_channel.h"

// globals
hmap::Location vehicle(hmap::loc::ANY);
car::Msg car_status_msg;
RadioChannel radio_channel;

void on_car_msg(void* raw_msg){
    car::Msg* msg = static_cast<car::Msg*>(raw_msg);
    Serial.println(msg->body.car_id);
    Serial.println(msg->body.color);
    Serial.println(msg->body.heading);
    Serial.println(msg->body.gas_level);
    Serial.println(msg->body.speed); 
}

const byte address[6] = "00001";
void setup() {
  
    radio_channel.setup(address);
    Serial.begin(9600);
    
    //Setup the car characteristics
    car_status_msg.body.car_id = 22;//33;
    car_status_msg.body.color = BLUE;//GREEN;
    car_status_msg.body.heading = 270;
    car_status_msg.body.gas_level = 14;
    car_status_msg.body.speed = 35;

    //set up car location
    vehicle.subscribe<car::Msg>(&on_car_msg);
    vehicle.add_channel(radio_channel);
}

const unsigned int cycle_delay = 250;
const unsigned int publish_delay_ratio = 4;
unsigned int publish_count = 0;

void loop() {
    delay(cycle_delay);

    // Read "data" from potentiometers
    car_status_msg.heading = analogRead(A0)/4;
    
    if((publish_count%publish_delay_ratio) == 0){
        vehicle.destinations(hmap::loc::ANY).publish(car_status_msg);
    }
    vehicle.cycle();
    ++publish_count;
}
