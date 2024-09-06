# Binary Convertor Project

## Overview

The Binary Convertor is a powerful C program designed to convert hexadecimal input into various data types, including signed integers, unsigned integers, and floating-point numbers. It supports different byte orderings (little endian and big endian) and various data type sizes (1, 2, 3, or 4 bytes).

## Features

- Converts hexadecimal input to binary
- Supports multiple data types:
  - Signed integers
  - Unsigned integers
  - Floating-point numbers
- Handles different byte orderings:
  - Little Endian
  - Big Endian
- Supports various data type sizes:
  - 1 byte
  - 2 bytes
  - 3 bytes
  - 4 bytes
- Reads input from a file
- Writes output to a file
- Handles special cases for floating-point numbers (NaN, Infinity)
- User-friendly command-line interface

## Prerequisites

To compile and run this project, you need:

- A C compiler (e.g., GCC)
- Standard C libraries

## Compilation

To compile the project, use the following command:

```bash
gcc binary_convertor.c -o binary_convertor -lm
```

Note: The `-lm` flag is necessary to link the math library.

## Usage

1. Run the compiled program:

```bash
./binary_convertor
```

2. Follow the prompts to enter:
   - Input file name (must be a .txt file)
   - Byte ordering (Little Endian: 'l' or Big Endian: 'b')
   - Data type (Signed Integer: 'int', Unsigned Integer: 'unsigned', Floating-point Number: 'float')
   - Data type size (1, 2, 3, or 4 bytes)

3. The program will read the hexadecimal input from the specified file, convert it according to the given parameters, and write the output to a file named `output.txt`.

## Input File Format

The input file should contain hexadecimal values without any separators. For example:

```
0A1B2C3D4E5F6A7B8C9D0E1F
```

## Output

The program will generate an `output.txt` file containing the converted values. The output format depends on the chosen data type:

- Integers: Decimal representation
- Floating-point numbers: Decimal or scientific notation, depending on the value's magnitude

## Error Handling

The program includes error checking for:

- File opening failures
- Invalid input file content
- Incorrect byte ordering input
- Invalid data type input
- Invalid data type size input

## Limitations

- The maximum number of bytes that can be read from the input file is 1024.
- Floating-point conversions are limited to single-precision (32-bit) format.

## Contributing

Contributions to improve the Binary Convertor project are welcome. Please feel free to submit pull requests or open issues to suggest improvements or report bugs.

## License

This project is open-source and available under the [MIT License](https://opensource.org/licenses/MIT).
