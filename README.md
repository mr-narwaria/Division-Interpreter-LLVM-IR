# Basic LLVM Division Interpreter

Design and Implementation of a Interpreter that performs division using LLVM Compilation Framework

## Dependencies

- LLVM
- lli
- C++11
- make

## How to Run

### Run on Linux-Ubuntu or WSL in Windows
```bash
git clone 
```

**Steps for Run the Software**
1. Compile the Interpreter:
```bash
make
```
This command will generate the interpreter executable named `division-interpreter`.

2. Execute the Interpreter:
```bash
./division-interpreter input.my
```
Replace `input.my` with the path to your input file written.

Example:
./division-interpreter examples/input.my


3. Run the LLVM IR Code:
After executing the interpreter, it will generate an LLVM IR code file named `input.ll`. You can run this LLVM IR code using `lli` (LLVM interpreter tool) to get the output.
```bash
lli input.ll
```
This will execute the LLVM IR code and display the output.

## Example

Assuming you have an input file named `input.my` containing your input, you would run the following commands in the sequences: <br>
>make <br>
>./division-interpreter input.my <br>
>lli input.ll


This will execute your input and display the output.
