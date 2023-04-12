#pragma once
#include <vector>
#include <string.h>
#include <iostream>
#include <assert.h>

//template T for message ids

template <typename T>
struct message_header {
    T id{};
    uint32_t size = 0;
};

template <typename T>
struct message {

std::vector<uint8_t> body;
message_header<T>    header = {};

size_t size () {
    return sizeof(message_header<T>) + body.size();
};


friend std::ostream& operator << (std::ostream& os, const message<T>& message) {
    os << "ID " << int(message.header.id) << " " << "Size: " << message.header.size << "\n";
    return os;
};

template <typename DataType>
friend message<T>& operator << (message<T>& msg, const DataType& data) {

    //storing current size of a data
    size_t i = msg.body.size();

    msg.body.resize(i + sizeof(DataType));
    std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

    //here we update message size
    msg.header.size = msg.size();

    return msg;
};

template <typename DataType>
friend message<T>& operator >> (message<T>& msg, DataType& data) {
    assert(msg.body.size() >= sizeof(DataType));

    size_t i = msg.body.size() - sizeof(DataType);

    std::memcpy (&data, msg.body.data() + i, sizeof(DataType));
    msg.body.resize(i);

    msg.header.size = msg.size();

    return msg; 
};

template <typename DataType>
friend message<T>& operator << (message<T>& msg, const std::vector<DataType>& buffer){

    size_t i = msg.body.size();

    msg.body.resize(i + buffer.size()*sizeof(DataType));
    std::memcpy (msg.body.data() + i, buffer.data(), sizeof(DataType)*buffer.size());

    msg.header.size = msg.size();

    return msg;
}

template <typename DataType>
friend message<T>& operator >> (message<T>& msg, std::vector<DataType>& buffer){
    int buffer_size = buffer.size();
    buffer_size *= sizeof(DataType);

    assert(msg.body.size() >= buffer_size); //enough data to pour out

    int pos = msg.body.size() - buffer_size;
    std::memcpy(buffer.data(), msg.body.data() + pos, buffer_size);

    msg.body.resize(pos);
    msg.header.size = msg.size();
    
    return msg;
}

};






 