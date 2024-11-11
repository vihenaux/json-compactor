#include "bitstream.hpp"

void BitStream::push(bool value)
{
    if(in_bit_pos_ > 7)
    {
        in_bit_pos_ = 1;
        uint8_t byte = value;
        byte <<= 7;
        stream_.push_back(byte);
        return;
    }

    uint8_t byte = value;
    byte <<= (7-in_bit_pos_);
    stream_.back() += byte;
    ++in_bit_pos_;
}

void BitStream::push(char value)
{
    push(*(reinterpret_cast<uint8_t*>(&value)));
}

void BitStream::push(uint8_t value)
{
    switch(in_bit_pos_)
    {
        case 7:
            stream_.push_back(value);
            break;
        case 0:
            stream_.back() = value;
            break;
        default:
            uint8_t next_byte = value << (8-in_bit_pos_);
            stream_.back() += value >> in_bit_pos_;
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
char BitStream::get_char()
float BitStream::get_float()
double BitStream::get_double()
unsigned int BitStream::get_2_int()
unsigned int BitStream::get_4_int()
unsigned int BitStream::get_8_int()
unsigned int BitStream::get_16_int()
unsigned int BitStream::get_32_int()
unsigned long long BitStream::get_64_int()
std::string BitStream::get_very_short_str()
std::string BitStream::get_short_str()
std::string BitStream::get_str()