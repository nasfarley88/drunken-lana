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
    m_button_quit("Quit"),
    m_Box(Gtk::ORIENTATION_VERTICAL)
{
  set_title("Gtk::Grid");
  set_border_width(12);

  add(m_grid);
  // add(m_Box); // put a MenuBar at the top of the box and other stuff below it.

  //Create actions for menus and toolbars:
  m_refActionGroup = Gtk::ActionGroup::create();

  //File|New sub menu:
  m_refActionGroup->add(Gtk::Action::create("FileNewStandard",
					    Gtk::Stock::NEW, "_New", "Create a new file"),
			sigc::mem_fun(*this, &Window::on_menu_file_new_generic));

  m_refActionGroup->add(Gtk::Action::create("FileNewFoo",
					    Gtk::Stock::NEW, "New Foo", "Create a new foo"),
			sigc::mem_fun(*this, &Window::on_menu_file_new_generic));

  m_refActionGroup->add(Gtk::Action::create("FileNewGoo",
					    Gtk::Stock::NEW, "_New Goo", "Create a new goo"),
			sigc::mem_fun(*this, &Window::on_menu_file_new_generic));

  //File menu:
  m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));
  //Sub-menu.
  m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW));
  m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &Window::on_menu_file_quit));

  //Edit menu:
  m_refActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));
  m_refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
			sigc::mem_fun(*this, &Window::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("EditSomething", "Something"),
			Gtk::AccelKey("<control><alt>S"),
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
    "      <menu action='FileNew'>"
    "        <menuitem action='FileNewStandard'/>"
    "        <menuitem action='FileNewFoo'/>"
    "        <menuitem action='FileNewGoo'/>"
    "      </menu>"
    "      <separator/>"
    "      <menuitem action='FileQuit'/>"
    "    </menu>"
    "    <menu action='EditMenu'>"
    "      <menuitem action='EditCopy'/>"
    "      <menuitem action='EditPaste'/>"
    "      <menuitem action='EditSomething'/>"
    "    </menu>"
    "    <menu action='ChoicesMenu'>"
    "      <menuitem action='ChoiceOne'/>"
    "      <menuitem action='ChoiceTwo'/>"
    "    </menu>"
    "    <menu action='HelpMenu'>"
    "      <menuitem action='HelpAbout'/>"
    "    </menu>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "    <toolitem action='FileNewStandard'/>"
    "    <toolitem action='FileQuit'/>"
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


  // End of menu
  //
  // Start of buttons



  m_grid.add(m_skip_to_start);
  m_grid.add(m_rewind);
  m_grid.add(m_play);
  m_grid.add(m_pause);
  m_grid.add(m_stop);
  m_grid.add(m_fast_forward);
  m_grid.add(m_skip_to_end);
  if(pMenubar)
    m_grid.attach_next_to(*pMenubar, m_skip_to_start, Gtk::POS_TOP, 2,1);
  // m_grid.attach_next_to(m_button_quit, m_skip_to_start, Gtk::POS_BOTTOM, 2, 1);

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
