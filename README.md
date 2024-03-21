# Division-Interpreter-LLVM-IR

This is a basic interpreter for a pseudo-language that translates to LLVM IR version 3.3.

## Prerequisites

This project requires the following tools to be installed on your system:
- **LLVM**: The LLVM compiler infrastructure (Download LLVM)
- **lli**: The LLVM interpreter, typically included with the LLVM installation
- **C++11 compiler**: A compiler that supports the C++11 standard

## Installation (Optional)

This step is optional if you already have LLVM and `lli` installed.
1. Download and install LLVM from the official website following the instructions for your operating system: LLVM Download
2. Make sure the `lli` executable is included in your system path after installation. You can check this by running `lli --version` in your terminal.

## Running the Project

1. **Clone the Project (if not already downloaded)**:
   If you haven't already downloaded the project files, you can clone this repository using Git:

git clone https://github.com/mr-narwaria/basic-llvm-ir-interpreter.git


2. **Navigate to the Project Directory**:
Use the `cd` command to navigate to the directory containing the project files:

cd basic-llvm-ir-interpreter


3. **Compile the Interpreter**:
Compile the interpreter source code to generate the executable file. This is typically done using the `make` command:

>make

This will create an executable file named `mylang2ir` in the current directory.

4. **Execute the Interpreter**:
Now you can use the `mylang2ir` executable to interpret your pseudo-language code. The command syntax is:

>./mylang2ir input_file.my

Replace `input_file.my` with the actual filename of your code written in the pseudo-language.
The interpreter will generate an LLVM IR code file named `input_file.ll` in the same directory.

5. **Run the LLVM IR Code (Optional)**:
The generated LLVM IR code can be executed directly using the `lli` interpreter:

>lli input_file.ll

This will execute the LLVM IR code and display the output on your terminal.

Remember to replace `input_file.my` with your actual pseudo-language code file. Have fun experimenting with your interpreter!
