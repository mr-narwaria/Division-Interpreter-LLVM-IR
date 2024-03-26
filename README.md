# Basic LLVM Division Interpreter

Design and Implementation of an Interpreter that performs division using the LLVM Compilation Framework

## Dependencies

- LLVM
- lli
- C++11

## How to Run

### Run on Linux-Ubuntu or WSL in Windows

**Steps for Run the Software**
1. Compile the Interpreter:
Command: `make`
>make
This command will generate the interpreter executable named `mylang2ir`.

2. **Execute the Interpreter**:
>./mylang2ir input_file.my

Replace `input_file.my` with the path to your input file written in the pseudo-language.

Example:
./mylang2ir examples/input.my


3. **Run the LLVM IR Code**:
After executing the interpreter, it will generate an LLVM IR code file named `input_file.ll`. You can run this LLVM IR code using `lli` (LLVM interpreter) to get the output.
lli input_file.ll


This will execute the LLVM IR code and display the output.

## Example

Assuming you have an input file named `input.my` containing your pseudo-language code, you would run the following commands:
./mylang2ir input.my
lli input.ll


This will execute your pseudo-language code and display the output.
