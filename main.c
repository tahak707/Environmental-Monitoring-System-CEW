#include "weather_quality_monitor.h"
#include <curl/curl.h>
#include <string.h>

#define API_ENDPOINT "https://api.weatherapi.com/v1/"
#define API_KEY "fdd9a1c519e14cae908190928231812"

int main(int argc, char *argv[]) {
char *apikey = API_KEY;
    if (argc < 2) {
        printf("Usage: %s <option> <arguments>\n", argv[0]);
        return 1;
    }

    char *option = argv[1];

    if (strcmp(option, "-dr") == 0) {
        // Data retrieval
        printf("Choose option:\n1) Current (c)\n2) History (h)\n3) Forecast (f)\n");
            char subOption;
            scanf("%c", &subOption);
            while(getchar() != '\n'); // Clear input buffer
            char apiEndpoint[256];
            switch (subOption) {
                case 'c':
                    snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "current.json");
                    retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
                    storeRawDataToFile(argv[2],apiEndpoint,apikey, argv[3]);
                    break;
                case 'h':
                    snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "history.json");
                    char date[11];
		    printf("Enter date in YYYY-MM-DD format (within last 365 days) :");
		    scanf("%s", date);
		    sprintf(argv[3], "%s&dt=%s", argv[3], date);
		    retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
		    storeRawDataToFile(argv[2],apiEndpoint,apikey, argv[3]);
                    break;
                case 'f':
                    snprintf(apiEndpoint, sizeof(apiEndpoint), "%s%s", API_ENDPOINT, "forecast.json");
                    char days[3];
                    printf("Enter number of days (1-14) : ");
                    scanf("%s",days);
                    sprintf(argv[3], "%s&days=%s", argv[3], days);
                    retrieveWeatherData(apiEndpoint, apikey, argv[2], argv[3]);
                    storeRawDataToFile(argv[2],apiEndpoint,apikey, argv[3]);
                    break;
                default:
                    printf("Invalid sub-option: %c\n", subOption);
                    return 1;
            
            break;}
        
     }else if (strcmp(option, "-dp") == 0) {
        // Data processing
        processRawData(argv[2]);
     //else if (strcmp(option, "-rg") == 0) {
      // Report generation
        //generateWeatherReport(argv[2], argv[3]);
    
    }else {
        printf("Invalid option: %s\n", option);
        return 1;
    }

    return 0;
}

