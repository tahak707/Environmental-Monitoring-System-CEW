#include <stdio.h>
#include <jansson.h>

void detectTemperatureAnomalies(float temperature) {
    // Set a threshold for temperature anomalies
    const float temperatureThreshold = 1.0;
    printf("heloooooo");

    if (temperature > temperatureThreshold) {
        printf("Temperature anomaly detected: %f\n", temperature);
    }
}

void detectWindSpeedAnomalies(float windSpeed) {
    // Set a threshold for wind speed anomalies
    const float windSpeedThreshold = 10.0;

    if (windSpeed > windSpeedThreshold) {
        printf("Wind speed anomaly detected: %f\n", windSpeed);
    }
}

void processRawData(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        return;
    }

    // Parse the JSON data
    json_t *root;
    json_error_t error;

    root = json_loadf(file, 0, &error);
    fclose(file);

    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    // Extract current temperature and wind speed from JSON
    json_t *current = json_object_get(root, "current");
    if (current) {
        json_t *temp_c = json_object_get(current, "temp_c");
        json_t *wind_kph = json_object_get(current, "wind_kph");

        if (json_is_number(temp_c) && json_is_number(wind_kph)) {
            float temperature = json_number_value(temp_c);
            float windSpeed = json_number_value(wind_kph);

            // Process temperature anomalies
            detectTemperatureAnomalies(temperature);

            // Process wind speed anomalies
            detectWindSpeedAnomalies(windSpeed);
        } else {
            fprintf(stderr, "Error extracting temperature and wind speed from JSON.\n");
        }
    } else {
        fprintf(stderr, "Error extracting 'current' data from JSON.\n");
    }

    // Cleanup
    json_decref(root);
}

