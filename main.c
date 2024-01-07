#include "weather_quality_monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define API_ENDPOINT "https://api.weatherapi.com/v1/"
#define API_KEY "fdd9a1c519e14cae908190928231812"

volatile sig_atomic_t alertFlag = 0;

void sendEmail(const char *subject, const char *body) {
    char command[512];

    // Construct the mail command
    snprintf(command, sizeof(command),
             "echo \"%s\" | mail -s \"%s\" taha.khn70@gmail.com", body, subject);

    // Execute the mail command
    int result = system(command);

    // Check the result of the system command
    if (result == -1) {
        fprintf(stderr, "Failed to execute mail command\n");
    } else {
        printf("Email sent successfully\n");
    }
}


// Signal handler for alerts
void handleAlertSignal(int signum) {
    if (signum == SIGALRM) {
        // Handle the alert (e.g., send email, log, etc.)
        printf("Alert: Critical environmental readings detected!\n");
        // Set the flag to indicate an alert
        alertFlag = 1;
    }
}

int main(int argc, char *argv[]) {
    char *apikey = API_KEY;

    // Register the signal handler
    signal(SIGALRM, handleAlertSignal);

    if (argc < 2) {
        printf("Usage: %s <option> <arguments>\n", argv[0]);
        return 1;
    }

    char *option = argv[1];

    if (strcmp(option, "-dr") == 0) {
        // Data retrieval
        printf("Choose option:\n1) Current (c)\n2) History (h)\n3) Forecast (f)\n");
        char subOption;
        scanf(" %c", &subOption);  

        char apiEndpoint[256];

        switch (subOption) {
            case 'c':
                snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "current.json");
                retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
                storeRawDataToFile(argv[2], apiEndpoint, apikey, argv[3]);
                break;
            case 'h':
                snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "history.json");
                char date[11];
                printf("Enter date in YYYY-MM-DD format (within last 365 days): ");
                scanf("%s", date);
                sprintf(argv[3], "%s&dt=%s", argv[3], date);
                retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
                storeRawDataToFile(argv[2], apiEndpoint, apikey, argv[3]);
                break;
            case 'f':
                snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "forecast.json");
                char days[3];
                printf("Enter number of days (1-14): ");
                scanf("%s", days);
                sprintf(argv[3], "%s&days=%s", argv[3], days);
                retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
                storeRawDataToFile(argv[2], apiEndpoint, apikey, argv[3]);
                break;
            default:
                printf("Invalid sub-option: %c\n", subOption);
                return 1;
        }

        // Ask user if they want to process the data
        printf("Do you want to process the data? (Y/N): ");
        char s;
        scanf(" %c", &s);
        if (s == 'Y' || s == 'y') {
            system("./process_data.sh");
        } else if (s == 'N' || s == 'n') {
            printf("Exiting\n");
        } else {
            printf("Invalid input. Exiting\n");
        }
    } else if (strcmp(option, "-dp") == 0) {
        // Data processing
        processRawData(argv[2]);
        // Send email alert if an anomaly is detected
        if (alertFlag) {
            sendEmail("Environmental Alert", "Critical Anomaly detected !");
            // Reset the alert flag
            alertFlag = 0;
        }
        }else if (strcmp(option, "-rg") == 0){
        
        // Report generation
        // generateWeatherReport(argv[2]);
    } else {
        printf("Invalid option: %s\n", option);
        return 1;
    }

    // Set up a timer for periodic checks (e.g., every 5 minutes)
    alarm(300);  // 300 seconds = 5 minutes

    // Main loop
    while (!alertFlag) {
        // Your existing main loop logic

        // Sleep for a short duration before the next iteration
        sleep(1);
    }

    return 0;
}

