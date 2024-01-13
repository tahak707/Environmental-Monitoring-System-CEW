#include "weather_quality_monitor.h"
#include <stdio.h>
#include <jansson.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Function to send email alerts

// Function to send email alerts and update the log file
void detectAnomalies(float value, const char *name, float threshold) {
    // Hardcoded precautions based on the anomaly type
    const char *temperaturePrecaution = "Take necessary measures to stay cool.";
    const char *windSpeedPrecaution = "Secure outdoor objects and be cautious of strong winds.";
    const char *precipitationPrecaution = "Be prepared for wet weather and consider indoor activities.";

    if (value > threshold) {
        printf("%s anomaly detected: %g\n", name, value);

        const char *precaution = NULL;

        // Determine the type of anomaly and set the appropriate precaution
        if (strcmp(name, "Temperature") == 0) {
            precaution = temperaturePrecaution;
        } else if (strcmp(name, "Wind Speed") == 0) {
            precaution = windSpeedPrecaution;
        } else if (strcmp(name, "Precipitation") == 0) {
            precaution = precipitationPrecaution;
        }

        if (precaution) {
            // Send an email alert with the relevant precaution
            char subject[256];
            char body[256];
            snprintf(subject, sizeof(subject), "Critical Anomaly Detected: %s", name);
            snprintf(body, sizeof(body), "Critical anomaly detected in %s: %g\nPrecaution: %s", name, value, precaution);
            sendEmailWithLog(subject, body);
        } else {
            fprintf(stderr, "Error: Unknown anomaly type.\n");
        }
    }
}


typedef struct {
    const char *date;
    float maxtemp_c;
    float mintemp_c;
    float avgtemp_c;
    float maxwind_kph;
    float totalprecip_mm;
    float avgvis_km;
    float avghumidity;
    float chance_of_rain;
    float chance_of_snow;
    const char *condition;
} DailyForecast;

void processCurrentData(json_t *current, FILE *outputFile) {
        json_t *temp_c = json_object_get(current, "temp_c");
        json_t *humidity = json_object_get(current, "humidity");
        json_t *precip_mm = json_object_get(current, "precip_mm");
        json_t *wind_kph = json_object_get(current, "wind_kph");
        json_t *condition = json_object_get(current, "condition");

        // Write the processed current data to the output file
        fprintf(outputFile, "Current Data:\n");
        fprintf(outputFile, "Temperature C:  %g\n", json_real_value(temp_c));
        fprintf(outputFile, "Humidity : %lld\n", json_integer_value(humidity));
        fprintf(outputFile, "Precipitation (mm): %g\n", json_real_value(precip_mm));
        fprintf(outputFile, "Wind Speed (kph): %g\n", json_real_value(wind_kph));
        fprintf(outputFile, "Condition: %s\n", json_string_value(json_object_get(condition, "text")));

        // Detect anomalies
        detectAnomalies(json_real_value(temp_c), "Temperature", 20.0);
        detectAnomalies(json_real_value(wind_kph), "Wind Speed", 10.0);
        detectAnomalies(json_real_value(precip_mm), "Precipitation", 5.0);

        fprintf(outputFile, "\n");
    } 

    // Extract specific data from "forecast"
