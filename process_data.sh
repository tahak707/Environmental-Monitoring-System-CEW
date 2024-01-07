#!/bin/bash

# Define a variable to track the C program execution status
executed_flag=".c_program_executed"

# Check if the C program has already been executed
if [ -e "$executed_flag" ]; then
    echo "C program has already been executed. Exiting."
    exit 0
fi

# Compile the C program
gcc -o process_data retrieve_data.c main.c store_data.c process_data.c -lcurl -ljansson

read -p "Enter the name of the raw data file: " RAW_DATA_FILE

# Run the C program
./process_data "-dp" "$RAW_DATA_FILE"

# Check the C program exit status
if [ $? -eq 0 ]; then
    # Mark the C program as executed
    touch "$executed_flag"
else
    echo "C program execution failed."
fi

