//
// Created by ASUS on 02/11/2023.
//

#include "location.h"

int location::getId() {
    return id;
}

void location::setId(int tempId) {
    id = tempId;
}

float location::getLongitude() {
    return longitude;
}

void location::setLongitude(float tempLon) {
    longitude = tempLon;
}

float location::getLatitude() {
    return latitude;
}

void location::setLatitude(float tempLat) {
    latitude = tempLat;
}

std::string location::getCity() {
    return cityName;
}

void location::setCity(std::string tempCity) {
    cityName = tempCity;
}

std::string location::getCountry() {
    return countryName;
}

void location::setCountry(std::string tempCountry) {
    countryName = tempCountry;
}

std::string location::askForInput(std::string name) {
    std::string input;
    std::cout << "Type in " << name << " name: ";
    getline(std::cin, input);
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

json location::makeApiCall(std::string userCity, std::string userCountry) {
        CURL* curl;
        CURLcode res;
        std::string response;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();


        if (curl) {
            std::string url = "https://api.geoapify.com/v1/geocode/search?text=" + userCity + "&country=" + userCountry + "&limit=10&type=city&apiKey=c91e6371439d4849929c6215aed2b2f6";

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();

        // Parse JSON response
        json jsonResponse = json::parse(response);
        return jsonResponse;
}


size_t location::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void location::getLocationChoice(json jsonResponse) {
    std::vector<std::string> cities;
    std::vector<std::string> countries;

    for (int i = 0; i < jsonResponse["features"].size(); i++){
        json tempCity = jsonResponse["features"][i]["properties"]["city"];
        json tempCountry = jsonResponse["features"][i]["properties"]["country"];
        cities.push_back(tempCity);
        countries.push_back(tempCountry);
    }

//    json temp2 = temp[3]["properties"]["city"];

    // Format and print the output
    std::cout << "The options for your search are: " << std::endl;
//    std::cout << std::setw(4) << jsonResponse << std::endl;
    for (int i = 0; i < cities.size(); i++){
        std::cout << i+1 <<". City: " << cities[i] << ", Country: " << countries[i] << std::endl;
    }
    std::cout << "choose your city from the above list: ";
    int x;
    std::cin >> x;

    json longitude = jsonResponse["features"][x-1]["properties"]["lon"];
    json latitude = jsonResponse["features"][x-1]["properties"]["lat"];
    json city = jsonResponse["features"][x-1]["properties"]["city"];
    json country = jsonResponse["features"][x-1]["properties"]["country"];

    setLongitude(longitude);
    setLatitude(latitude);
    setCity(city);
    setCountry(country);
}