void processForecastData(json_t *forecast, FILE *outputFile) {
    json_t *forecastday = json_object_get(forecast, "forecastday");
    if (json_is_array(forecastday)) {
        size_t arraySize = json_array_size(forecastday);
        if (arraySize > 0) {
            // Write forecast data to the output file
            fprintf(outputFile, "Forecast Data:\n");
            for (size_t i = 0; i < arraySize; ++i) {
                json_t *day = json_array_get(forecastday, i);
                if (json_is_object(day)) {
                    // Retrieve the date for each forecast day
                    json_t *dateObj = json_object_get(day, "date");
                    if (json_is_string(dateObj)) {
                        const char *forecastDate = json_string_value(dateObj);
                        fprintf(outputFile, "Date: %s\n", forecastDate);
                    } else {
                        fprintf(stderr, "Error: 'date' data for day %zu is not a string.\n", i + 1);
                        continue;
                    }

                    json_t *day_data = json_object_get(day, "day");
                    if (json_is_object(day_data)) {
                        json_t *avgtemp_c = json_object_get(day_data, "avgtemp_c");
                        json_t *maxtemp_c = json_object_get(day_data, "maxtemp_c");
                        json_t *mintemp_c = json_object_get(day_data, "mintemp_c");
                        json_t *maxwind_kph = json_object_get(day_data, "maxwind_kph");
                        json_t *chance_of_rain = json_object_get(day_data, "daily_chance_of_rain");
                        json_t *chance_of_snow = json_object_get(day_data, "daily_chance_of_snow");
                        json_t *avgvis_km = json_object_get(day_data, "avgvis_km");
                        json_t *avghumidity = json_object_get(day_data, "avghumidity");
                        json_t *totalprecip_mm = json_object_get(day_data, "totalprecip_mm");
                        json_t *condition = json_object_get(day_data, "condition");

                        // Write forecast data to the output file
                        fprintf(outputFile, "Day %zu:\n", i + 1);
                        fprintf(outputFile, "Average Temperature C: %g\n", json_real_value(avgtemp_c));
                        fprintf(outputFile, "Max Temperature C: %g\n", json_real_value(maxtemp_c));
                        fprintf(outputFile, "Min Temperature C: %g\n", json_real_value(mintemp_c));
                        fprintf(outputFile, "Max Wind Speed (kph): %g\n", json_real_value(maxwind_kph));
                        fprintf(outputFile, "Chance of Rain : %g %s\n", json_real_value(chance_of_rain), "%");
                        fprintf(outputFile, "Chance of Snow : %g %s\n", json_real_value(chance_of_snow), "%");
                        fprintf(outputFile, "Average Humidity : %g %s\n", json_real_value(avghumidity), "%");
                        fprintf(outputFile, "Average Visibility (km): %g\n", json_real_value(avgvis_km));
                        fprintf(outputFile, "Total Precipitation (mm): %g\n", json_real_value(totalprecip_mm));
                        fprintf(outputFile, "Condition: %s\n", json_string_value(json_object_get(condition, "text")));
                        // Detect anomalies
        		detectAnomalies(json_real_value(avgtemp_c), "Temperature", 20.0);
        		detectAnomalies(json_real_value(maxwind_kph), "Wind Speed", 10.0);
        		detectAnomalies(json_real_value(totalprecip_mm), "Precipitation", 5.0);
                    } else {
                        fprintf(stderr, "Error: 'day' data for day %zu is not an object.\n", i + 1);
                    }

                    // Process hourly data if available
                    json_t *hourly = json_object_get(day, "hour");
                    if (json_is_array(hourly)) {
                        size_t hourlySize = json_array_size(hourly);
                        if (hourlySize > 0) {
                            fprintf(outputFile, "Hourly Stats:\n");
                            for (size_t j = 0; j < hourlySize; ++j) {
                                json_t *hour = json_array_get(hourly, j);
                                if (json_is_object(hour)) {
                                    json_t *timeObj = json_object_get(hour, "time");
                                    json_t *temp_c = json_object_get(hour, "temp_c");
                                    json_t *wind_kph = json_object_get(hour, "wind_kph");
                                    json_t *chance_of_rain = json_object_get(hour, "chance_of_rain");
                                    json_t *chance_of_snow = json_object_get(hour, "chance_of_snow");
                                    json_t *vis_km = json_object_get(hour, "vis_km");
                                    json_t *humidity = json_object_get(hour, "humidity");
                                    json_t *totalprecip_mm = json_object_get(hour, "precip_mm");
                                    json_t *condition = json_object_get(hour, "condition");

                                    // Write hourly data to the output file
                                    fprintf(outputFile, "Time: %s\n", json_string_value(timeObj));
                                    fprintf(outputFile, "Temperature C: %g\n", json_real_value(temp_c));
                                    fprintf(outputFile, "Wind Speed (kph): %g\n", json_real_value(wind_kph));
                                    fprintf(outputFile, "Chance of Rain : %g %s\n", json_real_value(chance_of_rain), "%");
                                    fprintf(outputFile, "Chance of Snow : %g %s\n", json_real_value(chance_of_snow), "%");
                                    fprintf(outputFile, "Visibility (km): %g\n", json_real_value(vis_km));
                                    fprintf(outputFile, "Humidity : %g %s\n", json_real_value(humidity), "%");
                                    fprintf(outputFile, "Total Precipitation (mm): %g\n", json_real_value(totalprecip_mm));
                                    fprintf(outputFile, "Condition: %s\n", json_string_value(json_object_get(condition, "text")));
                                    
                                    fprintf(outputFile, "\n");
                                } else {
                                    fprintf(stderr, "Error: Hourly data for hour %zu is not an object.\n", j + 1);
                                }
                            }
                        } else {
                            fprintf(stderr, "Error: 'hour' array is empty.\n");
                        }
                    } else {
                        fprintf(stderr, "Error: 'hour' data is not an array.\n");
                    }
                } else {
                    fprintf(stderr, "Error: Forecast data for day %zu is not an object.\n", i + 1);
                }
            }
            fprintf(outputFile, "\n");
        } else {
            fprintf(stderr, "Error: 'forecastday' array is empty.\n");
        }
    } else {
        fprintf(stderr, "Error: 'forecastday' data is not an array.\n");
    }
}

// Process raw data
void processRawData(const char *inputFileName) {
    // Open the input file for reading
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        fprintf(stderr, "Error opening file %s for reading.\n", inputFileName);
        return;
    }

    // Parse the JSON data
    json_t *root;
    json_error_t error;
    root = json_loadf(inputFile, 0, &error);
    fclose(inputFile);

    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    // Extract required data
    json_t *location = json_object_get(root, "location");
    json_t *current = json_object_get(root, "current");
    json_t *forecast = json_object_get(root, "forecast");

    // Extract 'name' and 'localtime' for generating the output file name
    json_t *name = json_object_get(location, "name");

    if (!json_is_string(name)) {
        fprintf(stderr, "Error: 'name' data is not a string.\n");
        json_decref(root);
        return;
    }

    // Create the output file name based on the extracted data
    const char *outputFileNameFormat = "%s_prcd.txt";
    char outputFileName[256];
    snprintf(outputFileName, sizeof(outputFileName), outputFileNameFormat,
             json_string_value(name));

    // Open the output file for writing
    FILE *outputFile = fopen(outputFileName, "a");
    if (!outputFile) {
        fprintf(stderr, "Error opening file %s for writing.\n", outputFileName);
        json_decref(root);
        return;
    }

    // Process current data if available
    if (json_is_object(current)) {
        processCurrentData(current, outputFile);
    }

    // Process forecast data if available
    if (json_is_object(forecast)) {
        processForecastData(forecast, outputFile);
    }

    // Close the output file
    fclose(outputFile);

    // Cleanup
    json_decref(root);
}


