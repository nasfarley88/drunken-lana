#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>

class Window : public Gtk::Window
{
public:
  Window();
  virtual ~Window();

private:
  // Signal handlers:
  void on_button_quit();
  void on_button_numbered(const Glib::ustring& data);

  // Child widgets:
  Gtk::Grid m_grid;
  Gtk::Button m_button_1, m_button_2, m_button_quit;
};

#endif /* GTKMM_WINDOW_H */
