#ifndef CONTROLEAERIEN_GROUPE_2_G_AIRPORT_H
#define CONTROLEAERIEN_GROUPE_2_G_AIRPORT_H

#include "Libraries.h"
#include "util.h"

class Airport {
private:
    std::string m_name;
    int m_listID;
    Pair m_pos;
    int m_nbTracks;
    int m_nbParkingSlots;
    int m_groundWaitingTime;
    int m_trackAccessDelay;
    int m_antiCollisionDelay;
    int m_lastDepartureTime;
    int m_landingTime;
    int m_loopWaitingTime;
    std::vector<std::pair<Airport*, int>> m_neighbours;

public:
    Airport(std::string Name, int ListID, Pair Pos, int NbTracks, int NbParkingSlots, int GroundWaitingTime,
            int TrackAccessDelay, int AntiCollisionDelay, int LandingTime, int LoopWaitingTime);
    ~Airport();
    void addNeighbour(Airport* airport, int distance);
    int getDistanceTo(Airport* airport) const;
    void print();
    void printNeighbours();
    std::string getName() const;
    Pair getPos() const;
    const std::vector<std::pair<Airport *, int>>& getNeighbours() const;
    int getLoopWaitingTime() const;
    int getListID() const;
    void setListID(int id);
    int getAntiCollisionDelay() const;
    void setAntiCollisionDelay(int mAntiCollisionDelay);
    int getLastDepartureTime() const;
    void setLastDepartureTime(int mLastDepartureTime);
};
#endif
