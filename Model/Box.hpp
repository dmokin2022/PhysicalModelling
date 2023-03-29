#pragma once

#include "PhysicalValues.hpp"

class Box {
public:
  physvalue top    = -10;
  physvalue bottom = 20;
  physvalue left   = -10;
  physvalue right  = 40;

  Box();
};
