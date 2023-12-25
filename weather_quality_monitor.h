#ifndef WEATHER_QUALITY_MONITOR_H_INCLUDED
#define WEATHER_QUALITY_MONITOR_H_INCLUDED

void retrieveWeatherData(char *apiEndpoint, char *apiKey, char *filename,char *query);
void processRawData(char *inputFile);
void storeRawDataToFile(char *filename, char *apiEndpoint, char *apiKey, char *query);
void generateWeatherReport(char *inputFile, char *reportFile);
void automateTasks(char *apiEndpoint, char *apiKey);


#endif // WEATHER_QUALITY_MONITOR_H_INCLUDED

