#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "args.hxx"



/*
printf strings:

printf("+------------------------------------------------+\n\r");
printf("| ERROR: invalid tag '%s'\n\r", argv[i]);

printf("| -> 0b%s", argv[i+1]);






*/

using namespace std;

string hex2bin(string hex);

class Numvert {

private:
public:
  vector<string> hex;
  vector<string> dec;
  vector<string> bin;
  bool outputToOct;
  bool outputBinWithSpace;
  bool errorNoFlags;

  Numvert(vector<string> arguments) {
    for (string arg : arguments) {
      if (arg.at(0) == '-') {
        if (arg.at(1) == 'b') {

        } else if (arg.at(1) == 'd') {

        } else if (arg.at(1) == 'o') {

        } else if (arg.at(1) == 'h') {

        } else {

        }
      }
    }
  };
};

int main(int argc, char **argv) {

    args::ArgumentParser parser("Numvert", "Author: David Ryan\n~ALL TO HIM");
    args::Group parameters(parser, "Output options:", args::Group::Validators::DontCare);
    args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

    args::ValueFlag<string> binary(parser, "Binary", "Binary input", {'b', "bin"});
    args::ValueFlag<int> decimal(parser, "Decimal", "Decimial input", {'i', "dec"});
    args::ValueFlag<string> hexadecimal(parser, "Hexadecimal", "Hexadecimal input", {'h', "hex"});
    args::ValueFlag<string> octadeicmal(parser, "Octadecimal", "Octadecimal input", {'o', "oct"});

    args::Flag printFull(parameters, "Print Full", "Prints advanced output", {'f', "full"});


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

    if (binary) { std::cout << "b: " << args::get(binary) << std::endl; }
    if (decimal) { std::cout << "d: " << args::get(decimal) << std::endl; }
    if (hexadecimal) { std::cout << "h: " << args::get(hexadecimal) << std::endl; }


//   printf("+------------------------------------------------+\n\r");

//   for (int i = 0; i < argc; i++) {

//     // printf("Argument %d : %s\n", i, argv[i]);
//     if (argv[i][0] == '-') {

//       if (argv[i][1] == 'h') {

//         printf("| HEX: 0x%s\n\r", argv[i + 1]);
//         printf("| DEC: \n\r");
//         printf("| BIN: %s\n\r", hex2bin(argv[i + 1]).c_str());

//       } else if (argv[i][1] == 'd') {
//         printf("| HEX: 0x\n\r");
//         printf("| DEC: %s\n\r", argv[i + 1]);
//         printf("| BIN: 0b\n\r");

//       } else if (argv[i][1] == 'b') {
//         printf("| HEX: \n\r");
//         printf("| DEC: \n\r");
//         printf("| BIN: 0b%s\n\r", argv[i + 1]);

//       } else {
//         printf("| ERROR: invalid tag '%s'\n\r", argv[i]);
//       }
//       printf("+------------------------------------------------+\n\r");
//     }
//   }

  return 0;
}

string hex2bin(string hex) {
  string bin = "0b";

  int i = 0;
  while (hex[i]) {

    switch (hex[i]) {
    case '0':
      bin.append("0000");
      break;
    case '1':
      bin.append("0001");
      break;
    case '2':
      bin.append("0010");
      break;
    case '3':
      bin.append("0011");
      break;
    case '4':
      bin.append("0100");
      break;
    case '5':
      bin.append("0101");
      break;
    case '6':
      bin.append("0110");
      break;
    case '7':
      bin.append("0111");
      break;
    case '8':
      bin.append("1000");
      break;
    case '9':
      bin.append("1001");
      break;
    case 'A':
    case 'a':
      bin.append("1010");
      break;
    case 'B':
    case 'b':
      bin.append("1011");
      break;
    case 'C':
    case 'c':
      bin.append("1100");
      break;
    case 'D':
    case 'd':
      bin.append("1101");
      break;
    case 'E':
    case 'e':
      bin.append("1110");
      break;
    case 'F':
    case 'f':
      bin.append("1111");
      break;
    default:
      string returnError = "ERROR: unable to parse Hex, invalid character: \'";
      returnError = returnError + hex[i] + '\'';
      return returnError;
    }
    i++;
  }

  return bin;
}
