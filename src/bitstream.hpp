#pragma once

#include <vector>
#include <string>
#include <stdint.h>

class BitStream
{
    public:

    BitStream() = default;
    BitStream(std::string const & path);
    ~BitStream() = default;

    void push(bool value);
    void push(char value);
    void push(uint8_t value);
    void push(float value);
    void push(double value);
    void push(unsigned long long value);
    void push(std::string const & value);

    void push_2_int(unsigned int value);
    void push_4_int(unsigned int value);
    void push_8_int(unsigned int value);
    void push_16_int(unsigned int value);
    void push_32_int(unsigned int value);

    void push_very_short_str(std::string const & value);
    void push_short_str(std::string const & value);

    bool get_bool();
    uint8_t get_uint8();
    char get_char();
    float get_float();
    double get_double();
    unsigned int get_2_int();
    unsigned int get_4_int();
    unsigned int get_8_int();
    unsigned int get_16_int();
    unsigned int get_32_int();
    unsigned long long get_64_int();
    unsigned long long get_int();
    std::string get_very_short_str();
    std::string get_short_str();
    std::string get_str();

    void save(std::string const & path) const;

    private:

    unsigned int write_in_bit_pos_{0};
    unsigned int read_cursor_{0};
    unsigned int read_in_bit_pos_{0};
    std::vector<uint8_t> stream_{0};
};
