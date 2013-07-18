#include "pedal.h"
#include <gtkmm.h>
#include <iostream>

Pedal::Pedal() : thread(0), stop(false) {
  std::cout << "I made a Pedal" << std::endl;
}
 
// Called to start the processing on the thread
void Pedal::start () {
  std::cout << "Pedal started" << std::endl;
  thread = Glib::Thread::create(sigc::mem_fun(*this, &Pedal::run), true);
  std::cout << "Pedal start ended" << std::endl;
}
 
// When shutting down, we need to stop the thread
Pedal::~Pedal() {
  {
    Glib::Mutex::Lock lock (mutex);
    stop = true;
  }
  if (thread)
    thread->join(); // Here we block to truly wait for the thread to complete
}
 
// This is where the real work happens
void Pedal::run () {
    std::cout << "Thread write!" << std::endl;
  while(true) {
    {
      Glib::Mutex::Lock lock (mutex);
      if (stop) break;
    }
    sleep(5);
    std::cout << "Thread write!" << std::endl;
    sig_done();
    break;
  }
}

