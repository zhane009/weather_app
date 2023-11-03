#include <iostream>
#include <curl/curl.h>
#include "json.hpp"
#include "location.h"

using json = nlohmann::json;

int main() {

    location location;
    location.saveLocation();
    location.writeToFile();
    location.readFileAndDisplay();

    return 0;
}
