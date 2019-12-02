#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include <iostream>
#include <vector>
#include <random>
#include <opencv2/opencv.hpp>

class particle
{
public:
  int x;
  int y;
  float likehood;
};

#endif
