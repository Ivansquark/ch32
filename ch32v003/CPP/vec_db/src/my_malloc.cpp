#include "my_malloc.h"

void* operator new(size_t size) {
    void* ptr = malloc(size);
    printVal((char*)"malloc_size = ", size);
    printAddr((char*)"malloc_ptr = 0x", (uint32_t)ptr);
    return ptr;
}
void* operator new[](size_t size) {
    void* ptr = malloc(size);
    printVal((char*)"malloc[]_size = ", size);
    printAddr((char*)"malloc[]_ptr = 0x", (uint32_t)ptr);
    return ptr;
}
void operator delete(void* p) {
    printAddr((char*)"free_ptr = 0x", (uint32_t)p);
    free(p);
}
void operator delete[](void* p) {
    printAddr((char*)"free[]_ptr = 0x", (uint32_t)p);
    free(p);
}
void operator delete(void* p, unsigned int) {
    printAddr((char*)"free_uint_ptr = 0x", (uint32_t)p);
    free(p);
}
void operator delete[](void* p, unsigned int) {
    printAddr((char*)"free[]_uint_ptr = 0x", (uint32_t)p);
    free(p);
}
// void operator delete(void* p, [[maybe_unused]] unsigned int x) { free(p); }

// namespace std {
// void __throw_length_error(const char*) {
//     while (true) {}
// }
// void __throw_bad_alloc() {
//     while (true) {}
// }
// } // namespace std
