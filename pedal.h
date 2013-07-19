#ifndef PEDAL_H
#define PEDAL_H
#include <gtkmm.h>
#include "hidapi.h"

class Pedal : public sigc::trackable
{
 public:
  Pedal(void *);
  ~Pedal();
  void start();
  Glib::Dispatcher sig_done;
  
 protected:
  hid_device *pedal;
  unsigned char pedal_buf[256];
  bool read_pedal(int x);
  const int read_pedal_timeout;
  void * mywindow;


  void run();
  
  // Multithread stuff
  Glib::Thread * thread;
  Glib::Mutex mutex;
  bool stop;
};

#endif
