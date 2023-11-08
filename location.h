//
// Created by ASUS on 02/11/2023.
//

#ifndef WEATHER_APP_LOCATION_H
#define WEATHER_APP_LOCATION_H

#include <iostream>
#include "json.hpp"
#include "curl/curl.h"
#include <fstream>
using json = nlohmann::json;

class location {
private:
    int id;
    float longitude;
    float latitude;
    std::string cityName;
    std::string countryName;
    std::vector<location> locations;

public:
    location();
    location(std::string, std::string, float, float);
    location(int, std::string, std::string, float, float);
    int getId();
    void setId(int);
    float getLongitude();
    void setLongitude(float);
    float getLatitude();
    void setLatitude(float);
    std::string getCity();
    void setCity(std::string);
    std::string getCountry();
    void setCountry(std::string);
    location* getLocations();
    void addElementToLocations(location);

    std::string askForInput(std::string);
    json makeApiCall(std::string, std::string);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    location getLocationChoice(json);
    void saveLocation();
    void writeToFile();
    void readFile();
    void displayLocations();
    void removeLocation();

};


#endif //WEATHER_APP_LOCATION_H
