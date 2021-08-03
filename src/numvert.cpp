// #include <cstdint>
// #include <cstdio>
// #include <cstring>
// #include <iomanip>
// #include <iostream>

// #include <string>
// #include <vector>

#include <bitset>
#include <boost/format/format_fwd.hpp>
#include <boost/format/group.hpp>
#include <ios>
#include <math.h>
#include <ostream>
#include <stdio.h>

// local libs
#include "args.hxx"
#include "boost/format.hpp"

const std::string version = "v1.0.1";

unsigned long long int hex_to_dec(std::string hex); // stoull
std::string hex_to_bin(std::string hex);            // While loop

std::string dec_to_bin(unsigned long long int dec, std::string bin);

unsigned long long int bin_to_dec(std::string bin); // stoull
std::string bin_insert_space(std::string bin, int spacing_option);

std::string dec_to_bin_set_spacing(unsigned long long int dec);

int main(int argc, char **argv) {

  args::ArgumentParser parser("Numvert", "Author: David Ryan\n~ ALL TO HIM");

  args::Group inputGroup(
      parser, "Input types:",
      args::Group::Validators::AtLeastOne); // TODO: add group validation for
                                            // input peramaters

  args::Group outputFormatingGroup(
      parser, "Output formating options:", args::Group::Validators::DontCare);

  args::Group otherOptionsGroup(
      parser, "Other options:", args::Group::Validators::DontCare);

  args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

  args::ValueFlagList<std::string> hexadecimalInputFlg(
      inputGroup, "Hexadecimal", "Hexadecimal input (do not apply 0x prefix)",
      {'h', "hex"});
  args::ValueFlagList<unsigned long long int> decimalInputFlg(
      inputGroup, "Decimal", "Decimal input", {'d', "dec", 'i', "int"});
  args::ValueFlagList<std::string> binaryInputFlg(
      inputGroup, "Binary", "Binary input (do not apply 0b prefix)",
      {'b', "bin"});

  // args::ValueFlag<string> octalDeicmalInput(parser, "Octal Decimal",
  //                                          "Octal Decimal input", {'o',
  //                                          "oct"}); // TODO: write Octal
  //                                          Decimal converters

  /* Output Formating Group */
  args::Flag printFullFlg(outputFormatingGroup, "Print Full",
                          "Prints advanced output", {'f', "full"});

  args::ValueFlag<int> printBinSpaceFlg(
      outputFormatingGroup, "4 or 8",
      "Prints spaces in between every forth binary bit or every byte",
      {'s', "binary-spacing"});

  args::Flag truncateBinFlg(outputFormatingGroup, "Truncate Binary",
                            "Truncates binary output to least significant '1'",
                            {"tb", "truncate-binary"});

  args::Flag printCapHexFlg(outputFormatingGroup, "Print Hex with Cap",
                            "Prints all hexadecimal output with capital "
                            "letters (can not be used as input)",
                            {'H', "cap-hex"});

  /* Other options */

  args::Flag versionFlg(otherOptionsGroup, "Version",
                        "Prints version information", {"version"});

  // args::Flag signedBinInput(otherOptionsGroup, "Signed Binary Input", "Marks
  // all binary input as signed values", {"sb"}); // TODO: signed binary math

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

  // testing
  // unsigned long long int temp = 3245782638762346785;
  // std::cout << boost::format("%x %d") % temp % 77 << std::endl;
  // // long version
  // std::cout << boost::format("%1%") %
  //                  boost::io::group(std::hex, std::showbase, 40)
  //           << std::endl;
  // // shorthand
  // std::cout << boost::format("%1$#x") % 40 << std::endl;

  std::cout << boost::format(
                   "+---------------------------------------numvert--+")
            << std::endl;

  /************************
   * HEXADECIMAL INPUT
   *************************/
  if (hexadecimalInputFlg) {
    for (std::string hex : args::get(hexadecimalInputFlg)) {

      std::cout << boost::format("[%1%]") % hex << std::endl;
      // printf("[0x%s]\n\r", hex.c_str());

      if (printCapHexFlg) {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          std::uppercase, hex_to_dec(hex))
                  << std::endl;
        // printf("\tHEX : 0x%llX\n\r", hex_to_dec(hex));
      } else {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          hex_to_dec(hex))
                  << std::endl;
        // std::cout << boost::format("\tHEX : %1%") % hex << std::endl;
        // printf("\tHEX : 0x%s\n\r", hex.c_str());
      }

      std::cout << boost::format("\tDEC : %1%") %
                       boost::io::group(std::dec, hex_to_dec(hex))
                << std::endl;
      // printf("\tDEC : %llu\n\r", hex_to_dec(hex));

      if (printBinSpaceFlg) {

        std::cout << boost::format("\tBIN : 0b%1%") %
                         bin_insert_space(hex_to_bin(hex),
                                          args::get(printBinSpaceFlg))
                  << std::endl;
        // printf("\tBIN : 0b%s\n\r",
        //        bin_insert_space(hex_to_bin(hex), args::get(printBinSpaceFlg))
        //            .c_str());
      } else {
        std::cout << boost::format("\tBIN : 0b%1%") % hex_to_bin(hex)
                  << std::endl;
        // printf("\tBIN : 0b%s\n\r", hex_to_bin(hex).c_str());
      }
      std::cout << boost::format(
                       "+------------------------------------------------+")
                << std::endl;
      // printf("+------------------------------------------------+\n\r");
    }
  }

  /************************
   * DECIMAL INPUT
   *************************/
  if (decimalInputFlg) {
    for (unsigned long long int dec : decimalInputFlg) {

      printf("[%llu]\n\r", dec);
      if (printCapHexFlg) {
        printf("\tHEX : 0x%llX\n\r", dec);
      } else {
        printf("\tHEX : 0x%llx\n\r", dec);
      }
      // string bin = dec_to_bin(dec, "").c_str();

      printf("\tDEC : %llu\n\r", dec);

      std::string bin;

      if (truncateBinFlg) // truncating bin option
      {
        bin = dec_to_bin(dec, "");
      } else {
        bin = dec_to_bin_set_spacing(dec);
      }

      if (printBinSpaceFlg) // bin spacing option
      {
        printf("\tBIN : 0b%s\n\r",
               bin_insert_space(bin, args::get(printBinSpaceFlg)).c_str());
      } else {
        printf("\tBIN : 0b%s\n\r", bin.c_str());
      }

      printf("+------------------------------------------------+\n\r");
    }
  }

  /************************
   * BINARY INPUT
   *************************/
  if (binaryInputFlg) {
    for (std::string bin : args::get(binaryInputFlg)) {

      printf("[0b%s]\n\r", bin.c_str());
      if (printCapHexFlg) {
        printf("\tHEX : 0x%llX\n\r", bin_to_dec(bin));
      } else {
        printf("\tHEX : 0x%llx\n\r", bin_to_dec(bin));
      }
      printf("\tDEC : %llu\n\r", bin_to_dec(bin));
      printf("\tBIN : 0b%s\n\r", bin.c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }

  if (versionFlg) {
    printf("version: %s\n\r", version.c_str());
    printf("+------------------------------------------------+\n\r");
  }

  if (argc <= 1) {
    std::cout << parser;
  }

  return 0;
}

/************************
 * Function Deffinitions
 *************************/

std::string hex_to_bin(std::string hex) {
  std::string bin = "";
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
      std::string returnError =
          "ERROR: unable to parse Hex, invalid character: \'";
      returnError = returnError + hex[i] + '\'';
      return returnError;
    }
    i++;
  }

  return bin;
}

