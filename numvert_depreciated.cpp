/*
* old conversion functions
*/

#include <string>
#include <math.h>


using namespace std;





/*
printf strings:

printf("+------------------------------------------------+\n\r");
printf("| ERROR: invalid tag '%s'\n\r", argv[i]);

printf("| -> 0b%s", argv[i+1]);






*/

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



  unsigned long long int bin2dec(string bin) {
  
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



void hexToDec () {
  // hex conversion using std::hex
  // std::cout << "HEX: " << args::get(hexadecimal) << std::endl;
//   long long int decOutput;
//   std::stringstream ss;
//   ss << args::get(hexadecimal);
//   ss >> std::hex >> decOutput;
//   std::cout << "DEC: " << decOutput << std::endl;

}