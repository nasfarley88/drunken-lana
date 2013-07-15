#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>

class Window : public Gtk::Window
{
public:
  Window();
  virtual ~Window();

private:
  //Signal handlers:
  void on_menu_file_new_generic();
  void on_menu_file_quit();
  void on_menu_others();

  void on_menu_choices_one();
  void on_menu_choices_two();


  // Signal handlers:
  void on_button_quit();
  void on_button_numbered(const Glib::ustring& data);

  // Child widgets:
  Gtk::Grid m_grid;
  Gtk::Button m_skip_to_start, m_rewind, m_play, m_pause, m_stop, m_fast_forward, m_skip_to_end, m_button_quit;

  //Child widgets for menu:
  Gtk::Box m_Box;

  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
  Glib::RefPtr<Gtk::RadioAction> m_refChoiceOne, m_refChoiceTwo;

};

#endif /* GTKMM_WINDOW_H */
