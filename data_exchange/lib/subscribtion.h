#pragma once
#include <iostream>

class subscribtion {
    public:
        subscribtion (void* memory_unit, size_t unit_size);

    private:
        void*  memmory_ptr;
        size_t memmory_size;
};