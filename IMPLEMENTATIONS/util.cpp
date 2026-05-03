#include "../HEADERS/util.h"

int randomInt(int min, int max) {
    static std::mt19937 randGen(time(nullptr));
    return std::uniform_int_distribution<>(min, max)(randGen);
}

float map(float n, float min1, float max1, float min2, float max2) {
    return min2 + (max2 - min2) * ((n - min1) / (max1 - min1));
}

bool inbound(int x, int y, int upperLeftX, int upperLeftY, int downRightX, int downRightY) {
    return x >= upperLeftX && x < downRightX && y >= upperLeftY && y < downRightY;
}

std::string conversionUTHeure(int nbUT){
    int totalminutes = nbUT * 15;
    if (totalminutes > 1440) { totalminutes = totalminutes - 1440; }
    int minutes = totalminutes % 60;
    int heures = (totalminutes - totalminutes % 60) / 60;
    return std::to_string(heures) + "h" + std::to_string(minutes);
}
