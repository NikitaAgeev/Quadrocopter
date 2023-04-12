#pragma once
#include <iostream>
#include <unordered_map>
#include "subscribtion.h"

const int SUB_WR;
const int SUB_SEND_DATA;

enum msg_types {
    read_data,
    write_data,
    start_send_data,
    stop_send_data
};

void send_data_once (std::string sub_name);

class message_manager {
    public:
        ~message_manager();

        void add_new_subscribtion(std::string sub_name, void* memory_unit, size_t unit_size, int flag = SUB_WR);

        void delete_subscribtion(std::string sub_name);

        void on_message_recieve();

        void send_all_data() {
            std::unordered_map<std::string, subscribtion*>::iterator it = on_sending.begin();

            while (it != on_sending.end()) {
                send_data_once(it->first);
                it ++;
            }
        };

        void update_subs_memory() {
            std::unordered_map<std::string, subscribtion*>::iterator it = on_sending.begin();

            while (it != on_sending.end()) {
                active_subs[it->first]->update_latest_change();
                it ++;
            }
        };

    private:
        std::unordered_map<std::string, subscribtion*> active_subs;
        std::unordered_map<std::string, subscribtion*> on_sending;
};
