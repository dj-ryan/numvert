




![Imgur](https://imgur.com/Ye2hUWS.png)


# Description

### A lightweight cli program to convert numbers between bases

# Features

- Color coded output for easy readability 
- Batch convert multiple values from single command
- Convert numbers up to 8 bytes or 18,446,744,073,709,551,615
- Format output with capital hexadecimal output, 
    spaced binary output (on nibble or byte), comma decimal output etc...

# Installation

- Clone and go to the root directory
- Build using the make file or compile using:
  - > g++ src/numvert.cpp -o numvert -Wall -std=c++11 -Iinclude
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
        --hex=[Hexadecimal...]            Hexadecimal input, case insensitive
                                          (do not apply 0x prefix)
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
                                          capital letters (can not be used as
                                          input)
      Copy to clipboard options:
        -x, --ch, --copy-hex,
        --copy-hexadecimal                Copies hexadecimal conversion output
                                          to clipboard
        -y, --cd, --copy-dec,
        --copy-decimal                    Copies decimal conversion output to
                                          clipboard
        -z, --cb, --copy-bin,
        --copy-binary                     Copies binary conversion output to
                                          clipboard
      Other options:
        --version                         Prints version information
      -?, --help                        Display this help menu

    Author: David Ryan
    ~ ALL TO HIM
```

# Trouble shooting

### Make file not working?

You can build the file your self. 
1. Make sure that you have `g++` installed.
2. Compile using this line:
> g++ src/numvert.cpp -o numvert -Wall -std=c++11 -Iinclude

### Random Characters?  

If you see random characters in your output such as `\x1b[41m` 

You need to enable ANSI character escapes on your terminal or use a different terminal. 

On windows
- [Follow these instructions](https://ss64.com/nt/syntax-ansi.html)

On Linux or Mac
- Try a different terminal

# Features to come
  - Binary Parsing
    - Hex and nibble matching
  - copy output to clipboard
  - parse and convert csv file
    - output to file
  - Octal conversions
  - Copy specified output to clipboard automatically from command

