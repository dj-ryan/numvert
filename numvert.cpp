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
                  string bin);

// int bin_to_hex(uint32_t bin); // bin to dec with %llx
unsigned long long int bin_to_dec(string bin); // stoull
string bin_insert_space(string bin, int spacing_option);

string dec_to_bin_set_spacing(unsigned long long int dec);

int main(int argc, char **argv)
{

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

  // args::ValueFlag<string> octalDeicmalInput(parser, "Octal Decimal",
  //                                          "Octal Decimal input", {'o', "oct"}); // TODO: write Octal Decimal converters

  args::Flag printFull(parameters, "Print Full", "Prints advanced output",
                       {'f', "full"});

  args::ValueFlag<int> printBinSpace(
      parameters, "4 or 8",
      "Prints spaces in between every forth binary bit or every byte", {'s', "binary-spacing"});

  args::Flag truncateBin(parameters, "Truncate Binary", "Truncates binary output to least significant '1'", {"tb", "truncate-binary"});

  // args::Flag signedBinInput(parameters, "Signed Binary Input", "Marks all
  // binary input as signed values", {"sb"}); // TODO: signed binary math

  args::Flag printCapHex(parameters, "Print Hex with Cap",
                         "Prints all hexadecimal output with capital letters",
                         {'H', "cap-hex"});

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

  printf("+------------------------------------------------+\n\r");
  if (hexadecimalInput)
  {
    for (string hex : args::get(hexadecimalInput))
    {

      printf("[0x%s]\n\r", hex.c_str());

      if (printCapHex)
      {
        printf("\tHEX : 0x%llX\n\r", hex_to_dec(hex));
      }
      else
      {
        printf("\tHEX : 0x%s\n\r", hex.c_str());
      }
      printf("\tDEC : %llu\n\r", hex_to_dec(hex));

      if (printBinSpace)
      {
        printf("\tBIN : 0b%s\n\r",
               hex_to_bin(hex, args::get(printBinSpace)).c_str());
      }
      else
      {
        printf("\tBIN : 0b%s\n\r", hex_to_bin(hex, 0).c_str());
      }
      printf("+------------------------------------------------+\n\r");
    }
  }
  if (decimalInput)
  {
    for (unsigned long long int dec : decimalInput)
    {

      printf("[%llu]\n\r", dec);
      if (printCapHex)
      {
        printf("\tHEX : 0x%llX\n\r", dec);
      }
      else
      {
        printf("\tHEX : 0x%llx\n\r", dec);
      }
      //string bin = dec_to_bin(dec, "").c_str();

      printf("\tDEC : %llu\n\r", dec);

      string bin;

      if (truncateBin) // truncating bin option
      {
        bin = dec_to_bin(dec, "");
      }
      else
      {
        bin = dec_to_bin_set_spacing(dec);
      }

      if (printBinSpace) // bin spacing option
      {
        printf("\tBIN : 0b%s\n\r", bin_insert_space(bin,
                                                    args::get(printBinSpace))
                                       .c_str());
      }
      else
      {
        printf("\tBIN : 0b%s\n\r", bin.c_str());
      }

      printf("+------------------------------------------------+\n\r");
    }
  }
  if (binaryInput)
  {
    for (string bin : args::get(binaryInput))
    {

      printf("[0b%s]\n\r", bin.c_str());
      if (printCapHex)
      {
        printf("\tHEX : 0x%llX\n\r", bin_to_dec(bin));
      }
      else
      {
        printf("\tHEX : 0x%llx\n\r", bin_to_dec(bin));
      }
      printf("\tDEC : %llu\n\r", bin_to_dec(bin));
      printf("\tBIN : 0b%s\n\r", bin.c_str());

      printf("+------------------------------------------------+\n\r");
    }
  }
  return 0;
}

string hex_to_bin(string hex, int spacing_option)
{
  string bin = "";
  int i = 0;
  while (hex[i])
  {
    switch (hex[i])
    {
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

    if (spacing_option == 4)
    {
      bin.append(" ");
    }
    else if (spacing_option == 8)
    {
      if (i % 2 == 0)
      {
        bin.append(" ");
      }
    }
  }

  return bin;
}

unsigned long long int hex_to_dec(string hex)
{
  return stoull(hex, nullptr, 16);
}

string dec_to_bin(unsigned long long int dec, string bin)
{
  if (dec > 1)
  {
    bin = dec_to_bin(dec / 2, bin);
  }
  bin.append(to_string(dec % 2));
  return bin;
}

string bin_insert_space(string bin, int spacing_option)
{
  int spacer = 0;
  for (int i = bin.length(); i > 0; i--)
  {
    if (spacing_option == 4 && (spacer % 4) == 0)
    {
      bin.insert(i, " ");
    }
    else if (spacing_option == 8 && (spacer % 8) == 0)
    {
      bin.insert(i, " ");
    }
    spacer++;
  }
  return bin;
}

unsigned long long int bin_to_dec(string bin)
{
  return stoull(bin, nullptr, 2);
}

string dec_to_bin_set_spacing(unsigned long long int dec)
{
  if (dec <= 16)
  {
    bitset<4> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 256)
  {
    bitset<8> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 4096)
  {
    bitset<12> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 65536)
  {
    bitset<16> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 1048576)
  {
    bitset<20> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 16777216)
  {
    bitset<24> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 268435456)
  {
    bitset<28> bin(dec);
    return bin.to_string();
  }
  else if (dec <= 4294967296)
  {
    bitset<432> bin(dec);
    return bin.to_string();
  }

  bitset<64> bin(dec);
  return bin.to_string();
}

// TODO: dec and hex to binary with set length
