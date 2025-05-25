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
    bool contains_value_0 = false;
    bool contains_ALPHA = false;
    bool contains_alpha = false;
    bool contains_point = false;
    bool contains_num = false;
    bool contains_punctuation = false;
    bool contains_special = false;
    bool contains_other = false;

    for(unsigned int i(0); i < value.size(); ++i)
    {
        switch(value[i])
        {
            case '\0':
                contains_value_0 = true;
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                contains_ALPHA = true;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
                contains_alpha = true;
                break;
            case '.':
                contains_point = true;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                contains_num = true;
                break;
            case '!':
            case '?':
            case ':':
            case ';':
            case ',':
            case '"':
            case '-':
                contains_punctuation = true;
                break;
            case '&':
            case '~':
            case '#':
            case '\'':
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case '_':
            case '|':
            case '+':
            case '=':
            case '\\':
            case '/':
            case '*':
            case '%':
                contains_special = true;
                break;
            default:
                contains_other = true;
                break;
        }
    }

    if(contains_other)
    {
        contains_point = false;
        contains_ALPHA = false;
        contains_alpha = false;
        contains_num = false;
        contains_punctuation = false;
        contains_special = false;
    }
    if(contains_point && ! contains_num)
    {
        contains_punctuation = true;
    }

    push(contains_value_0);
    push(contains_ALPHA);
    push(contains_alpha);
    push(contains_num);
    push(contains_punctuation);
    push(contains_special);

    if(contains_value_0)
    {
        push(static_cast<unsigned long long>(value.size()));
    }

    if(!contains_ALPHA && !contains_alpha && !contains_num && !contains_punctuation && !contains_special)
    {
        for(unsigned long long i(0); i < value.size(); ++i)
        {
            push(value[i]);
        }

        if(!contains_value_0)
        {
            push('\0');
        }
    }
    else
    {
        std::vector<char> char_set({'\0'});
        if(contains_ALPHA)
        {
            char_set.insert(char_set.end(), {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'});
        }
        if(contains_alpha)
        {
            char_set.insert(char_set.end(), {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'});
        }
        if(contains_num)
        {
            char_set.insert(char_set.end(), {'0','1','2','3','4','5','6','7','8','9','.'});
        }
        if(contains_punctuation)
        {
            char_set.insert(char_set.end(), {'!','.','?',':',';',',','"'});
        }
        if(contains_special)
        {
            char_set.insert(char_set.end(), {'&','~','#','\'','{','}','(',')','[',']','_','|','+','=','\\','/','*','%','-'});
        }

        auto push_next_char_index = [this, &char_set](unsigned int index)
        {
            if(char_set.size() <= 4)
            {
                this->push_2_int(index);
            }
            else if(char_set.size() <= 8)
            {
                this->push_3_int(index);
            }
            else if(char_set.size() <= 16)
            {
                this->push_4_int(index);
            }
            else if(char_set.size() <= 32)
            {
                this->push_5_int(index);
            }
            else if(char_set.size() <= 64)
            {
                this->push_6_int(index);
            }
            else if(char_set.size() <= 128)
            {
                this->push_7_int(index);
            }
        };

        for(unsigned int i(0); i < value.size(); ++i)
        {
            for(unsigned int j(0); j < char_set.size(); ++j)
            {
                if(char_set[j] == value[i])
                {
                    push_next_char_index(j);
                }
            }
        }
        push_next_char_index(0);
    }
}

void BitStream::push_2_int(unsigned int value)
{
    push(static_cast<bool>((value >> 1)%2));
    push(static_cast<bool>(value%2));
}

void BitStream::push_3_int(unsigned int value)
{
    push(static_cast<bool>((value >> 2)%2));
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

void BitStream::push_5_int(unsigned int value)
{
    push(static_cast<bool>((value >> 4)%2));
    push(static_cast<bool>((value >> 3)%2));
    push(static_cast<bool>((value >> 2)%2));
    push(static_cast<bool>((value >> 1)%2));
    push(static_cast<bool>((value)%2));
}

void BitStream::push_6_int(unsigned int value)
{
    push(static_cast<bool>((value >> 5)%2));
    push(static_cast<bool>((value >> 4)%2));
    push(static_cast<bool>((value >> 3)%2));
    push(static_cast<bool>((value >> 2)%2));
    push(static_cast<bool>((value >> 1)%2));
    push(static_cast<bool>((value)%2));
}

void BitStream::push_7_int(unsigned int value)
{
    push(static_cast<bool>((value >> 6)%2));
    push(static_cast<bool>((value >> 5)%2));
    push(static_cast<bool>((value >> 4)%2));
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

void BitStream::push_flt_str(std::string const & value)
{
    for(unsigned int i(0); i < value.size(); ++i)
    {
        if(value[i] == '.')
        {
            push_4_int(10ul);
        }
        else
        {
            push_4_int(static_cast<unsigned int>(value[i] - '0'));
        }
    }
    push_4_int(15);
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

std::string BitStream::get_flt_str()
{
    std::string res;
    unsigned int tmp = get_4_int();
    while(tmp != 15)
    {
        if(tmp == 10)
        {
            res.push_back('.');
        }
        else
        {
            res.push_back(static_cast<char>('0' + tmp));
        }
        tmp = get_4_int();
    }
    return res;
}

unsigned int BitStream::get_2_int()
{
    return (static_cast<unsigned int>(get_bool()) << 1) + static_cast<unsigned int>(get_bool());
}

unsigned int BitStream::get_3_int()
{
    return (static_cast<unsigned int>(get_bool()) << 2) + (static_cast<unsigned int>(get_bool()) << 1) + static_cast<unsigned int>(get_bool());
}

unsigned int BitStream::get_4_int()
{
    unsigned int x = static_cast<unsigned int>(get_bool()) << 3;
    x += static_cast<unsigned int>(get_bool()) << 2;
    x += static_cast<unsigned int>(get_bool()) << 1;
    x += static_cast<unsigned int>(get_bool());

    return x;
}

unsigned int BitStream::get_5_int()
{
    unsigned int x = static_cast<unsigned int>(get_bool()) << 4;
    x += static_cast<unsigned int>(get_bool()) << 3;
    x += static_cast<unsigned int>(get_bool()) << 2;
    x += static_cast<unsigned int>(get_bool()) << 1;
    x += static_cast<unsigned int>(get_bool());

    return x;
}

unsigned int BitStream::get_6_int()
{
    unsigned int x = static_cast<unsigned int>(get_bool()) << 5;
    x += static_cast<unsigned int>(get_bool()) << 4;
    x += static_cast<unsigned int>(get_bool()) << 3;
    x += static_cast<unsigned int>(get_bool()) << 2;
    x += static_cast<unsigned int>(get_bool()) << 1;
    x += static_cast<unsigned int>(get_bool());

    return x;
}

unsigned int BitStream::get_7_int()
{
    unsigned int x = static_cast<unsigned int>(get_bool()) << 6;
    x += static_cast<unsigned int>(get_bool()) << 5;
    x += static_cast<unsigned int>(get_bool()) << 4;
    x += static_cast<unsigned int>(get_bool()) << 3;
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
    std::string str;

    bool contains_value_0 = get_bool();
    bool contains_ALPHA = get_bool();
    bool contains_alpha = get_bool();
    bool contains_num = get_bool();
    bool contains_punctuation = get_bool();
    bool contains_special = get_bool();
    unsigned long long str_size = 0;

    if(contains_value_0)
    {
        str_size = get_int();
    }

    if(!contains_ALPHA && !contains_alpha && !contains_num &&
        !contains_punctuation && ! contains_special)
    {
        if(contains_value_0)
        {
            for(unsigned long long i(0); i < str_size; ++i)
            {
                str.push_back(get_char());
            }
        }
        else
        {
            char tmp = get_char();
            while(tmp != '\0')
            {
                str.push_back(tmp);
                tmp = get_char();
            }
        }
    }
    else
    {
        std::vector<char> char_set({'\0'});
        if(contains_ALPHA)
        {
            char_set.insert(char_set.end(), {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'});
        }
        if(contains_alpha)
        {
            char_set.insert(char_set.end(), {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'});
        }
        if(contains_num)
        {
            char_set.insert(char_set.end(), {'0','1','2','3','4','5','6','7','8','9','.'});
        }
        if(contains_punctuation)
        {
            char_set.insert(char_set.end(), {'!','.','?',':',';',',','"','-'});
        }
        if(contains_special)
        {
            char_set.insert(char_set.end(), {'&','~','#','\'','{','}','(',')','[',']','_','|','+','=','\\','/','*','%'});
        }

        auto get_next_char = [this, &char_set]() -> char
        {
            if(char_set.size() <= 4)
            {
                return char_set[get_2_int()];
            }
            else if(char_set.size() <= 8)
            {
                return char_set[get_3_int()];
            }
            else if(char_set.size() <= 16)
            {
                return char_set[get_4_int()];
            }
            else if(char_set.size() <= 32)
            {
                return char_set[get_5_int()];
            }
            else if(char_set.size() <= 64)
            {
                return char_set[get_6_int()];
            }
            else if(char_set.size() <= 128)
            {
                return char_set[get_7_int()];
            }

            return char_set[static_cast<unsigned int>(get_char())];
        };

        if(contains_value_0)
        {
            for(unsigned long long i(0); i < str_size; ++i)
            {
                str.push_back(get_next_char());
            }
        }
        else
        {
            char tmp = get_next_char();
            while(tmp != '\0')
            {
                str.push_back(tmp);
                tmp = get_next_char();
            }
        }
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
