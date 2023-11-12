#include <iostream>
#include <curl/curl.h>
#include "json.hpp"
#include "location.h"

using json = nlohmann::json;

int main() {

    location location;
//    location.saveLocation();
//    location.writeToFile();
//    location.removeLocation();
//    location.displayLocations();

    std::cout << "Welcome to the weather app!" << std::endl;
    std::cout << "1. Weather Forecast" << std::endl;
    std::cout << "2. Location Management" << std::endl;
    std::cout << "Please choose an option: ";
    int choice;
    try{
        std::cin >> choice;
        if (choice < 1 || choice > 2){
            throw std::exception();
        }
    }
    catch(std::exception e){
        std::cout << "Invalid input, Please try again" << std::endl;
    }
    if (choice == 1){
        std::cout << "Weather Forecast" << std::endl;
    }
    else if (choice == 2){
        std::cout << "Location Management\n"
                     "1. Add a location\n"
                     "2. Remove a location\n"
                     "3. See your added locations\n" << std::endl;
        std::cout << "Please choose an option: ";
        int locationChoice;
        try{
            std::cin >> locationChoice;
            if (locationChoice < 1 || locationChoice > 3){
                throw std::exception();
            }
        }
        catch(std::exception e){
            std::cout << "Invalid input, Please try again" << std::endl;
        }

        if (locationChoice == 1){
            location.saveLocation();
            location.writeToFile();
        }
        else if (locationChoice == 2){
            location.removeLocation();
        }
        else if (locationChoice == 3){
            location.displayLocations();
        }
    }

    return 0;
}
