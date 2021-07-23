# numvert
A cli program to convert numbers between bases

```
    numvert {OPTIONS}

    Numvert

    OPTIONS:

      Output formating options:
        -f, --full                        Prints advanced output
        -s[4 or 8], --binary-spacing=[4
        or 8]                             Prints spaces in between every forth
                                          binary bit or every byte
        --tb, --truncate-binary           Truncates binary output to least
                                          significant '1'
        -H, --cap-hex                     Prints all hexadecimal output with
                                          capital letters
      -?, --help                        Display this help menu
      -h[Hexadecimal...],
      --hex=[Hexadecimal...]            Hexadecimal input (do not apply 0x
                                        prefix)
      -i[Decimal...], -d[Decimal...],
      --dec=[Decimal...]                Decimal input
      -b[Binary...], --bin=[Binary...]  Binary input (do not apply 0b prefix
```

## Road Map
  - Binary Parsing
    - Hex and nibble matching
