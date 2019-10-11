#include <iostream>

#include <stdint.h>

#include <tools/socket/Endian.h>

union U64 {
    uint64_t u64;
    uint8_t arr[8];
};

union U32 {
    uint32_t u32;
    uint8_t arr[4];
};

union U16 {
    uint16_t u16;
    uint8_t arr[2];
};

int main(){
    U64 val_64;
    val_64.arr[0] = 0x11;
    val_64.arr[1] = 0x22;
    val_64.arr[2] = 0x33;
    val_64.arr[3] = 0x44;
    val_64.arr[4] = 0x55;
    val_64.arr[5] = 0x66;
    val_64.arr[6] = 0x77;
    val_64.arr[7] = 0x88;

    std::cout << std::hex << val_64.u64 << std::endl;
    std::cout << std::hex << sockets::host_to_network64(val_64.u64) << std::endl;
    std::cout << std::hex << sockets::network_to_host64(val_64.u64) << std::endl;

    U32 val_32;
    val_32.arr[0] = 0x11;
    val_32.arr[1] = 0x22;
    val_32.arr[2] = 0x33;
    val_32.arr[3] = 0x44;

    std::cout << std::hex << val_32.u32 << std::endl;
    std::cout << std::hex << sockets::host_to_network32(val_32.u32) << std::endl;
    std::cout << std::hex << sockets::network_to_host32(val_32.u32) << std::endl;

    U16 val_16;
    val_16.arr[0] = 0x11;
    val_16.arr[1] = 0x22;
    std::cout << std::hex << val_16.u16 << std::endl;
    std::cout << std::hex << sockets::host_to_network16(val_16.u16) << std::endl;
    std::cout << std::hex << sockets::network_to_host16(val_16.u16) << std::endl;

    return 0;
}