#include <iostream>
#include "../src/bitstream.hpp"
#include <fstream>

int main()
{
    BitStream s;

    s.push(true);
    s.push(true);
    s.push(false);

    s.push('A');
    s.push('b');
    s.push('C');
    s.push('0');

    s.push(static_cast<uint8_t>(0));
    s.push(static_cast<uint8_t>(113));
    s.push(static_cast<uint8_t>(255));

    s.push(1.f);
    s.push(.0004f);
    s.push(10000.f);
    s.push(12347.345f);

    s.push(1.);
    s.push(234.456);
    s.push(.148456946);
    s.push(1985785.4562);

    s.push(1ull);
    s.push(0ull);
    s.push(256ull);
    s.push(25634545ull);
    s.push(8923345876ull);

    for(unsigned long long i(1); i < 1000000000; i*=2)
    {
        s.push(i);
    }

    s.push(std::string("short str"));
    s.push(std::string("not so short str"));
    s.push(std::string("A very long string to show that the BitStream class is able to store any sizes of strings. But this is not nearly enough to prove that still. Therefore I need to continue writing sentences until I hit the 256 char mark. We a bit past 200 right now, just a bit more. And that should be it."));

    s.push_flt_str("1345.86562");

    if(!s.get_bool())
        std::cerr << "\033[31mError 1" << std::endl;
    if(!s.get_bool())
        std::cerr << "\033[31mError 2" << std::endl;
    if(s.get_bool())
        std::cerr << "\033[31mError 3" << std::endl;
    if(s.get_char() != 'A')
        std::cerr << "\033[31mError 4" << std::endl;
    if(s.get_char() != 'b')
        std::cerr << "\033[31mError 5" << std::endl;
    if(s.get_char() != 'C')
        std::cerr << "\033[31mError 6" << std::endl;
    if(s.get_char() != '0')
        std::cerr << "\033[31mError 7" << std::endl;
    if(s.get_uint8() != 0)
        std::cerr << "\033[31mError 8" << std::endl;
    if(s.get_uint8() != 113)
        std::cerr << "\033[31mError 9" << std::endl;
    if(s.get_uint8() != 255)
        std::cerr << "\033[31mError 10" << std::endl;
    if(s.get_float() != 1.f)
        std::cerr << "\033[31mError 11" << std::endl;
    if(s.get_float() != .0004f)
        std::cerr << "\033[31mError 12" << std::endl;
    if(s.get_float() != 10000.f)
        std::cerr << "\033[31mError 13" << std::endl;
    if(s.get_float() != 12347.345f)
        std::cerr << "\033[31mError 14" << std::endl;
    if(s.get_double() != 1.)
        std::cerr << "\033[31mError 15" << std::endl;
    if(s.get_double() != 234.456)
        std::cerr << "\033[31mError 16" << std::endl;
    if(s.get_double() != .148456946)
        std::cerr << "\033[31mError 17" << std::endl;
    if(s.get_double() != 1985785.4562)
        std::cerr << "\033[31mError 18" << std::endl;
    if(s.get_int() != 1ull)
        std::cerr << "\033[31mError 19" << std::endl;
    if(s.get_int() != 0ull)
        std::cerr << "\033[31mError 20" << std::endl;
    if(s.get_int() != 256ull)
        std::cerr << "\033[31mError 21" << std::endl;
    if(s.get_int() != 25634545ull)
        std::cerr << "\033[31mError 22" << std::endl;
    if(s.get_int() != 8923345876ull)
        std::cerr << "\033[31mError 23" << std::endl;

    for(unsigned long long i(1); i < 1000000000; i*=2)
    {
        unsigned long long x = s.get_int();
        if(i != x)
        {
            std::cerr << "\033[31mError 24 (" << i << " != " << x << ")" << std::endl;
        }
    }

    if(s.get_str() != std::string("short str"))
        std::cerr << "\033[31mError 25" << std::endl;
    if(s.get_str() != std::string("not so short str"))
        std::cerr << "\033[31mError 26" << std::endl;
    if(s.get_str() != std::string("A very long string to show that the BitStream class is able to store any sizes of strings. But this is not nearly enough to prove that still. Therefore I need to continue writing sentences until I hit the 256 char mark. We a bit past 200 right now, just a bit more. And that should be it."))
        std::cerr << "\033[31mError 27" << std::endl;

    if(s.get_flt_str() != "1345.86562")
        std::cerr << "\033[31mError 28" << std::endl;

    BitStream to_save;
    to_save.push(1ull);
    to_save.save("tmp_save.bin");

    BitStream to_read("tmp_save.bin");
    if(to_read.get_int() != 1ull)
        std::cerr << "\033[31mError 24" << std::endl;

    return 0;
}
