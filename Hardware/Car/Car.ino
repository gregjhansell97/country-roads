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
int heading_tolerance = 15;
int speed_tolerance = 25;
int led_pin = 8;

void on_car_msg(void* raw_msg){
    car::Msg* msg = static_cast<car::Msg*>(raw_msg);
    Serial.println(msg->car_id);
    Serial.println(msg->color);
    Serial.println(msg->heading);
    Serial.println(msg->gas_level);
    Serial.println(msg->speed); 

    // if two cars are going the same way and have different speeds
    int hdiff = msg->heading - car_status_msg.heading;
    int sdiff = msg->speed - car_status_msg.speed;
    if (heading_tolerance>=hdiff && heading_tolerance*(-1)<=hdiff &&
        speed_tolerance<=sdiff && speed_tolerance*(-1)>=sdiff){
        // turn on an LED
        digitalWrite(led_pin, HIGH);
    }
}

const byte address[6] = "00001";
void setup() {
  
    radio_channel.setup(address);
    Serial.begin(9600);

    // Setup I/O pins
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
    
    //Setup the car characteristics
    car_status_msg.car_id = 22;//33;
    car_status_msg.color = BLUE;//GREEN;
    car_status_msg.heading = analogRead(A0)/4;
    car_status_msg.gas_level =  analogRead(A1)/4;
    car_status_msg.speed =  analogRead(A2)/8;

    //set up car location
    vehicle.subscribe<car::Msg>(&on_car_msg);
    vehicle.add_channel(radio_channel);
}

const unsigned int cycle_delay = 400;
const unsigned int publish_delay_ratio = 4;
unsigned int publish_count = 0;

void loop() {
    delay(cycle_delay);

    // Read "data" from potentiometers
    car_status_msg.heading = analogRead(A0)/4;
    car_status_msg.gas_level =  analogRead(A1)/4;
    car_status_msg.speed =  analogRead(A2)/8;
    
    if((publish_count%publish_delay_ratio) == 0){
        vehicle.destinations(hmap::loc::ANY).publish(car_status_msg);
    }
    vehicle.cycle();
    ++publish_count;

    if (publish_count%8==0){
        // Periodically turn off the LED
        digitalWrite(led_pin,LOW);
    }
}
