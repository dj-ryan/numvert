![Imgur](https://imgur.com/Ye2hUWS.png)


# Description

### A cli program to convert numbers between bases

# Installation

- Clone and go to the root directory
- Build using the make file or compile using:
  - g++ src/numvert.cpp -o numvert -Wall -std=c++11 -Iinclude
- Install
  - Windows
    - Copy binary to system folder such as 'Program Files'
    - Add path to PATH variable
  - Linux
    - Copy binary into bin folder


# Usage
```
  numvert {OPTIONS}

    Numvert

  OPTIONS:

      Input types:
        -h[Hexadecimal...],
        --hex=[Hexadecimal...]            Hexadecimal input (do not apply 0x
                                          prefix)
        -d[Decimal...], -i[Decimal...],
        --dec=[Decimal...],
        --int=[Decimal...]                Decimal input
        -b[Binary...], --bin=[Binary...]  Binary input (do not apply 0b prefix)
      Output formating options:
        -f, --full                        Prints advanced output
        -s[4 or 8], --binary-spacing=[4
        or 8]                             Prints spaces in between every forth
                                          binary bit or every byte
        --tb, --truncate-binary           Truncates binary output to least
                                          significant '1'
        -H, --cap-hex                     Prints all hexadecimal output with
                                          capital letters
      Other options:
        --version                         Prints version information
      -?, --help                        Display this help menu

    Author: David Ryan
    ~ ALL TO HIM
```

# Road Map
  - Binary Parsing
    - Hex and nibble matching
    - copy output to clipboard
    - parse and convert csv file
      - output to file

