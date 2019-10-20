#include <hive_map.hpp>

#include "locations.h"
#include "messages.h"
#include "radio_channel.h"

// globals
const unsigned int http_requester_id = HTTP_REQUESTER;
hmap::Location relay_station(http_requester_id);
RadioChannel radio_channel;

void on_relay_msg(void* raw_msg){
    relay::Msg& r_msg = *static_cast<relay::Msg*>(raw_msg);
    Serial.write((byte*)(&r_msg.body), sizeof(relay::Body));
}

const byte address[6] = "00001";
void setup() {
    radio_channel.setup(address);
    Serial.begin(9600);
    
    //set look up car location
    relay_station.subscribe<relay::Msg>(&on_relay_msg);
    relay_station.add_channel(radio_channel);
}

const unsigned int cycle_delay = 250;

void loop() {
    delay(cycle_delay);
    relay_station.cycle();
}
