#include "args.hxx"
#include <bitset>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

string hex_to_bin_with_space(string hex, int spacing_option);
int bin_to_hex(uint32_t bin);
void print_char_array_as_num(char arr[], int len);
string dec_to_bin(int dec);

unsigned long long int bin_to_int(string bin);
unsigned long long int hex_to_int(string hex);

string dec_to_bin_with_spaces(unsigned long long int dec);
string dec_to_hex(unsigned long long int dec);

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

  args::ValueFlagList<string> hexadecimalInput(
      parser, "Hexadecimal", "Hexadecimal input", {'h', "hex"});
  args::ValueFlagList<unsigned long long int> decimalInput(
      parser, "Decimal", "Decimial input", {'i', 'd', "dec"});
  args::ValueFlagList<string> binaryInput(parser, "Binary", "Binary input",
                                          {'b', "bin"});

  args::ValueFlag<string> octadeicmalInput(parser, "Octadecimal",
                                           "Octadecimal input", {'o', "oct"});

  args::Flag printFull(parameters, "Print Full", "Prints advanced output",
                       {'f', "full"});

  args::ValueFlag<int> printBinSpace(
      parameters, "4 or 8",
      "Prints spaces inbetween every forth binary bit or every byte",
      {'s'}); // TODO input validation

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
  if (hexadecimalInput) {
    for (string hex : args::get(hexadecimalInput)) {

      printf("[0x%s]\n\r", hex.c_str());

      printf("\tHEX : 0x%s\n\r", hex.c_str());
      printf("\tDEC : %llu\n\r", hex_to_int(hex));
      if (printBinSpace) {
        printf("\tBIN : %s\n\r",
               hex_to_bin_with_space(hex, args::get(printBinSpace)).c_str());
      } else {
        printf("\tBIN : %s\n\r", hex_to_bin_with_space(hex, 0).c_str());
      }
      printf("+------------------------------------------------+\n\r");
    }
  }
  if (decimalInput) {
    for (unsigned long long int dec : decimalInput) {

      printf("[%llu]\n\r", dec);
      printf("\tHEX : %llx\n\r", dec);
      printf("\tDEC : %llu\n\r", dec);
      printf("\tBIN : %s\n\r", dec_to_bin_with_spaces(dec).c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }
  if (binaryInput) {
    for (string bin : args::get(binaryInput)) {

      printf("[0b%s]\n\r", bin.c_str());
      printf("\tHEX : %s\n\r", "hello");
      printf("\tDEC : %llu\n\r", bin_to_int(bin));
      printf("\tBIN : %s\n\r", bin.c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }
  return 0;
}

// TODO break binary string up

string hex_to_bin_with_space(string hex, int spacing_option) {
  string bin = "";
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
    // append propper spacing
    if (spacing_option == 4) {
      bin.append(" ");
    } else if (spacing_option == 8) {
      if (i % 2 == 0) {
        bin.append(" ");
      }
    }
  }

  return bin;
}

int bin_to_hex(uint32_t bin) { return 1; }

unsigned long long int bin_to_int(string bin) {
  return stoull(bin, nullptr, 2);
}

unsigned long long int hex_to_int(string hex) {
  return stoull(hex, nullptr, 16);
}

string dec_to_bin_with_spaces(unsigned long long int dec) {  
  bitset<8> b(dec);
  return b.to_string();
}

string dec_to_hex(unsigned long long int dec) {
  stringstream stream;
  // stream << setfill('0') << setw(sizeof(dec) * 2) << hex << dec;
  stream << hex << dec;
  return stream.str();
}


