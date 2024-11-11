#include "bitstream.hpp"
#include <iostream>

void BitStream::push(bool value)
{
    if(write_in_bit_pos_ > 7)
    {
        write_in_bit_pos_ = 1;
        uint8_t byte = value;
        byte <<= 7;
        stream_.push_back(byte);
        return;
    }

    uint8_t byte = value;
    byte <<= (7-write_in_bit_pos_);
    stream_.back() += byte;
    ++write_in_bit_pos_;
}

void BitStream::push(char value)
{
    push(*(reinterpret_cast<uint8_t*>(&value)));
}

void BitStream::push(uint8_t value)
{
    switch(write_in_bit_pos_)
    {
        case 7:
            stream_.push_back(value);
            break;
        case 0:
            stream_.back() = value;
            break;
        default:
            uint8_t next_byte = value << (8-write_in_bit_pos_);
            stream_.back() += value >> write_in_bit_pos_;
            stream_.push_back(next_byte);
            break;
    }
}

void BitStream::push(float value)
{
    uint32_t bytes = *(reinterpret_cast<uint32_t*>(&value));

    push(static_cast<uint8_t>((bytes >> 24)%256));
    push(static_cast<uint8_t>((bytes >> 16)%256));
    push(static_cast<uint8_t>((bytes >> 8 )%256));
    push(static_cast<uint8_t>( bytes       %256));
}

void BitStream::push(double value)
{
    uint64_t bytes = *(reinterpret_cast<uint64_t*>(&value));

    push(static_cast<uint8_t>((bytes >> 56)%256));
    push(static_cast<uint8_t>((bytes >> 48)%256));
    push(static_cast<uint8_t>((bytes >> 40)%256));
    push(static_cast<uint8_t>((bytes >> 32)%256));
    push(static_cast<uint8_t>((bytes >> 24)%256));
    push(static_cast<uint8_t>((bytes >> 16)%256));
    push(static_cast<uint8_t>((bytes >> 8 )%256));
    push(static_cast<uint8_t>( bytes       %256));
}

void BitStream::push(unsigned long long value)
{
    if(value < 256)
    {
        push_2_int(0);
        push(static_cast<uint8_t>(value));
        return;
    }
    if(value < 65536)
    {
        push_2_int(1);
        push_16_int(static_cast<unsigned int>(value));
        return;
    }
    if(value < 16777216)
    {
        push_2_int(2);
        push_32_int(static_cast<unsigned int>(value));
        return;
    }
    push_2_int(3);
    push(static_cast<uint8_t>(value >> 56));
    push(static_cast<uint8_t>(value >> 48));
    push(static_cast<uint8_t>(value >> 40));
    push(static_cast<uint8_t>(value >> 32));
    push(static_cast<uint8_t>(value >> 24));
    push(static_cast<uint8_t>(value >> 16));
    push(static_cast<uint8_t>(value >> 8));
    push(static_cast<uint8_t>(value));
}

void BitStream::push(std::string const & value)
{
    push(static_cast<unsigned long long>(value.size()));
    for(unsigned long long i(0); i < value.size(); ++i)
    {
        push(value[i]);
    }
}

void BitStream::push_2_int(unsigned int value)
{
    push(static_cast<bool>((value >> 1)%2));
    push(static_cast<bool>(value%2));
}

void BitStream::push_4_int(unsigned int value)
{
    push(static_cast<bool>((value >> 3)%2));
    push(static_cast<bool>((value >> 2)%2));
    push(static_cast<bool>((value >> 1)%2));
    push(static_cast<bool>((value)%2));
}

void BitStream::push_8_int(unsigned int value)
{
    push(static_cast<uint8_t>(value));
}

void BitStream::push_16_int(unsigned int value)
{
    push(static_cast<uint8_t>(value >> 8));
    push(static_cast<uint8_t>(value));
}

void BitStream::push_32_int(unsigned int value)
{
    push(static_cast<uint8_t>(value >> 24));
    push(static_cast<uint8_t>(value >> 16));
    push(static_cast<uint8_t>(value >> 8));
    push(static_cast<uint8_t>(value));
}

