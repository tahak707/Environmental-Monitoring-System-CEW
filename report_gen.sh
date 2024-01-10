#!/bin/bash

# Set the name of the C source file
SOURCE_FILE="report_gen.c"

# Set the name of the executable file
EXECUTABLE="report_gen"

# Compile the C program
gcc -o $EXECUTABLE retrieve_data.c main.c store_data.c process_data.c report_gen.c -lcurl -ljansson

read -p "Enter location: " LOCATION
# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."

    # Run the executable with the input file name as a command-line argument
    ./report_gen "-rg" "$LOCATION"

    # Check if the program executed successfully
    if [ $? -eq 0 ]; then
        echo "Program execution successful."
    else
        echo "Error: Program execution failed."
    fi

else
    echo "Error: Compilation failed."
fi

# Remove the executable file
rm -f $EXECUTABLE

