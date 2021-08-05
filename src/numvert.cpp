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
#include <exception>
#include <ios>
#include <math.h>
#include <ostream>
#include <stdexcept>
#include <stdio.h>

// local libs
#include "args.hxx"
#include "boost/format.hpp"

const std::string version = "v1.0.1";

unsigned long long int hex_to_dec(std::string hex);
std::string hex_to_bin(std::string hex);

std::string dec_to_bin(unsigned long long int dec, std::string bin);

unsigned long long int bin_to_dec(std::string bin);
std::string bin_insert_space(std::string bin, int spacing_option);

std::string dec_to_bin_set_spacing(unsigned long long int dec);

int main(int argc, char **argv) {

  /* Groups */
  args::ArgumentParser parser("Numvert", "Author: David Ryan\n~ ALL TO HIM");

  args::Group inputGroup(parser,
                         "Input types:", args::Group::Validators::AtLeastOne);

  args::Group outputFormatingGroup(
      parser, "Output formating options:", args::Group::Validators::DontCare);

  args::Group copyGroup(
      parser, "Copy to clipboard options:", args::Group::Validators::DontCare);

  args::Group otherOptionsGroup(
      parser, "Other options:", args::Group::Validators::DontCare);

  /* Input types */
  args::ValueFlagList<std::string> hexadecimalInputFlg(
      inputGroup, "Hexadecimal",
      "Hexadecimal input, case insensitive (do not apply 0x prefix)",
      {'h', "hex"});
  args::ValueFlagList<unsigned long long int> decimalInputFlg(
      inputGroup, "Decimal", "Decimal input", {'d', "dec", 'i', "int"});
  args::ValueFlagList<std::string> binaryInputFlg(
      inputGroup, "Binary", "Binary input (do not apply 0b prefix)",
      {'b', "bin"});

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

  /* Copy options */
  args::Flag copyHexFlg(copyGroup, "Copies hex output",
                        "Copies hexadecimal conversion output to clipboard",
                        {'x', "ch", "copy-hex", "copy-hexadecimal"});
  args::Flag copyDecFlg(copyGroup, "COpies dec output",
                        "Copies decimal conversion output to clipboard",
                        {'y', "cd", "copy-dec", "copy-decimal"});
  args::Flag copyBinFlg(copyGroup, "Copies bin output",
                        "Copies binary conversion output to clipboard",
                        {'z', "cb", "copy-bin", "copy-binary"});


  /* Other options */
  args::Flag versionFlg(otherOptionsGroup, "Version",
                        "Prints version information", {"version"});
  args::HelpFlag help(parser, "help", "Display this help menu", {'?', "help"});

  try {
    parser.ParseCLI(argc, argv);
  } catch (const args::Help &e) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  } catch (args::ValidationError &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  std::cout << boost::format(
                   "+---------------------------------------numvert--+")
            << std::endl;

  /************************
   * HEXADECIMAL INPUT
   *************************/
  if (hexadecimalInputFlg) {
    for (std::string hex : args::get(hexadecimalInputFlg)) {


      // unsigned long long int convertedDec = hex_to_dec(hex);
      // std::string convertedBin = hex_to_bin(hex);

      if (printCapHexFlg) {

        std::cout << boost::format("[%1%]") %
                         boost::io::group(std::hex, std::showbase,
                                          std::uppercase, hex_to_dec(hex))
                  << std::endl;

        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          std::uppercase, hex_to_dec(hex))
                  << std::endl;
      } else {

        std::cout << boost::format("[%1%]") % boost::io::group(std::hex,
                                                               std::showbase,
                                                               hex_to_dec(hex))
                  << std::endl;

        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          hex_to_dec(hex))
                  << std::endl;
      }

      std::cout << boost::format("\tDEC : %1%") %
                       boost::io::group(std::dec, hex_to_dec(hex))
                << std::endl;

      if (printBinSpaceFlg) {

        std::cout << boost::format("\tBIN : 0b%1%") %
                         bin_insert_space(hex_to_bin(hex),
                                          args::get(printBinSpaceFlg))
                  << std::endl;

      } else {
        std::cout << boost::format("\tBIN : 0b%1%") % hex_to_bin(hex)
                  << std::endl;

      }
      std::cout << boost::format(
                       "+------------------------------------------------+")
                << std::endl;
    }
  }

  /************************
   * DECIMAL INPUT
   *************************/
  if (decimalInputFlg) {
    for (unsigned long long int dec : decimalInputFlg) {

      std::cout << boost::format("[%1%]") % dec << std::endl;

      if (printCapHexFlg) {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          std::uppercase, dec)
                  << std::endl;

      } else {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase, dec)
                  << std::endl;
      }

      std::cout << boost::format("\tDEC : %1%") % dec << std::endl;

      std::string bin;

      if (truncateBinFlg) // truncating bin option
      {
        bin = dec_to_bin(dec, "");
      } else {
        bin = dec_to_bin_set_spacing(dec);
      }

      if (printBinSpaceFlg) // bin spacing option
      {
        std::cout << boost::format("\tBIN : 0b%1%") %
                         bin_insert_space(bin, args::get(printBinSpaceFlg))
                  << std::endl;

      } else {
        std::cout << boost::format("\tBIN : 0b%1%") % bin << std::endl;
      }

      std::cout << boost::format(
                       "+------------------------------------------------+")
                << std::endl;
    }
  }

  /************************
   * BINARY INPUT
   *************************/
  if (binaryInputFlg) {
    for (std::string bin : args::get(binaryInputFlg)) {

      std::cout << boost::format("[0b%1%]") % bin << std::endl;

      if (printCapHexFlg) {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          std::uppercase, bin_to_dec(bin))
                  << std::endl;

      } else {
        std::cout << boost::format("\tHEX : %1%") %
                         boost::io::group(std::hex, std::showbase,
                                          bin_to_dec(bin))
                  << std::endl;
      }

      std::cout << boost::format("\tDEC : %1%") % bin_to_dec(bin) << std::endl;

      std::cout << boost::format("\tBIN : 0b%1%") % bin << std::endl;

      std::cout << boost::format(
                       "+------------------------------------------------+")
                << std::endl;
    }
  }

  if (versionFlg) {
    std::cout << boost::format("version: %1%") % version << std::endl;

    std::cout << boost::format(
                     "+------------------------------------------------+")
              << std::endl;
  }

  return 0;
}

std::string hex_to_bin(std::string hex) {
  std::string bin = "";
  int i = 0;
  try {
    while (hex.at(i)) {
      switch (hex.at(i)) {
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
  } catch (const std::exception &e) {
    return bin;
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