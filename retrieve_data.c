#include <stdio.h>
#include <curl/curl.h>

#define API_KEY "fdd9a1c519e14cae908190928231812"
#define API_ENDPOINT "https://api.weatherapi.com/v1/"

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *data = (char *)contents;
    printf("%.*s", (int)realsize, data);
    return realsize;
}

void retrieveWeatherData(char *apiEndpoint, char *apiKey, char *filename, char *query) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "%s?key=%s&q=%s", apiEndpoint, apiKey, query);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        FILE *file = fopen(filename, "wb");

        if (file) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            fclose(file);
            printf("Raw data has been written to %s.\n", filename);
        } else {
            fprintf(stderr, "Error opening file %s for writing raw data.\n", filename);
        }

        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Error initializing CURL.\n");
    }
}



