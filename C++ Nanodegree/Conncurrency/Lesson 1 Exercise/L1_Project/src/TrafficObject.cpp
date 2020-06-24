#include <iostream>
#include <chrono>
#include "TrafficObject.h"

// init static variable
int TrafficObject::_idCnt = 0;

void TrafficObject::setPosition(double x, double y)
{
    _posX = x;
    _posY = y;
}

void TrafficObject::getPosition(double &x, double &y)
{
    x = _posX;
    y = _posY;
}

TrafficObject::TrafficObject()
{
    _type = ObjectType::noObject;
    _id = _idCnt++;
}

TrafficObject::~TrafficObject()
{
    // Task L1.1 : Set up a thread barrier that ensures that all the thread objects in the member vector _threads are joined.
  std::vector<std::thread> _threads; // holds all threads that have been launched within this object

  for(auto &thred_m : _threads) {
    
    thred_m.join(); 
  }
}
