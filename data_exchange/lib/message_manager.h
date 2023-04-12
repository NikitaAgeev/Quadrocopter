#pragma once
#include <iostream>
#include <unordered_map>
#include "subscribtion.h"

class message_manager {
    public:
        ~message_manager();

        void add_new_subscribtion(std::string sub_name, void* memory_unit, size_t unit_size);

        void delete_subscribtion(std::string sub_name);

    private:
        std::unordered_map<std::string, subscribtion*> active_subs;
};
