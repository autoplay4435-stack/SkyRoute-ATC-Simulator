#include "../HEADERS/Airport.h"

Airport::Airport(std::string Name, int ListID, Pair Pos, int NbTracks, int NbParkingSlots, int GroundWaitingTime,
                 int TrackAccessDelay, int AntiCollisionDelay, int LandingTime, int LoopWaitingTime) :
        m_name(std::move(Name)), m_listID(ListID), m_pos(std::move(Pos)), m_nbTracks(NbTracks),
        m_nbParkingSlots(NbParkingSlots), m_groundWaitingTime(GroundWaitingTime),
        m_trackAccessDelay(TrackAccessDelay), m_antiCollisionDelay(AntiCollisionDelay),
        m_landingTime(LandingTime), m_loopWaitingTime(LoopWaitingTime) {
    m_lastDepartureTime = 0;
}

Airport::~Airport() = default;

void Airport::addNeighbour(Airport *airport, int distance) {
    if (airport == nullptr) return;
    m_neighbours.emplace_back(airport, distance);
}

int Airport::getDistanceTo(Airport *airport) const {
    for (const auto &neighbour : m_neighbours) {
        if (neighbour.first->getName() == airport->getName()) return neighbour.second;
    }
    return -1;
}

void Airport::print() {
    std::cout << "Airport name: " << m_name << "\nLocation x: " << m_pos.first << "\nLocation y: " << m_pos.second 
              << "\nNumber of runways: " << m_nbTracks << "\nNumber of parking slots: " << m_nbParkingSlots 
              << "\nGround waiting time: " << m_groundWaitingTime << "\nRunway access time: " << m_trackAccessDelay 
              << "\nAnti-collision delay: " << m_antiCollisionDelay << "\nLanding time: " << m_landingTime 
              << "\nIn-flight waiting loop duration: " << m_loopWaitingTime << "\n" << std::endl;
}

void Airport::printNeighbours() {
    std::cout << "[Skyroad from " << m_name << "]" << std::endl;
    if (m_neighbours.empty()) { std::cout << "No neighbours" << std::endl; return; }
    for (const auto &neighbour: m_neighbours) {
        std::cout << m_name << " --> " << neighbour.first->getName() << " | Distance : " << neighbour.second << "km" << std::endl;
    }
    std::cout << std::endl;
}

std::string Airport::getName() const { return m_name; }
Pair Airport::getPos() const { return m_pos; }
const std::vector<std::pair<Airport *, int>>& Airport::getNeighbours() const { return m_neighbours; }
int Airport::getLoopWaitingTime() const { return m_loopWaitingTime; }
int Airport::getListID() const { return m_listID; }
void Airport::setListID(int id) { m_listID = id; }
int Airport::getAntiCollisionDelay() const { return m_antiCollisionDelay; }
void Airport::setAntiCollisionDelay(int mAntiCollisionDelay) { m_antiCollisionDelay = mAntiCollisionDelay; }
int Airport::getLastDepartureTime() const { return m_lastDepartureTime; }
void Airport::setLastDepartureTime(int mLastDepartureTime) { m_lastDepartureTime = mLastDepartureTime; }
