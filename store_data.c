#include <stdio.h>
#include <curl/curl.h>

struct WriteCallbackData {
    FILE *file;
};

size_t writecallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct WriteCallbackData *data = (struct WriteCallbackData *)userp;

    // Write the received data to the file
    if (data->file) {
        fwrite(contents, size, nmemb, data->file);
    }

    return realsize;
}

void storeRawDataToFile(char *filename, char *apiEndpoint, char *apiKey, char *query) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        // Open file for writing in binary mode
        FILE *file = fopen(filename, "ab");

        // Set up data structure to pass to write callback
        struct WriteCallbackData writeCallbackData = { file };

        char url[256];
        snprintf(url, sizeof(url), "%s?key=%s&q=%s", apiEndpoint, apiKey, query);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);

        // Pass the data structure to the write callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeCallbackData);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Close the file
        if (file) {
            fclose(file);
        }

        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Error initializing CURL.\n");
    }
}

