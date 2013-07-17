#include "window.h"
#include "pedal.h"
#include <gtkmm/application.h>

#include <vlc/vlc.h>
using namespace std;

int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create
    (argc, argv, "org.gtkmm.example");

  Window window;

  // Shows the window and returns when it is closed.
  return app->run(window);

}