unsigned long long int hex_to_dec(std::string hex) {
  return stoull(hex, nullptr, 16);
}

std::string dec_to_bin(unsigned long long int dec, std::string bin) {
  if (dec > 1) {
    bin = dec_to_bin(dec / 2, bin);
  }
  bin.append(std::to_string(dec % 2));
  return bin;
}

std::string bin_insert_space(std::string bin, int spacing_option) {
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

unsigned long long int bin_to_dec(std::string bin) {
  return stoull(bin, nullptr, 2);
}

std::string dec_to_bin_set_spacing(unsigned long long int dec) {
  if (dec <= 16) {
    std::bitset<4> bin(dec);
    return bin.to_string();
  } else if (dec <= 256) {
    std::bitset<8> bin(dec);
    return bin.to_string();
  } else if (dec <= 4096) {
    std::bitset<12> bin(dec);
    return bin.to_string();
  } else if (dec <= 65536) {
    std::bitset<16> bin(dec);
    return bin.to_string();
  } else if (dec <= 1048576) {
    std::bitset<20> bin(dec);
    return bin.to_string();
  } else if (dec <= 16777216) {
    std::bitset<24> bin(dec);
    return bin.to_string();
  } else if (dec <= 268435456) {
    std::bitset<28> bin(dec);
    return bin.to_string();
  } else if (dec <= 4294967296) {
    std::bitset<432> bin(dec);
    return bin.to_string();
  }

  std::bitset<64> bin(dec);
  return bin.to_string();
}

// TODO: dec and hex to binary with set length

/************************
 * DEPRECATED FUNCTIONS
 * (these get optimized out during compilation)
 *************************/

unsigned long long int hex2dec(std::string hex) {
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

unsigned long long int bin2dec(std::string bin) {

  return stoull(bin, nullptr, 2);

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

std::string dec_to_bin_hard_value(int dec) {
  int bin = 0, i = 0, rem;
  while (dec != 0) {
    rem = dec % 2;
    dec /= 10;
    bin += rem * pow(2, i);
    i++;
  }
  return "hello";
}

std::string dec_to_hex_hard_value(unsigned long long int dec) {
  std::stringstream stream;
  stream << std::hex << dec;
  return stream.str();
}
