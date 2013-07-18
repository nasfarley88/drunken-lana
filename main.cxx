#include "window.h"
#include <gtkmm/application.h>
#include "hidapi.h"
#include <iostream>
#include <vlc/vlc.h>
using namespace std;

int main(int argc, char *argv[])
{

  // Gheck if we're ready to support threads, and if not, make it ready.
  if(!Glib::thread_supported()) Glib::thread_init();
  
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create
    (argc, argv, "org.gtkmm.example");

  Window window;


  // Shows the window and returns when it is closed.
  return app->run(window);

}
