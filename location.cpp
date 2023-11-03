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

location* location::getLocations(){
    return locations.data();
}

void location::addElementToLocations(location tempLocation) {
    locations.push_back(tempLocation);
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

location location::getLocationChoice(json jsonResponse) {
    std::vector<std::string> cities;
    std::vector<std::string> countries;

    for (int i = 0; i < jsonResponse["features"].size(); i++){
        json tempCity = jsonResponse["features"][i]["properties"]["city"];
        json tempCountry = jsonResponse["features"][i]["properties"]["country"];
        cities.push_back(tempCity);
        countries.push_back(tempCountry);
    }

    // Format and print the output
    std::cout << "The options for your search are: " << std::endl;

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

    location temp;
    temp.setLongitude(longitude);
    temp.setLatitude(latitude);
    temp.setCity(city);
    temp.setCountry(country);

    return temp;
}

void location::saveLocation() {
    std::string city = askForInput("city");
    std::string country = askForInput("country");

    location choice = getLocationChoice(makeApiCall(city, country));
    addElementToLocations(choice);
}

void location::writeToFile() {
    std::fstream LocationFile;
    std::fstream tempFile;

    LocationFile.open("locations.txt", std::ios::app);
    tempFile.open("locations.txt");
    int count = 0;

    if (LocationFile.is_open() && tempFile.is_open()){
        std::string temp;
        while(std::getline(tempFile, temp)){
            count ++;
        }
        for (int i = 0; i < locations.size(); i++){
            LocationFile << count + 1 << "," << locations[i].getCity() << "," << locations[i].getCountry() <<
                            "," << locations[i].getLongitude() << "," << locations[i].getLatitude() << std::endl;
        }
    }

    LocationFile.close();
    tempFile.close();

    std::cout << "Your location has been saved" << std::endl;
}

void location::readFileAndDisplay() {
    std::fstream LocationFile;

    LocationFile.open("locations.txt");
    std::string line,tempWord, tempCity, tempCountry;
    int tempId;
    double tempLat, tempLon;
    std::cout << "Your location data is as follows: " << std::endl;

    if (LocationFile.is_open()){
        while(std::getline(LocationFile, line)){
            std::stringstream locationString(line);
            int counter = 0;

            while (std::getline(locationString>>std::ws, tempWord, ',')) {
                if (counter == 0) {
                    tempId = stoi(tempWord);
                }
                else if (counter == 1){
                    tempCity = tempWord;
                }
                else if (counter == 2){
                    tempCountry = tempWord;
                }
                else if (counter == 3) {
                    tempLon = std::stod(tempWord);
                }
                else if (counter == 4) {
                    tempLat = std::stod(tempWord);
                }
                counter++;

            }

            std::cout << "ID: " << tempId << ", City Name: " << tempCity << ", Country Name: " << tempCountry
                        << "\n Longitude: " << tempLon << ", Latitude: " << tempLat << std::endl;

        }
    }

    LocationFile.close();

}
