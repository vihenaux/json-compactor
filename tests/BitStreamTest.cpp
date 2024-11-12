#include <iostream>
#include <string>
#include "../src/bitstream.hpp"

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

    s.push(std::string("short str"));
    s.push(std::string("not so short str"));
    s.push(std::string("A very long string to show that the BitStream class is able to store any sizes of strings. But this is not nearly enough to prove that still. Therefore I need to continue writing sentences until I hit the 256 char mark. We a bit past 200 right now, just a bit more. And that should be it."));

    return 0;
}
