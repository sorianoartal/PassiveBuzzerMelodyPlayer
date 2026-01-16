#include "Timer/Delay.h"
#include <Arduino.h>

/**Constructor: we pass the  target time delay*/
Delay::Delay(unsigned long delayTime) : 
_delayTime(delayTime),
_previousTime(0),
_disarm(false)
{}

/** set the target time delay and start counting */
void Delay::init(){
  this->_disarm = false;
  this->_delayTime = _delayTime;
  this->_previousTime = micros();
}

void Delay::init(unsigned long delayTime){
  this->_disarm = false;
  this->_delayTime = delayTime;
  _previousTime = micros();
}


/** Calculate if the delay time has elapsed*/
bool Delay::isDelayTimeElapsed(){ 
  unsigned long now = micros();
  
  // If the timer has not been stop 
  if(!_disarm)
  {
    if(now - _previousTime >= _delayTime) 
    {
      restartTimer() ; 
      return true;
    }
    else
    {
      return false;
    }
  }

  // If we are here is because the timer is stop
  return false;
 
}

/**
 * @brief Stop the delay timer
 * @details
 * This function stops the delay time tracking by by setting the _disarm
 * flag to true so the delay won't fired 
 */
void Delay::stopDelay(){
  _disarm = true;
}

/** when the time delay has elapse we update the time counter*/
void Delay::restartTimer(){
  this->_previousTime = micros();
}

/** Set new Delay Value for the Class*/
void Delay::updateDelayTime(unsigned long newDelayTime){
  this->_delayTime = newDelayTime;
}