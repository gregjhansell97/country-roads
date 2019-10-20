#ifndef COUNTRY_ROADS_HARDWARE_MESSAGES_H_
#define COUNTRY_ROADS_HARDWARE_MESSAGES_H_
//Basic car structure
//Needs to include Gerg's header from Hive Map
//Contains: ID, Color, Heading, GasLevel, speed

#include <hive_map.hpp>
#include <message.h>


#define ORANGE 0
#define YELLOW 1
#define GREEN 2
#define BLUE 3


// message constants
#define CAR_MSG 12 //10 or higher
#define RELAY_STATION_MSG 24


namespace car {

struct Body {
  unsigned char car_id;
  unsigned char color;
  unsigned char heading;    // 0 is N 255 is N
  unsigned char gas_level; //scaled 0 to 255
  unsigned char speed; //MPH
};

struct Msg{
  hmap::msg::Header header {
      .type = CAR_MSG,
      .bcast_radius = 1,
      .destination = hmap::loc::ANY,
      .size = sizeof(Msg)
  };
  Body body;
};

}


namespace relay {
struct Body {
    unsigned char location;
    car::Body car;
};
struct Msg {
    hmap::msg::Header header {
        .type = RELAY_STATION_MSG,
        .bcast_radius = 1,
        .destination = hmap::loc::ANY,
        .size = sizeof(Msg)
    };
    Body body;
};

}
#endif //COUNTRY_ROADS_HARDWARE_MESSAGES_H_
