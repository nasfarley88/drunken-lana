#include "window.h"
#include <gtkmm/stock.h>
#include <iostream>

Window::Window()
: m_Box(Gtk::ORIENTATION_VERTICAL)
{
  set_title("main menu example");
  set_default_size(200, 200);

  add(m_Box); // put a MenuBar at the top of the box and other stuff below it.

  //Create actions for menus and toolbars:
  m_refActionGroup = Gtk::ActionGroup::create();

  //File menu:
  m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));

  //Sub-menu.
  m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN));
  m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
          sigc::mem_fun(*this, &Window::on_menu_file_quit));

  //Edit menu:
  // m_refActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));
  // m_refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
  //         sigc::mem_fun(*this, &Window::on_menu_others));
  // m_refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
  //         sigc::mem_fun(*this, &Window::on_menu_others));
  // m_refActionGroup->add(Gtk::Action::create("EditSomething", "Something"),
  //         Gtk::AccelKey("<control><alt>S"),
  //         sigc::mem_fun(*this, &Window::on_menu_others));

  // Playback menu:
  m_refActionGroup->add(Gtk::Action::create("PlaybackMenu", "Playback"));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPrevious",
					    Gtk::Stock::MEDIA_PREVIOUS),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaRewind",
					    Gtk::Stock::MEDIA_REWIND),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPlay",
					    Gtk::Stock::MEDIA_PLAY),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaPause",
					    Gtk::Stock::MEDIA_PAUSE),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaStop",
					    Gtk::Stock::MEDIA_STOP),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaForward",
					    Gtk::Stock::MEDIA_FORWARD),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("PlaybackMediaNext",
					    Gtk::Stock::MEDIA_NEXT),
			sigc::mem_fun(*this, &Window::on_menu_others));


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
        "      <menuitem action='PlaybackMediaNext'/>"
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
        "      <toolitem action='PlaybackMediaNext'/>"
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
}

void Window::on_menu_file_new_generic()
{
   std::cout << "A File|New menu item was selected." << std::endl;
}

void Window::on_menu_others()
{
  std::cout << "A menu item was selected." << std::endl;
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
