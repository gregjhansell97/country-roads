//Basic car structure
//Needs to include Gerg's header from Hive Map
//Contains: ID, Color, Heading, GasLevel, speed

#ifndef CAR_MSG_H
#define CAR_MSG_H

#include <hive_map.hpp>
#include <message.h>


#define ORANGE 0
#define YELLOW 1
#define GREEN 2
#define BLUE 3



#define CAR_MSG 12 //10 or higher


struct CarMsg{
  hmap::msg::Header header {
      .type = CAR_MSG,
      .bcast_radius = 1,
      .destination = hmap::loc::ANY,
      .size = sizeof(CarMsg)
  };
  unsigned char carID;
  unsigned char color;
  unsigned int heading;
  unsigned char gasLevel; //Percentage
  unsigned char pace; //MPH
};


#endif //CAR_MSG_H
