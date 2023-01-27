#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BYTES 1000

unsigned char buffer[MAX_BYTES];
int buffer_index = 0;

void write_string(const char *str) {
    int len = strlen(str);
    write_int(len);
    for (int i = 0; i < len; i++) {
        buffer[buffer_index++] = str[i];
    }
}

void write_int(int value) {
    buffer[buffer_index++] = (value >> 24) & 0xFF;
    buffer[buffer_index++] = (value >> 16) & 0xFF;
    buffer[buffer_index++] = (value >> 8) & 0xFF;
    buffer[buffer_index++] = value & 0xFF;
}

void write_byte(unsigned char value) {
    buffer[buffer_index++] = value;
}

void write_vint(int value) {
    if (value < 0) {
        value = ~value;
    }
    while (value >= 0x80) {
        buffer[buffer_index++] = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    buffer[buffer_index++] = value;
}

char* read_string() {
    int len = read_int();
    char* str = (char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        str[i] = buffer[buffer_index++];
    }
    str[len] = '\0';
    return str;
}

int read_int() {
    int value = 0;
    value |= buffer[buffer_index++] << 24;
    value |= buffer[buffer_index++] << 16;
    value |= buffer[buffer_index++] << 8;
    value |= buffer[buffer_index++];
    return value;
}

unsigned char read_byte() {
    return buffer[buffer_index++];
}

int read_vint() {
    int value = 0;
    int shift = 0;
    unsigned char b;
    do {
        b = buffer[buffer_index++];
        value |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if (value >= (1 << (shift - 1))) {
        value = -(value & ~(1 << (shift - 1)));
    }
    return value;
}

int main() {
    write_string("Hello World!");
    write_int(12345);
    write_byte(0xFF);
    write_vint(100);

    char* str = read_string();
    printf("String: %s\n", str);
    free(str);
    printf("Integer: %d\n", read_int());
    printf("Byte: %02X\n", read_byte());
    return 0;
