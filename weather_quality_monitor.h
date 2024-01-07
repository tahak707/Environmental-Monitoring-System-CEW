#include <stddef.h>
#include <curl/curl.h>
#ifndef WEATHER_QUALITY_MONITOR_H_INCLUDED
#define WEATHER_QUALITY_MONITOR_H_INCLUDED

void retrieveWeatherData(char *apiEndpoint, char *apiKey, char *filename,char *query);
void processRawData(const char *inputFile);
void storeRawDataToFile(char *filename, char *apiEndpoint, char *apiKey, char *query);
void generateWeatherReport(char *inputFile, char *reportFile);
void automateTasks(char *apiEndpoint, char *apiKey);
void sendEmail(const char *subject, const char *body);


#endif // WEATHER_QUALITY_MONITOR_H_INCLUDED

