#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>
#include <vlc/vlc.h>

class Window : public Gtk::Window
{
public:
Window();
virtual ~Window();

protected:
//Signal handlers:
void on_menu_file_new_generic();
void on_menu_file_quit();
void on_menu_file_open();
void on_menu_play();
void on_menu_pause();
void on_menu_stop();
void on_menu_forward();
void on_menu_others();

void on_menu_choices_one();
void on_menu_choices_two();

//Child widgets:
Gtk::Box m_Box;

Glib::RefPtr<Gtk::UIManager> m_refUIManager;
Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
Glib::RefPtr<Gtk::RadioAction> m_refChoiceOne, m_refChoiceTwo;


libvlc_instance_t *inst;
libvlc_media_player_t *mp;
libvlc_media_t *m;

};

#endif //GTKMM_WINDOW_H
