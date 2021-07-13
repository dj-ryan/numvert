#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>







/*
printf("+------------------------------------------------+");
printf("| ERROR: invalid tag '%c'                        |", argv[i][1]);



















*/

unsigned long long int bin2dec(char bin[], int len);
int hex2dec(char bin[], int len);
void hex2bin(char bin[], char hex[], int len);
void printBin(char bin[], int len);

int main(int argc, char *argv[]) {

  int i;

  for (i = 0; i < argc; i++) {

    // printf("Argument %d : %s\n", i, argv[i]);
    if (argv[i][0] == '-') {
      printf("+------------------------------------------------+");
      if (argv[i][1] == 'b') {
        unsigned long long int dec = bin2dec(argv[i + 1], strlen(argv[i + 1]));

        printf("DEC : %llu \n\r", dec);
        printf("HEX : %llX \n\r", dec);

      } else if (argv[i][1] == 'd') {
      } else if (argv[i][1] == 'h') {

         printf("strlen: %d \n\r", strlen(argv[i+1]));

        char bin[strlen(argv[i + 1]) * 4];
        hex2bin(bin, argv[i + 1], strlen(argv[i + 1]));
        printf("BIN : ");
        printBin(bin, strlen(bin));
        printf(" \n\r");
      }
    } else {
      printf("| ERROR: invalid tag '%c'", argv[i][1]);
    }
    printf("+------------------------------------------------+");
  }

  return 0;
}

int bin2hex(uint32_t bin) {}

unsigned long long int bin2dec(char bin[], int len) {
  unsigned long long int dec = 0;
  int pos = 0;
  int i = len - 1;
  while (i >= 0) {
    dec = dec + (bin[i] - 48) * (pow(2, pos));
    pos++;
    i--;
  }
  return dec;
}

int hex2dec(char hex[], int len) {}

void hex2bin(char bin[], char hex[], int len) {

  int i;
  strcpy(bin,"0000");
  // for (i = 1; i < len; i++) {

  //   switch (hex[i]) {
  //   case '0':
  //     strcat(bin, "0000");
  //     break;
  //   case '1':
  //     strcat(bin, "0001");
  //     break;
  //   }
  // }
   while (hex[i]) {

       switch (hex[i]) {
       case '0':
           strcat(bin, "0000");
           break;
       case '1':
           strcat(bin, "0001");
           break;
       case '2':
           strcat(bin, "0010");
           break;
       case '3':
           strcat(bin, "0011");
           break;
       case '4':
           strcat(bin, "0100");
           break;
       case '5':
           strcat(bin, "0101");
           break;
       case '6':
           strcat(bin, "0110");
           break;
       case '7':
           strcat(bin, "0111");
           break;
       case '8':
           strcat(bin, "1000");
           break;
       case '9':
           strcat(bin, "1001");
           break;
       case 'A':
       case 'a':
           strcat(bin, "1010");
           break;
       case 'B':
       case 'b':
           strcat(bin, "1011");
           break;
       case 'C':
       case 'c':
           strcat(bin, "1100");
           break;
       case 'D':
       case 'd':
           strcat(bin, "1101");
           break;
       case 'E':
       case 'e':
           strcat(bin, "1110");
           break;
       case 'F':
       case 'f':
           strcat(bin, "1111");
           break;
       default:
           printf("\nInvalid hexadecimal digit %c",
                  bin[i]);
       }
       i++;
   }
}

void print_char_array_as_num(char arr[], int len) {
  int i;
  for (i = 0; i < len; i++) {
    printf("%u", arr[i] - 48);
  }
}