void BitStream::push_very_short_str(std::string const & value)
{
    push_4_int(value.size());
    for(unsigned int i(0); i < value.size()%16; ++i)
    {
        push(value[i]);
    }
}

void BitStream::push_short_str(std::string const & value)
{
    push_8_int(value.size());
    for(unsigned int i(0); i < value.size()%256; ++i)
    {
        push(value[i]);
    }
}

bool BitStream::get_bool()
{
    if(read_in_bit_pos_ > 7)
    {
        read_in_bit_pos_ = 0;
        ++read_cursor_;
    }
    if(read_cursor_ >= stream_.size())
    {
        std::cerr << "BitStream error : Data expected but not found" << std::endl;
    }

    bool bit = static_cast<bool>((stream_[read_cursor_] >> (7-read_in_bit_pos_))%2);
    ++read_in_bit_pos_;

    return bit;
}

uint8_t BitStream::get_uint8()
{
    if(read_in_bit_pos_ > 7)
    {
        read_in_bit_pos_ = 0;
        ++read_cursor_;
    }
    if(read_cursor_ >= stream_.size() || (read_in_bit_pos_ != 0 && read_cursor_+1 >= stream_.size()))
    {
        std::cerr << "BitStream error : Data expected but not found" << std::endl;
    }
    if(read_in_bit_pos_ == 0)
    {
        uint8_t byte = stream_[read_cursor_];
        ++read_cursor_;
        return byte;
    }

    uint8_t byte = (stream_[read_cursor_] >> read_in_bit_pos_) << read_in_bit_pos_;
    ++read_cursor_;
    byte += stream_[read_cursor_] >> (7-read_in_bit_pos_);
    return byte;
}

char BitStream::get_char()
{
    return static_cast<char>(get_uint8());
}

float BitStream::get_float()
{
    uint32_t value = get_32_int();
    return *(reinterpret_cast<float*>(&value));
}

double BitStream::get_double()
{
    uint64_t value = get_64_int();
    return *(reinterpret_cast<double*>(&value));
}

unsigned int BitStream::get_2_int()
{
    return (static_cast<unsigned int>(get_bool()) << 1) + static_cast<unsigned int>(get_bool());
}

unsigned int BitStream::get_4_int()
{
    unsigned int x = static_cast<unsigned int>(get_bool()) << 3;
    x += static_cast<unsigned int>(get_bool()) << 2;
    x += static_cast<unsigned int>(get_bool()) << 1;
    x += static_cast<unsigned int>(get_bool());

    return x;
}

unsigned int BitStream::get_8_int()
{
    return static_cast<unsigned int>(get_uint8());
}

unsigned int BitStream::get_16_int()
{
    return (get_8_int() << 8) + get_8_int();
}

unsigned int BitStream::get_32_int()
{
    return (get_16_int() << 16) + get_16_int();
}

unsigned long long BitStream::get_64_int()
{
    return (static_cast<unsigned long long>(get_32_int()) << 32) + get_32_int();
}

unsigned long long BitStream::get_int()
{
    unsigned int int_type = get_2_int();
    switch(int_type)
    {
        case 0:
            return static_cast<unsigned long long>(get_8_int());
        case 1:
            return static_cast<unsigned long long>(get_16_int());
        case 2:
            return static_cast<unsigned long long>(get_32_int());
        default:
            return static_cast<unsigned long long>(get_64_int());
    }
}

std::string BitStream::get_very_short_str()
{
    unsigned int size = get_4_int();
    std::string str;

    for(unsigned int i(0); i < size; ++i)
    {
        str.push_back(get_char());
    }

    return str;
}

std::string BitStream::get_short_str()
{
    unsigned int size = get_8_int();
    std::string str;

    for(unsigned int i(0); i < size; ++i)
    {
        str.push_back(get_char());
    }

    return str;
}

std::string BitStream::get_str()
{
    unsigned long long size = get_int();
    std::string str;

    for(unsigned long long i(0); i < size; ++i)
    {
        str.push_back(get_char());
    }

    return str;
}
