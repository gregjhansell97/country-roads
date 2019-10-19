#include "c_message.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class RadioChannel: public hmap::Channel {
public:
    RadioChannel():
            m_radio(7, 8){ }
    void setup(const byte address[6]) {
        m_radio.begin();
        m_radio.openReadingPipe(1,address);
        m_radio.openWritingPipe(address);
        m_radio.startListening();
        m_radio.setPALevel(RF24_PA_MIN);
    }
    
    unsigned long random() { return 5; }
    void send_data(char* data, size_t len) override{
        m_radio.stopListening();
        m_radio.write(data, len);
        m_radio.startListening();
    }
    size_t recv_data(char* data, size_t len) override{
        if(m_radio.available()){
            m_radio.read(data,len);
            return len;
        }
        return 0;
    }
private:
    RF24 m_radio; 
};


hmap::Location car(hmap::loc::ANY);


CarMsg c;
RadioChannel rc;

void on_car_msg(void* raw_msg){
    CarMsg* msg = static_cast<CarMsg*>(raw_msg);
    Serial.println("Other car detected");
}

void setup() {
    rc.setup("00001");
    Serial.begin(9600);
    
    //Setup the car characteristics
    c.carID = 1;
    c.color = BLUE;
    c.heading = 270;
    c.gasLevel = 14;
    c.pace = 35;

    //Setup channel
    car.subscribe<CarMsg>(&on_car_msg);
    car.add_channel(rc);

  
}

byte publish_flag = 0;

void loop() {
    delay(250);
    if((publish_flag%4) == 0){
        hmap::Destination& any = car.destinations(hmap::loc::ANY);
        any.publish(c);
    }
    car.cycle();
    publish_flag++;
}
