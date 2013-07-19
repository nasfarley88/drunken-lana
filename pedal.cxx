#include "pedal.h"
#include <gtkmm.h>
#include <iostream>
#include "window.h"

Pedal::Pedal(void* x) : read_pedal_timeout(2) {

  thread = 0;
  stop = false;
  mywindow = (MyWindow *)x;

  // Find all HID's and print thier details
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

  // Loads the pedal (VEC infinity only)
  pedal = hid_open(0x5f3, 0xff, NULL);
  // pedal = hid_open_path("/dev/hidraw0");
  if (!pedal) {
    printf("unable to open device\n");
  } else {
    std::cout << "Device open!" << std::endl;
  }

  // Set the hid_read() function to be non-blocking.
  // TODO set it to be blocking when the multithreading works properly
  if(pedal) hid_set_nonblocking(pedal, 0);
  std::cout << "I just tried to set the device to  blocking" << std::endl;
  std::cout << "I made a Pedal" << std::endl;
}
 
// Called to start the processing on the thread
void Pedal::start () {
  std::cout << "Pedal started" << std::endl;
  thread = Glib::Thread::create(sigc::mem_fun(*this, &Pedal::run), true);
  std::cout << "Pedal start ended" << std::endl;
}
 
// When shutting down, we need to stop the thread
Pedal::~Pedal() {
  {
    Glib::Mutex::Lock lock (mutex);
    stop = true;
  }
  if (thread)
    thread->join(); // Here we block to truly wait for the thread to complete
}
 
// This is where the real work happens
void Pedal::run () {
  std::cout << "Threading time!" << std::endl;
  while(true) {
    {
      Glib::Mutex::Lock lock (mutex);
      if (stop) break;
    }
    // Load pedal stuff
  
    // Now I need a timer for the pedal. 
    if(pedal) {
      // sigc::slot<bool> pedal_slot =
      // 	sigc::bind(sigc::mem_fun(*this, &Pedal::read_pedal), 0);
      // sigc::connection pedal_timer_conn =
      // 	Glib::signal_timeout().connect(pedal_slot, read_pedal_timeout);
      while (true) {
	// sleep(2);
      	this->read_pedal(0);
      }
    }

    std::cout << "Pedal timer set" << std::endl;
    sig_done();
    break;
  }
}

bool Pedal::read_pedal(int x) {
  // if the pedal is assigned, do it!
  if(pedal) {
    // std::cout << "Here is where I would've read from the pedal!" << std::endl;
    unsigned char buf[1];
    hid_read(pedal, buf, sizeof(buf));
    // std::cout << "But here I have actually read from the pedal" << std::endl;
    if(buf[0] == 00) (*(MyWindow*)mywindow).on_menu_pause();
    if(buf[0] == 01) (*(MyWindow*)mywindow).on_menu_forward();
    if(buf[0] == 02) (*(MyWindow*)mywindow).on_menu_play();
  }
  else {
    std::cout << "No pedal is attached" << std::endl;
  }
  return true;
}
