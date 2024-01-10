#include "weather_quality_monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototype for processRawData
void processRawData(const char *inputFileName);

// Function to generate a detailed report based on processed data
void generateWeatherReport(char *inputFileName) {
    // Open the processed file for reading
    char processedFileName[256];
    snprintf(processedFileName, sizeof(processedFileName), "%s_prcd.txt", inputFileName);
    FILE *processedFile = fopen(processedFileName, "r");
    if (!processedFile) {
        fprintf(stderr, "Error opening processed file %s for reading.\n", processedFileName);
        return;
    }

    // Open the report file for writing
    char reportFileName[256];
    snprintf(reportFileName, sizeof(reportFileName), "%s_report.txt", inputFileName);
    FILE *reportFile = fopen(reportFileName, "w");
    if (!reportFile) {
        fprintf(stderr, "Error opening report file %s for writing.\n", reportFileName);
        fclose(processedFile);
        return;
    }

    // Write header to the report file
    fprintf(reportFile, "Detailed Weather Report\n");
    fprintf(reportFile, "------------------------\n");

    // Variables to store extracted data
    char line[256];
    char time[26], condition[50];
    float temperature, windSpeed;
    int humidity, visibility;
    
    // Read and process each line from the processed file
    while (fgets(line, sizeof(line), processedFile) != NULL) {
        // Check if the line contains time information
        if (sscanf(line, "Time: %[^\n]", time) == 1) {
            fprintf(reportFile, "\nTime: %s\n", time);
        }
        // Check if the line contains temperature information
        else if (sscanf(line, "Temperature C: %f", &temperature) == 1) {
            fprintf(reportFile, "Temperature: %.1f °C\n", temperature);
        }
        // Check if the line contains humidity information
        else if (sscanf(line, "Humidity : %d", &humidity) == 1) {
            fprintf(reportFile, "Humidity: %d%%\n", humidity);
        }
        // Check if the line contains wind speed information
        else if (sscanf(line, "Wind Speed (kph): %f", &windSpeed) == 1) {
            fprintf(reportFile, "Wind Speed: %.2f km/h\n", windSpeed);
        }
        // Check if the line contains visibility information
        else if (sscanf(line, "Visibility (km): %d", &visibility) == 1) {
            fprintf(reportFile, "Visibility: %d km\n", visibility);
        }
        // Check if the line contains condition information
        else if (strstr(line, "Condition:")) {
            sscanf(line, "Condition: %[^\n]", condition);
            fprintf(reportFile, "Condition: %s\n", condition);
        }
    }

    // Close files
    fclose(processedFile);
    fclose(reportFile);

    printf("Report generated successfully: %s\n", reportFileName);
}

