#include <iostream>
#include "window.h"

Window::Window()
  : m_skip_to_start("skip_to_start"),
    m_rewind("rewind"),
    m_play("play"),
    m_pause("pause"),
    m_stop("stop"),
    m_fast_forward("fast_forward"),
    m_skip_to_end("skip_to_end"),
    m_button_quit("Quit")
{
  set_title("Gtk::Grid");
  set_border_width(12);

  add(m_grid);



  m_grid.add(m_skip_to_start);
  m_grid.add(m_rewind);
  m_grid.add(m_play);
  m_grid.add(m_pause);
  m_grid.add(m_stop);
  m_grid.add(m_fast_forward);
  m_grid.add(m_skip_to_end);
  m_grid.attach_next_to(m_button_quit, m_skip_to_start, Gtk::POS_BOTTOM, 2, 1);

  m_skip_to_start.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "skip to start") );
  m_rewind.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "rewind") );
  m_play.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "play") );
  m_pause.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "pause") );
  m_stop.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "stop") );
  m_fast_forward.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "fast_forward") );
  m_skip_to_end.signal_clicked().connect(
    sigc::bind<Glib::ustring>( sigc::mem_fun(*this,
      &Window::on_button_numbered), "skip_to_end") );

  m_button_quit.signal_clicked().connect(sigc::mem_fun(*this,
    &Window::on_button_quit) );

  show_all_children();
}

Window::~Window()
{
}

void Window::on_button_quit()
{
  hide();
}

void
Window::on_button_numbered(const Glib::ustring& data)
{
  std::cout << data << " was pressed" << std::endl;
}
