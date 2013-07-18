#ifndef PEDAL_H
#define PEDAL_H
#include <gtkmm.h>
#include "hidapi.h"

class Pedal {
 public:
  Pedal();
  ~Pedal();
  void start();
  Glib::Dispatcher sig_done;
  
 protected:
  void run();
  
  // Multithread stuff
  Glib::Thread * thread;
  Glib::Mutex mutex;
  bool stop;
};

#endif
