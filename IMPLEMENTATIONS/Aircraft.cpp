#include "../HEADERS/Aircraft.h"

Aircraft::Aircraft(int mType, int mConsumption, int mTankCapacity, int speed, int altitude) :
        m_type(mType), m_consumption(mConsumption), m_tankCapacity(mTankCapacity), m_speed(speed), m_altitude(altitude),
        m_fuite(false), m_crash(false) {
    m_pos = {-1, -1};
}

Aircraft::~Aircraft() = default;

void Aircraft::print() {
    switch (m_type) {
        case 1: std::cout << "Short type " << std::endl; break;
        case 2: std::cout << "Medium type " << std::endl; break;
        case 3: std::cout << "Long type " << std::endl; break;
    }
    std::cout << "Consumption: " << m_consumption << "\nFuel capacity: " << m_tankCapacity << "\n" << std::endl;
}

void Aircraft::setPos(Pair pos) { m_pos = pos; }
Pair Aircraft::getPos() const { return m_pos; }
void Aircraft::setSpeed(int speed) { m_speed = speed; }
int Aircraft::getSpeed() const { return m_speed; }
int Aircraft::getSpeed(const Cell *cell) const {
    switch (cell->getType()) {
        case 1: return std::round(m_speed);
        case 2: return std::round(m_speed * 0.5);
        case 3: return 1;
    }
    return std::round(m_speed);
}
int Aircraft::getType() const { return m_type; }
int Aircraft::getTankCapacity() const { return m_tankCapacity; }
int Aircraft::getConsumption() const { return m_consumption; }
void Aircraft::doubleConsumption() { m_consumption *= 2; m_fuite = true; }
int Aircraft::getAltitude() const { return m_altitude; }
void Aircraft::setAltitude(int newAltitude) { m_altitude = newAltitude; }
bool Aircraft::getFuite() const { return m_fuite; }
void Aircraft::setFuite(bool fuite) { m_fuite = fuite; }
void Aircraft::crash() { m_crash = true; }
bool Aircraft::isCrash() const { return m_crash; }
