#!/bin/bash
#include <stddef.h>
# Set your API key
API_KEY="fdd9a1c519e14cae908190928231812"

# Compile the C program
gcc -o weather_program retrieve_data.c main.c store_data.c process_data.c -lcurl -ljansson

# Prompt the user for input
read -p "Enter location: " LOCATION
read -p "Enter output file name: " OUTPUT_FILE
read -p "Choose option (c: Current, h: History, f: Forecast): " DATA_OPTION

# Set the API endpoint and output file based on the user's choice
case "$DATA_OPTION" in
  c)
    API_ENDPOINT="https://api.weatherapi.com/v1/current.json"

    ;;
  h)
    API_ENDPOINT="https://api.weatherapi.com/v1/history.json"
    read -p "Enter date in YYYY-MM-DD format (within last 2 weeks): " DATE

    ;;
  f)
    API_ENDPOINT="https://api.weatherapi.com/v1/forecast.json"
    read -p "Enter number of days (1-14): " DAYS

    ;;
  *)
    echo "Invalid option. Please choose 'c', 'h', or 'f'."
    exit 1
    ;;
esac

# Run the C program with the specified parameters
./weather_program "-dr" "$OUTPUT_FILE" "$LOCATION" "$DATA_OPTION" "$DATE" "$DAYS"

