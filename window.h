#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>
#include <vlc/vlc.h>
#include <string>
#include "hidapi.h"
#include "pedal.h"

class MyWindow : public Gtk::Window
{
 public:
  MyWindow();
  virtual ~MyWindow();

  void on_menu_previous();
  void on_menu_rewind();
  void on_menu_play();
  void on_menu_pause();
  void on_menu_stop();
  void on_menu_forward();

 protected:
  //Signal handlers:
  void on_menu_file_new_generic();
  void on_menu_file_quit();
  void on_menu_file_open();
  void on_menu_others();
  void on_menu_panic();

  void on_menu_pedal_test();
  void on_menu_choices_one();
  void on_menu_choices_two();

  //Child widgets:
  Gtk::Box m_Box;

  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
  Glib::RefPtr<Gtk::RadioAction> m_refPedalNone, m_refPedalVECInfinity, m_refChoiceOne, m_refChoiceTwo;
  Gtk::RadioAction::Group group_userlevel;


  libvlc_instance_t *inst;
  libvlc_media_player_t *mp;
  libvlc_media_t *m;

  std::string current_filename;

  // Timer stuffs
  bool time_in_title(int x); // aparently callback functions need to
                             // be passed something

  const int time_in_title_timeout_value;

  sigc::connection time_in_title_timer; /* This isn't used... */
  

  // Pedal stuff
  Pedal * pedal_thread;
  void pedal_thread_done();

  hid_device *pedal;
  unsigned char pedal_buf[256];
  bool read_pedal(int x);
  const int read_pedal_timeout;
  void all_the_pedal_things(const int);


};

#endif //GTKMM_WINDOW_H
