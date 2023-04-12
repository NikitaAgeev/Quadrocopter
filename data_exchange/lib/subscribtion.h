#pragma once
#include <iostream>


class subscribtion {
    public:
        subscribtion (void* memory_unit, size_t unit_size) {
            memory_ptr  = memory_unit;
            memory_size = unit_size;

            to_change = false;
            latest_change(unit_size, 0);
        };

        void read_data(std::vector<unsigned char>& buffer) {
            //memcpy into buffer from memory ptr
        };

        void write_data(std::vector<unsigned char>& buffer) {
            //memcpy into memory ptr from buffer
        };

        void store_latest_change(std::vector<unsigned char>& buffer) {
            //memcpy into latest change from buffer
        };

        void update_latest_change() {
            if (to_change) {
                //memcpy from latest change to memory ptr
                //to_change = false
            }
        };


    private:
        bool   to_change;
        size_t memory_size;

        void*  memory_ptr;
        std::vector <unsigned char> latest_change;

};