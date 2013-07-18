#include "pedal.h"
#include <gtkmm.h>
#include <iostream

class Pedal {
  public:
 
    Pedal() : thread(0), stop(false) {}
 
    // Called to start the processing on the thread
    void start () {
      thread = Glib::Thread::create(sigc::mem_fun(*this, &Pedal::run), true);
    }
 
    // When shutting down, we need to stop the thread
    ~Pedal() {
      {
        Glib::Mutex::Lock lock (mutex);
        stop = true;
      }
      if (thread)
        thread->join(); // Here we block to truly wait for the thread to complete
    }
 
    Glib::Dispatcher sig_done;
 
  protected:
    // This is where the real work happens
    void run () {
 
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
 
    Glib::Thread * thread;
    Glib::Mutex mutex;
    bool stop;
};
