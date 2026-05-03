#ifndef CONTROLEAERIEN_GROUPE_2_G_UTIL_H
#define CONTROLEAERIEN_GROUPE_2_G_UTIL_H

#include "Libraries.h"

int randomInt(int min, int max);
float map(float n, float min1, float max1, float min2, float max2);
bool inbound(int x, int y, int upperLeftX, int upperLeftY, int downRightX, int downRightY);
std::string conversionUTHeure(int nbUT);

#endif
