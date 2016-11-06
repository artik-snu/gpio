#include "gpio.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <ctime>
#include <thread>

using namespace std;
using std::chrono::milliseconds;

void gpio_callback (gpio_h gpio, gpio_event_s *event, void *data) {
  cout << "Data for " << gpio->pin << " has changed to " << event->value << endl;
}


int main() {
  gpio_h handle;
  gpio_listener_h listener;
  gpio_get_default_gpio(J27_13, &handle, GPIO_OUT);
  gpio_create_listener(handle, &listener);
//  gpio_listener_set_event_cb(listener, 1, gpio_callback, NULL);
  gpio_listener_start(listener);
  gpio_event_s evt;
  gpio_h led2_h;
  gpio_listener_h led2;
  gpio_get_default_gpio(J27_12, &led2_h, GPIO_OUT);
  gpio_create_listener(led2_h, &led2);
  for (int i = 0; i < 100; i++) {
    //gpio_inject_data(J27_13, (gpio_value_e)(i%2));
    //cout << "value is changed to " << i % 2 << endl;
    gpio_listener_set_data(listener, (gpio_value_e)(i%2));
    gpio_listener_set_data(led2, (gpio_value_e)(1-i%2));
    cout << "writing " << i % 2 << ", returned value is ";

    gpio_listener_read_data(listener, &evt);
    cout << evt.value << endl;
    this_thread::sleep_for(milliseconds(1000));
  }
  gpio_listener_read_data(listener, &evt);
  cout << evt.value << endl;
  gpio_listener_set_data(listener, HIGH);
  gpio_listener_read_data(listener, &evt);
  cout << evt.value << endl;
  return 0;
}
