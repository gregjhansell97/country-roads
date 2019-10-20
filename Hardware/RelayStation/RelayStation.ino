#include <hive_map.hpp>

#include "locations.h"
#include "messages.h"
#include "radio_channel.h"

// globals
const unsigned int relay_id = RELAY_1;
hmap::Location relay_station(relay_id);
relay::Msg car_msg;
RadioChannel radio_channel;

void on_car_msg(void* raw_msg){
    car::Msg& c_msg = *static_cast<car::Msg*>(raw_msg);
    Serial.println(c_msg.body.car_id);
    Serial.println("publishing message");
    relay::Msg r_msg;
    r_msg.body.car = c_msg.body;
    r_msg.body.location = relay_id;
    relay_station.destinations(HTTP_REQUESTER).publish(r_msg);
}

const byte address[6] = "00001";
void setup() {
    radio_channel.setup(address);
    Serial.begin(9600);
    
    //set look up car location
    relay_station.subscribe<car::Msg>(&on_car_msg);
    relay_station.add_channel(radio_channel);
}

const unsigned int cycle_delay = 250;
void loop() {
    delay(cycle_delay);
    relay_station.cycle();
}
