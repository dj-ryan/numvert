#include <cstdio>
#include <cstring>
#include <iostream>
#include <math.h>
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
unsigned long long int hex2dec(string hex);
int bin2hex(uint32_t bin);
unsigned long long int bin2dec(string bin);
void print_char_array_as_num(char arr[], int len);
string dec2bin(int dec);

class Numvert {

public:
  vector<string> hex;
  vector<string> dec;
  vector<string> bin;

  bool printFullOutput;
  bool printOctOutput;
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
  }

private:
};

int main(int argc, char **argv) {

  args::ArgumentParser parser("Numvert", "Author: David Ryan\n~ALL TO HIM");
  args::Group parameters(
      parser, "Output formating options:", args::Group::Validators::DontCare);
  args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

  args::ValueFlagList<string> binaryInput(parser, "Binary", "Binary input",
                                          {'b', "bin"});
  args::ValueFlagList<int> decimalInput(parser, "Decimal", "Decimial input",
                                        {'i', "dec"});
  args::ValueFlagList<string> hexadecimalInput(
      parser, "Hexadecimal", "Hexadecimal input", {'h', "hex"});
  args::ValueFlag<string> octadeicmalInput(parser, "Octadecimal",
                                           "Octadecimal input", {'o', "oct"});

  args::Flag printFull(parameters, "Print Full", "Prints advanced output",
                       {'f', "full"});
  args::Flag printBinSpace4(parameters, "Print space 4",
                            "Prints spaces inbetween every fourth binary bit",
                            {"b4"});
  args::Flag printBinSpace8(parameters, "Print space 8",
                            "Prints spaces inbetween every byte", {"b8"});

  args::Flag signedBinInput(parameters, "Signed Binary Input",
                            "Marks all binary input as signed values", {"sb"});

  try {
    parser.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  } catch (args::ValidationError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  printf("+------------------------------------------------+\n\r");

  if (binaryInput) {
    for (string bin : args::get(binaryInput)) {
      printf("HEX : %s\n\r", "hello");
      printf("DEC : %lld\n\r", bin2dec(bin));
      printf("BIN : %s\n\r", bin.c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }
  if (decimalInput) {
    std::cout << "d: " << args::get(decimalInput)[0] << std::endl;
  }
  if (hexadecimalInput) {
    for (string hex : args::get(hexadecimalInput)) {

      printf("HEX : %s\n\r", hex.c_str());
      printf("DEC : %llu\n\r", hex2dec(hex));
      printf("BIN : %s\n\r", hex2bin(hex).c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }

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

  // hex conversion using std::hex
  // std::cout << "HEX: " << args::get(hexadecimal) << std::endl;
  // long long int decOutput;
  // std::stringstream ss;
  // ss << args::get(hexadecimal);
  // ss >> std::hex >> decOutput;
  // std::cout << "DEC: " << decOutput << std::endl;

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

unsigned long long int hex2dec(string hex) {

  long long int dec = 0;
  int pos = 1;
  int i = 0;
  while (hex[i]) {
    if (hex[i] >= '0' && hex[i] <= '9') {
      dec += (hex[i] - 48) * pos;
      pos *= 16;
    } else if (hex[i] >= 'a' && hex[i] <= 'f') {
      dec += (hex[i] - 87) * pos;
      pos *= 16;
    } else if (hex[i] >= 'A' && hex[i] <= 'F') {
      dec += (hex[i] - 55) * pos;
      pos *= 16;
    } else {
      return 0;
    }
    i++;
  }

  return dec;
}

int bin2hex(uint32_t bin) { return 10; }

unsigned long long int bin2dec(string bin) {
  unsigned long long int dec = 0;
  int pos = 0;
  int i = bin.length() - 1;
  while (i >= 0) {
    dec = dec + (bin[i] - 48) * (pow(2, pos));
    pos++;
    i--;
  }
  return dec;
}

string dec2bin(int dec) {
  int bin = 0, i = 0, rem;
  while (dec != 0) {
    rem = dec % 2;
    dec /= 10;
    bin += rem * pow(2, i);
    i++;
  }
}

void print_char_array_as_num(char arr[], int len) {
  int i;
  for (i = 0; i < len; i++) {
    printf("%u", arr[i] - 48);
  }
}
