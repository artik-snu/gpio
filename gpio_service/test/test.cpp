#include "gpio.h"
#include <iostream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

using namespace std;

long p_num = 42;
int msg_queue_id;

void msg_create(msg_data &data, gpio_msg_e msg_type, gpio_pin_e pin, uint8_t value) {
    data.data_type=1;
    data.data_num = p_num;
    data.data_buff[0] = (char)msg_type;
    data.data_buff[1] = (char)value;
    *(uint32_t *)(data.data_buff+4) = (uint32_t)pin;
}

inline gpio_msg_e msg_get_type(const msg_data &data) {
    return (gpio_msg_e)data.data_buff[0];
}

inline gpio_pin_e msg_get_pin(const msg_data &data) {
    return (gpio_pin_e)*(uint32_t *)(data.data_buff+4);
}

inline int8_t msg_get_return (msg_data &data) {
    return data.data_buff[2];
}


void send_message(const void *msgp) {
    if (-1 == msgsnd(msg_queue_id, msgp, sizeof(msg_data) - sizeof(long), 0)) {
        perror("msgsnd() failed.");
        exit(1);
    }
}

int main() {
    msg_data data;

    if (-1 == (msg_queue_id = msgget((key_t)913, IPC_CREAT | 0666))) {
        perror("msgget() failed.");
        exit(1);
    }

    msg_create(data, GPIO_SET_DIRECTION, J27_13, GPIO_OUT);
    send_message(&data);
    for (int i = 0; i < 10; i++) {
        msg_create(data, GPIO_SET_DIRECTION, J27_13, GPIO_OUT);
        send_message(&data);
        sleep(1);
        msg_create(data, GPIO_SET_VALUE, J27_13, HIGH);
        send_message(&data);
        sleep(1);
        msg_create(data, GPIO_SET_VALUE, J27_13, LOW);
        send_message(&data);
        sleep(1);
    }
    while (true) {
        if (-1 == msgrcv(msg_queue_id, &data, sizeof(msg_data) - sizeof(long), p_num, 0)) {
            perror("msgsnd() failed.");
            exit(1);
        }
        cout << "MSG TYPE: " << msg_get_type(data) << " RETURN VALUE: " << (int)msg_get_return(data) << " PIN: " << setbase(16) << msg_get_pin(data) << endl;
    }
}
    
    
    
    
