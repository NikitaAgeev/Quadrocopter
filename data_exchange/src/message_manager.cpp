#include "message_manager.h"
#include "messages.h"


message_manager::~message_manager() {
    std::unordered_map<std::string, subscribtion*>::iterator it = active_subs.begin();

    while (it != active_subs.end()) {
        delete it->second;
        it ++;
    }
    
    std::cout << "Manager finished his work\n";
};

void message_manager::add_new_subscribtion(std::string sub_name, void* memory_unit, size_t unit_size, int flag) {
    subscribtion* new_sub = new subscribtion(memory_unit, unit_size);
    active_subs[sub_name] = new_sub;

    if (flag == SUB_SEND_DATA) {
        on_sending[sub_name] = new_sub;
    }

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

void message_manager::on_message_recieve() {
    message<msg_types> recieved_msg;

    size_t header_size = sizeof(recieved_msg.header);
    read_bytes_to_msg(source, recieved_msg, header_size);

    size_t body_size = recieved_msg.header.size - header_size;
    read_bytes_to_msg(source + header_size, recieved_msg, body_size);

    switch (recieved_msg.header.id)
    {
    case msg_types::read_data: {
        std::string sub_name = "";
        recieved_msg >> sub_name;

        send_data_once(sub_name);
    }
    break;

    case msg_types::write_data: {
        std::string sub_name = "";
        recieved_msg >> sub_name;

        size_t buf_size = active_subs[sub_name]->unit_size;
        std::vector<unsigned char> buffer(buf_size, 0);

        recieved_msg >> buffer;

        active_subs[sub_name]->store_latest_change(buffer);
        active_subs[sub_name]->to_change = true;
    }
    break;

    case msg_types::start_send_data: {
        std::string sub_name = "";
        recieved_msg >> sub_name;

        add_to_on_sending(active_subs[sub_name]);
    }
    break;

    case msg_types::stop_send_data: {
        std::string sub_name = "";
        recieved_msg >> sub_name;

        remove_from_on_sending(sub_name);
    }
    default:
        break;
    }
};


void send_data_once (std::string sub_name){
    size_t buf_size = active_subs[sub_name]->unit_size;
    std::vector<unsigned char> buffer(buf_size, 0);

    active_subs[sub_name]->read_data(buffer);

    message<msg_types> reply;
    reply.header.id = msg_types::read_data;

    reply << buffer;

    send_on_USB_port(reply);
};