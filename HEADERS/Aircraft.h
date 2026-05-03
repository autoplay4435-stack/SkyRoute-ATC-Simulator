#ifndef CONTROLEAERIEN_GROUPE_2_G_AIRCRAFT_H
#define CONTROLEAERIEN_GROUPE_2_G_AIRCRAFT_H

#include "Libraries.h"
#include "Cell.h"
#include "util.h"

class Aircraft {
private:
    Pair m_pos;
    int m_type; 
    int m_consumption;
    int m_tankCapacity;
    int m_speed;
    int m_altitude; 
    bool m_fuite;
    bool m_crash;

public:
    Aircraft(int mType, int mConsumption, int mTankCapacity, int speed, int altitude);
    ~Aircraft();
    void print();
    void setPos(Pair pos);
    Pair getPos() const;
    void setSpeed(int speed);
    int getSpeed() const;
    int getSpeed(const Cell *cell) const;
    int getType() const;
    int getTankCapacity() const;
    int getConsumption() const;
    int getAltitude() const;
    void setAltitude(int newAltitude);
    void doubleConsumption();
    bool getFuite() const;
    void setFuite(bool fuite);
    void crash();
    bool isCrash() const;
};
#endif
