#include "message_manager.h"


message_manager::~message_manager() {
    std::unordered_map<std::string, subscribtion*>::iterator it = active_subs.begin();

    while (it != active_subs.end()) {
        delete it->second;
        it ++;
    }
    
    std::cout << "Manager finished his work\n";
};

void message_manager::add_new_subscribtion(std::string sub_name, void* memory_unit, size_t unit_size) {
    subscribtion* new_sub = new subscribtion(memory_unit, unit_size);
    active_subs[sub_name] = new_sub;

    std::cout << "New sub added\n";
};

void message_manager::delete_subscribtion(std::string sub_name) {
    if (active_subs.find(sub_name) != active_subs.end()) {
        delete active_subs[sub_name];

        std::cout << "subscribtion " << sub_name << " successfully deleted\n";
    }
    else 
        std::cout << "subscribtion " << sub_name << " does not exist\n";
};