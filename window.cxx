#include "window.h"
#include <gtkmm/stock.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// USB stuff
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"


#include <vlc/vlc.h>

Window::Window()
  : m_Box(Gtk::ORIENTATION_VERTICAL),
    time_in_title_timeout_value(2),
    read_pedal_timeout(2)
{

  // Load pedal stuff
  struct hid_device_info *devs, *cur_dev;
	
  devs = hid_enumerate(0x0, 0x0);
  cur_dev = devs;	
  while (cur_dev) {
    printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
    printf("\n");
    printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
    printf("  Product:      %ls\n", cur_dev->product_string);
    printf("  Release:      %hx\n", cur_dev->release_number);
    printf("  Interface:    %d\n",  cur_dev->interface_number);
    printf("\n");
    cur_dev = cur_dev->next;
  }
  hid_free_enumeration(devs);

  // Loads the pedal
  pedal = hid_open(0x5f3, 0xff, NULL);
  if (!pedal) {
    printf("unable to open device\n");
  }

  // I have no idea why these are here. but I need pedal_buf later so I'll put
  // this in. memset reserves memeroy at the address specified for pedal_buf
  // memset(pedal_buf,0x00,sizeof(pedal_buf));
  // pedal_buf[0] = 0x01;
  // pedal_buf[1] = 0x81;

  // Set the hid_read() function to be non-blocking.
  if(pedal) hid_set_nonblocking(pedal, 0); // TODO OK, if you can't open the pedal it
  				 // segfaults here.
  std::cout << "I just tried to set the device to nonblocking" << std::endl;
  
  // Now I need a timer for the pedal. 
  if(pedal) {
    sigc::slot<bool> pedal_slot =
      sigc::bind(sigc::mem_fun(*this, &Window::read_pedal),0);
    sigc::connection pedal_timer_conn =
     Glib::signal_timeout().connect(pedal_slot, read_pedal_timeout);
  }

  
  // load the vlc engine
  inst = libvlc_new(0, NULL);
  mp = libvlc_media_player_new(inst);

  // Sets the volume
  std::cout << " result of audio set is " << libvlc_audio_set_volume(mp, 25) << std::endl;

  // Tiimer stuff
  sigc::slot<bool> my_slot =
    sigc::bind(sigc::mem_fun(*this, &Window::time_in_title),0);
  sigc::connection conn = Glib::signal_timeout().connect(my_slot,
          time_in_title_timeout_value);

  current_filename = "null";

  set_title("Drunken Lana");
  set_default_size(300, 100);

  add(m_Box); // put a MenuBar at the top of the box and other stuff below it.

  //Create actions for menus and toolbars:
  m_refActionGroup = Gtk::ActionGroup::create();

  //File menu:
  m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));

  //Sub-menu.
  m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
			sigc::mem_fun(*this, &Window::on_menu_file_open));
  m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &Window::on_menu_file_quit));

  // Playback menu:
  m_refActionGroup->add(Gtk::Action::create("PlaybackMenu", "Playback"));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPrevious",
					    Gtk::Stock::MEDIA_PREVIOUS),
			sigc::mem_fun(*this, &Window::on_menu_previous));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaRewind",
					    Gtk::Stock::MEDIA_REWIND),
			sigc::mem_fun(*this, &Window::on_menu_rewind));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPlay",
					    Gtk::Stock::MEDIA_PLAY),
			sigc::mem_fun(*this, &Window::on_menu_play));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPause",
					    Gtk::Stock::MEDIA_PAUSE),
			sigc::mem_fun(*this, &Window::on_menu_pause));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaStop",
					    Gtk::Stock::MEDIA_STOP),
			sigc::mem_fun(*this, &Window::on_menu_stop));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaForward",
					    Gtk::Stock::MEDIA_FORWARD),
			sigc::mem_fun(*this, &Window::on_menu_forward));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPanic",
					    "Reset Volume"),
			sigc::mem_fun(*this, &Window::on_menu_panic));
  // m_refActionGroup->add(Gtk::Action::create("PlaybackMediaNext",
  // 					    Gtk::Stock::MEDIA_NEXT),
  // 			sigc::mem_fun(*this, &Window::on_menu_others));


  //Choices menu, to demonstrate Radio items
  m_refActionGroup->add( Gtk::Action::create("ChoicesMenu", "Choices") );
  Gtk::RadioAction::Group group_userlevel;
  m_refChoiceOne = Gtk::RadioAction::create(group_userlevel, "ChoiceOne", "One");
  m_refActionGroup->add(m_refChoiceOne,
          sigc::mem_fun(*this, &Window::on_menu_choices_one) );
  m_refChoiceTwo = Gtk::RadioAction::create(group_userlevel, "ChoiceTwo", "Two");
  m_refActionGroup->add(m_refChoiceTwo,
          sigc::mem_fun(*this, &Window::on_menu_choices_two) );

  //Help menu:
  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
  m_refActionGroup->add( Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
          sigc::mem_fun(*this, &Window::on_menu_others) );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);

  add_accel_group(m_refUIManager->get_accel_group());

  //Layout the actions in a menubar and toolbar:
  Glib::ustring ui_info = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileOpen'/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='PlaybackMenu'>"
        "      <menuitem action='PlaybackMediaPrevious'/>"
        "      <menuitem action='PlaybackMediaRewind'/>"
        "      <menuitem action='PlaybackMediaPlay'/>"
        "      <menuitem action='PlaybackMediaPause'/>"
        "      <menuitem action='PlaybackMediaStop'/>"
        "      <menuitem action='PlaybackMediaForward'/>"
        "      <menuitem action='PlaybackMediaPanic'/>"
        // "      <menuitem action='PlaybackMediaNext'/>"
        "    </menu>"
        // "    <menu action='ChoicesMenu'>"
        // "      <menuitem action='ChoiceOne'/>"
        // "      <menuitem action='ChoiceTwo'/>"
        // "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "      <toolitem action='PlaybackMediaPrevious'/>"
        "      <toolitem action='PlaybackMediaRewind'/>"
        "      <toolitem action='PlaybackMediaPlay'/>"
        "      <toolitem action='PlaybackMediaPause'/>"
        "      <toolitem action='PlaybackMediaStop'/>"
        "      <toolitem action='PlaybackMediaForward'/>"
        // "      <toolitem action='PlaybackMediaPanic'/>" // TODO get an icon
        // for this!
        // "      <toolitem action='PlaybackMediaNext'/>"
        "  </toolbar>"
        "</ui>";

  try
  {
    m_refUIManager->add_ui_from_string(ui_info);
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

  //Get the menubar and toolbar widgets, and add them to a container widget:
  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
  if(pMenubar)
    m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

  Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
  if(pToolbar)
    m_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

  show_all_children();
}

