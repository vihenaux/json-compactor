#include "bitstream.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

BitStream::BitStream(std::string const & path)
{
    std::ifstream input_file(path, std::ios_base::binary);

    if(!input_file.is_open())
    {
        std::cerr << "Cannot open file at : " << path << std::endl;
        return;
    }

    char single_buffer;
    for(unsigned int i(0); i < 4; ++i)
    {
        input_file.get(single_buffer);
        push(single_buffer);
    }

    unsigned int file_size = get_32_int();
    char *buffer = new char[file_size];

    input_file.read(buffer, file_size);

    for(unsigned int i(0); i < file_size; ++i)
    {
        push(buffer[i]);
    }

    delete[] buffer;
}

void BitStream::push(bool value)
{
    switch(write_in_bit_pos_)
    {
        case 8:
            write_in_bit_pos_ = 1;
            stream_.push_back(static_cast<uint8_t>(value << 7));
            break;
        default:
            stream_.back() += static_cast<uint8_t>(value) << (7-write_in_bit_pos_);
            ++write_in_bit_pos_;
    }
}

void BitStream::push(char value)
{
    push(*(reinterpret_cast<uint8_t*>(&value)));
}

void BitStream::push(uint8_t value)
{
    switch(write_in_bit_pos_)
    {
        case 8:
            stream_.push_back(value);
            break;
        case 0:
            stream_.back() = value;
            write_in_bit_pos_ = 8;
            break;
        default:
            uint8_t next_byte = static_cast<uint8_t>(static_cast<uint8_t>(value) << (8-write_in_bit_pos_));
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
        push_8_int(static_cast<unsigned int>(value));
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
    push_32_int(static_cast<unsigned int>(value >> 32));
    push_32_int(static_cast<unsigned int>(value));
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
    push_4_int(static_cast<unsigned int>(value.size()));
    for(unsigned int i(0); i < value.size()%16; ++i)
    {
        push(value[i]);
    }
}

void BitStream::push_short_str(std::string const & value)
{
    push_8_int(static_cast<unsigned int>(value.size()));
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

    uint8_t byte = static_cast<uint8_t>(stream_[read_cursor_] << read_in_bit_pos_);
    ++read_cursor_;
    byte += stream_[read_cursor_] >> (8-read_in_bit_pos_);
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

void BitStream::save(std::string const & path) const
{
    std::ofstream output_file(path, std::ios_base::binary);
    BitStream tmp;
    tmp.push_32_int(static_cast<unsigned int>(stream_.size()));
    output_file << tmp.get_char() << tmp.get_char() << tmp.get_char() << tmp.get_char();

    for(unsigned int i(0); i < stream_.size(); ++i)
    {
        output_file << stream_[i];
    }
}

void BitStream::debug() const
{
    std::cout << "write in bit position : " << write_in_bit_pos_ << std::endl;
    std::cout << "read cursor : " << read_cursor_ << std::endl;
    std::cout << "read in bit position : " << read_in_bit_pos_ << std::endl;
    std::cout << "Stream size : " << stream_.size() << std::endl;
    for(unsigned int i(0); i < stream_.size(); ++i)
    {
        std::cout << std::hex << static_cast<unsigned int>(stream_[i]) << " ";
    }
    std::cout << std::endl << std::endl;
}
