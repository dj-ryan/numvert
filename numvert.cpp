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

/**
 * TO HEX => %llx
      Caps
 * TO DEC => stoull
 * TO BIN => individual implementation
      Spaces
      Full
*/

unsigned long long int hex_to_dec(string hex);     // stoull
string hex_to_bin(string hex, int spacing_option); // While loop

// string dec_to_hex(unsigned long long int dec); // %llx
string dec_to_bin(unsigned long long int dec,
                  string bin); // TODO implement dec_to_bin, recursive function

// int bin_to_hex(uint32_t bin); // bin to dec with %llx
unsigned long long int bin_to_dec(string bin); // stoull
string bin_insert_space(string bin, int spacing_option);

int main(int argc, char **argv) {

  args::ArgumentParser parser("Numvert", "Author: David Ryan\n~ALL TO HIM");
  args::Group parameters(
      parser, "Output formating options:", args::Group::Validators::DontCare);
  args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

  args::ValueFlagList<string> hexadecimalInput(
      parser, "Hexadecimal", "Hexadecimal input (do not apply 0x prefix)",
      {'h', "hex"});
  args::ValueFlagList<unsigned long long int> decimalInput(
      parser, "Decimal", "Decimal input", {'i', 'd', "dec"});
  args::ValueFlagList<string> binaryInput(
      parser, "Binary", "Binary input (do not apply 0b prefix", {'b', "bin"});

  args::ValueFlag<string> octadeicmalInput(parser, "Octadecimal",
                                           "Octadecimal input", {'o', "oct"});

  args::Flag printFull(parameters, "Print Full", "Prints advanced output",
                       {'f', "full"});

  args::ValueFlag<int> printBinSpace(
      parameters, "4 or 8",
      "Prints spaces in between every forth binary bit or every byte", {"bs"});

  // args::Flag signedBinInput(parameters, "Signed Binary Input", "Marks all
  // binary input as signed values", {"sb"});

  args::Flag printCapHex(parameters, "Print Hex with Cap",
                         "Prints all hexadecimal output with capital letters",
                         {'H'});

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

      if (printCapHex) {
        printf("\tHEX : 0x%llX\n\r", hex_to_dec(hex));
      } else {
        printf("\tHEX : 0x%s\n\r", hex.c_str());
      }
      printf("\tDEC : %llu\n\r", hex_to_dec(hex));
      if (printBinSpace) {
        printf("\tBIN : %s\n\r",
               hex_to_bin(hex, args::get(printBinSpace)).c_str());
      } else {
        printf("\tBIN : 0b%s\n\r", hex_to_bin(hex, 0).c_str());
      }
      printf("+------------------------------------------------+\n\r");
      if (printFull) {
        printf("BIN : ");

        printf("+------------------------------------------------+\n\r");
      }
    }
  }
  if (decimalInput) {
    for (unsigned long long int dec : decimalInput) {

      printf("[%llu]\n\r", dec);
      if (printCapHex) {
        printf("\tHEX : 0x%llX\n\r", dec);
      } else {
        printf("\tHEX : 0x%llx\n\r", dec);
      }
      string bin = dec_to_bin(dec, "").c_str();

      printf("\tDEC : %llu\n\r", dec);

      if (printBinSpace) {
        printf("\tBIN : 0b%s\n\r", bin_insert_space(dec_to_bin(dec, "").c_str(),
                                                    args::get(printBinSpace))
                                       .c_str());
      } else {
        printf("\tBIN : 0b%s\n\r", dec_to_bin(dec, "").c_str());
      }

      printf("+------------------------------------------------+\n\r");
    }
  }
  if (binaryInput) {
    for (string bin : args::get(binaryInput)) {

      printf("[0b%s]\n\r", bin.c_str());
      if (printCapHex) {
        printf("\tHEX : 0x%llX\n\r", bin_to_dec(bin));
      } else {
        printf("\tHEX : 0x%llx\n\r", bin_to_dec(bin));
      }
      printf("\tDEC : %llu\n\r", bin_to_dec(bin));
      printf("\tBIN : 0b%s\n\r", bin.c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }
  return 0;
}

// TODO break binary string up

string hex_to_bin(string hex, int spacing_option) {
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

unsigned long long int hex_to_dec(string hex) {
  return stoull(hex, nullptr, 16);
}

string dec_to_bin(unsigned long long int dec, string bin) {
  if (dec > 1) {
    bin = dec_to_bin(dec / 2, bin);
  }
  bin.append(to_string(dec % 2));
  return bin;
}

string bin_insert_space(string bin, int spacing_option) {
  int spacer = 0;
  for (int i = bin.length(); i > 0; i--) {
    if (spacing_option == 4 && (spacer % 4) == 0) {
      bin.insert(i, " ");
    } else if (spacing_option == 8 && (spacer % 8) == 0) {
      bin.insert(i, " ");
    }
    spacer++;
  }
  return bin;
}

unsigned long long int bin_to_dec(string bin) {
  return stoull(bin, nullptr, 2);
}