Window::~Window()
{
}

void Window::on_menu_file_quit()
{
  hide(); //Closes the main window to stop the app->run().
  libvlc_release(inst);
}

void Window::on_menu_file_open() {
  Gtk::FileChooserDialog dialog("Please choose file",
				Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  //Add filters, so that only certain file types can be selected:

  // Gtk::FileFilter filter_text;
  // filter_text.set_name("Text files");
  // filter_text.add_mime_type("text/plain");
  // dialog.add_filter(filter_text);

  // Gtk::FileFilter filter_cpp;
  // filter_cpp.set_name("C/C++ files");
  // filter_cpp.add_mime_type("text/x-c");
  // filter_cpp.add_mime_type("text/x-c++");
  // filter_cpp.add_mime_type("text/x-c-header");
  // dialog.add_filter(filter_cpp);

  // Gtk::FileFilter filter_any;
  // filter_any.set_name("Any files");
  // filter_any.add_pattern("*");
  // dialog.add_filter(filter_any);

  int result = dialog.run();

  switch(result) {

  case(Gtk::RESPONSE_OK): {

    // libvlc_media_release(m); 	// releasing any other media 

    std::cout << "Open clicked." << std::endl;

    //Notice that this is an std::string not a Glib::ustring
    std::string filename = dialog.get_filename();
    current_filename = filename;
    std::cout << "File selected: " << filename << std::endl;
    m = libvlc_media_new_path(inst, filename.c_str() );
    // mp = libvlc_media_player_new_from_media(m);
    libvlc_media_player_set_media(mp, m);
    libvlc_media_release(m);
    // TODO let the user set the volume

    this->set_title("Whut");
    break;

  }

  case(Gtk::RESPONSE_CANCEL): {

    std::cout << "Cancel clicked." << std::endl;
    break;

  }

  default: {

    std::cout << "Unexpected button clicked." << std::endl;
    break;

  }

  }
}

void Window::on_menu_file_new_generic()
{
   std::cout << "A File|New menu item was selected." << std::endl;
}

void Window::on_menu_previous() {
  libvlc_media_player_set_time(mp, 0);
  libvlc_media_player_play(mp);
}

void Window::on_menu_rewind() {
  libvlc_time_t t = libvlc_media_player_get_time(mp);
  libvlc_media_player_set_time(mp, t - 1000);
  libvlc_media_player_play(mp);
}

void Window::on_menu_play() {
  libvlc_media_player_play(mp);
  libvlc_media_player_set_rate(mp, 1.0);
}

void Window::on_menu_pause() {
  libvlc_media_player_set_pause(mp, 1);
}

void Window::on_menu_stop() {
  libvlc_media_player_stop(mp);
}

void Window::on_menu_forward() {
  libvlc_media_player_set_rate(mp, 3.0);
}

void Window::on_menu_others()
{
  std::cout << "A menu item was selected." << std::endl;
}

void Window::on_menu_panic() {
    std::cout << "Result of audio set is " << libvlc_audio_set_volume(mp, 100) << std::endl;
    std::cout << "The audio should no longer be distorted" << std::endl;
    // TODO investigate if distorted audio with libvlc is a known bug (I think it is)
}

void Window::on_menu_choices_one()
{
  Glib::ustring message;
  if(m_refChoiceOne->get_active())
    message = "Choice 1 was selected.";
  else
    message = "Choice 1 was deselected";

  std::cout << message << std::endl;
}

void Window::on_menu_choices_two()
{
  Glib::ustring message;
  if(m_refChoiceTwo->get_active())
    message = "Choice 2 was selected.";
  else
    message = "Choice 2 was deselected";

  std::cout << message << std::endl;
}

bool Window::time_in_title(int x) {
  std::string tmp;
  std::stringstream tmpstream;
  int time_ms = (int)libvlc_media_player_get_time(mp);
  int length_ms = (int)libvlc_media_player_get_length(mp);
  tmpstream << (time_ms/60000) % 60 // TODO format these numbers so that it's 01
				    // instead of 1
	    << ":"
	    << std::setfill('0')
	    << std::setw(2)
	    << (time_ms/1000) % 60
	    << "/"
	    << (length_ms/60000) % 60
	    << ":"
	    << std::setfill('0')
	    << std::setw(2)
	    << (length_ms/1000) % 60
	    << " - "
	    << current_filename;
  tmp = tmpstream.str();
  this->set_title(tmp.c_str() );
  return true;
}

// TODO add a condition that looks for a change in pedal state rather than what
// the state currently is
bool Window::read_pedal(int x) {
  // if the pedal is assigned, do it!
  if(pedal) {
    unsigned char buf[256];
    int res = hid_read(pedal, buf, sizeof(buf));
    if(buf[0] == 00) this->on_menu_play();
    if(buf[0] == 02) this->on_menu_pause();
  }
  else {
    std::cout << "No pedal is attached" << std::endl;
  }
  return true;
}
