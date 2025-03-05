#ifndef __ROAD_H__
#define __ROAD_H__

#define ROAD_CROSS -30000
#define ROAD_TB 30000
#define ROAD_TL -25000
#define ROAD_TR 25000
#define ROAD_LEFT -20000
#define ROAD_RIGHT 20000

// typedef enum Road {    // L F R
//   CrossRoad = 0b111,   // 1 1 1
//   TBRoad = 0b101,      // 1 0 1
//   TLRoad = 0b110,      // 1 1 0
//   TRRoad = 0b011,      // 0 1 1
//   LeftRoad = 0b100,    // 1 0 0
//   RightRoad = 0b001,   // 0 0 1
//   Straight = 0b010,    // 0 1 0
//   UnknowRoad = 0b000,  // 0 0 0
// } Road;

typedef enum Road {    // L F R
  CrossRoad = 0b111,   // 1 1 1
  TBRoad = 0b101,      // 1 0 1
  TLRoad = 0b011,      // 1 1 0
  TRRoad = 0b110,      // 0 1 1
  LeftRoad = 0b001,    // 1 0 0
  RightRoad = 0b100,   // 0 0 1
  Straight = 0b010,    // 0 1 0
  UnknowRoad = 0b000,  // 0 0 0
} Road;

#endif
