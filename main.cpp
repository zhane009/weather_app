#include <iostream>
#include <curl/curl.h>
#include "json.hpp"
#include "location.h"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void makeAPICall(std::string userCity, std::string userCountry) {
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
    std::cout << std::setw(4) << jsonResponse << std::endl;
    for (int i = 0; i < cities.size(); i++){
        std::cout << i+1 <<". City: " << cities[i] << ", Country: " << countries[i] << std::endl;
    }
    std::cout << "choose your city from the above list: ";
    int x;
    std::cin >> x;
    json longitude = jsonResponse["features"][x-1]["properties"]["lon"];
    json latitude = jsonResponse["features"][x-1]["properties"]["lat"];

    std::cout << longitude << ", " << latitude << std::endl;


//    std::cout << std::setw(4) << jsonResponse << std::endl;
}

int main() {
//    std::string city, country;
//    std::cout << "Enter your desired city name: " << std::endl;
//    getline(std::cin, city);
//    std::cout << "Enter your desired country name: " << std::endl;
//    getline(std::cin, country);
//
//    city.erase(remove(city.begin(), city.end(), ' '), city.end());
//    country.erase(remove(country.begin(), country.end(), ' '), country.end());
//
//    makeAPICall(city, country);

    location location;
    std::string city = location.askForInput("city");
    std::string country = location.askForInput("country");
    location.getLocationChoice(location.makeApiCall(city, country));

    std::cout << location.getLongitude() << ", " << location.getLatitude() << ", " << location.getCity() << ", " << location.getCountry() << std::endl;
    return 0;
}
