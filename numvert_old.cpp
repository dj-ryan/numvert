#include <iostream>
#include <vector>
#include "args.hxx"

class Numvert
{

public:
    std::vector<std::string> hex;
    std::vector<std::string> dec;
    std::vector<std::string> bin;
    std::vector<std::string> oct;

    bool printFullOutput;
    bool printOctOutput;

private:
};

int main(int argc, char **argv)
{
    args::ArgumentParser parser("Numvert", "Author: David Ryan");
    args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

    args::ValueFlag<std::string> hexadecimal(parser, "hexadecimal", "Hexadecimal input flag", {'h', "hex"});

    args::ValueFlag<std::string> decimal(parser, "decimal", "Decimal input flag", {'d', "dec"});

    args::ValueFlag<std::string> binary(parser, "binary", "Binary input flag", {'b', "bin"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if (hexadecimal)
    {
        std::cout << "HEX: " << args::get(hexadecimal) << std::endl;
        long long int decOutput;
        std::stringstream ss;
        ss << args::get(hexadecimal);
        ss >> std::hex >> decOutput;
        std::cout << "DEC: " << decOutput << std::endl;





    }

    //     if (integer)
    //     {
    //         std::cout << "i: " << args::get(integer) << std::endl;
    //     }
    // if (characters)
    // {
    //     for (const auto ch : args::get(characters))
    //     {
    //         std::cout << "c: " << ch << std::endl;
    //     }
    // }
    // if (foo)
    // {
    //     std::cout << "f: " << args::get(foo) << std::endl;
    // }
    // if (numbers)
    // {
    //     for (const auto nm : args::get(numbers))
    //     {
    //         std::cout << "n: " << nm << std::endl;
    //     }
    // }
    return 0;
}

std::string dec2bin(int dec)
{
    int bin = 0, i = 0, rem;
    while (dec != 0)
    {
        rem = dec % 2;
        dec /= 10;
        bin += rem * pow(2, i);
        i++;
    }
}

std::string bin2dec(std::string bin)
{
}
