#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() { 
    std::cout << "INITIALIZED TRAFFIC LIGHT" << std::endl; 
    _currentPhase = TrafficLightPhase::red; }

/* 
void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

*/

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate() {
  // FP.2b : Finally, the private method „cycleThroughPhases“ should be started
  // in a thread when the public method „simulate“ is called. To do this, use
  // the thread queue in the base class.
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));

}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
  // FP.2a : Implement the function with an infinite loop that measures the time
  // between two loop cycles and toggles the current phase of the traffic light
  // between red and green and sends an update method to the message queue using
  // move semantics. The cycle duration should be a random value between 4 and 6
  // seconds. Also, the while-loop should use std::this_thread::sleep_for to
  // wait 1ms between two cycles.
  
  // Similar to Vehicle::drive() method

  long cycleDuration; 
  TrafficLightPhase lightPhase = this->getCurrentPhase();  
  std::chrono::time_point<std::chrono::system_clock> lastUpdate; 
  lastUpdate = std::chrono::system_clock::now();

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(4000, 7000);
  cycleDuration = distr(eng); 
  
  std::cout << "Cycle duration Traffic Light: " << cycleDuration << std::endl; 

  while (true) {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
    // std::cout << "Time since last update: " << timeSinceLastUpdate << std::endl; 
    // std::cout << "Cycle duration: " << cycleDuration << std::endl; 
    if (timeSinceLastUpdate >= cycleDuration ){
      lightPhase = (TrafficLightPhase::red == this->getCurrentPhase())
                       ? TrafficLightPhase::green
                       : TrafficLightPhase::red;

      this->_currentPhase = lightPhase;

      std::cout << "Changed traffic light to: "
                << (this->getCurrentPhase() == TrafficLightPhase::red ? "red"
                                                                      : "green")
                << std::endl;

      // Get new random cycleDuration time and lastUpdate time for new phase change
      cycleDuration = distr(eng);
      lastUpdate = std::chrono::system_clock::now();
    }
  }
}
