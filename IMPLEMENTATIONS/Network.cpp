
#include "../HEADERS/Network.h"

Network::Network() {
    m_cells.resize(GRID_WIDTH);
    for (int x = 0; x < static_cast<int>(m_cells.size()); ++x) {
        m_cells[x].resize(GRID_HEIGHT);
        for (int y = 0; y < static_cast<int>(m_cells[x].size()); ++y) {
            m_cells[x][y] = new Cell({x, y});
        }
    }
    for (auto &cell: m_cells) for (auto &c: cell) c->addNeighbours(m_cells, true);

    int centrage = 20;
    for (int i = 0; i < randomInt(10, 15); ++i) {
        m_disruptionList.push_back(new Disruption({randomInt(centrage, GRID_WIDTH - centrage),
                                                   randomInt(centrage, GRID_HEIGHT - centrage)},
                                                  randomInt(1, CELL_SIZE / 2)));
    }
}

Network::~Network() {
    for (auto &cell: m_cells) for (auto &c: cell) delete c;
    for (auto &airport: m_airportList) delete airport;
    for (auto &aircraft: m_aircraftList) delete aircraft;
    for (auto &planning: m_flightPlanningList) delete planning;
    for (auto &disruption: m_disruptionList) delete disruption;
}

void Network::update(int ut) {
    for (auto &disruption: m_disruptionList) disruption->update();

    for (auto &planning: m_flightPlanningList) {
        if (!planning->hasStarted() && !planning->hasArrived() && randomInt(0, 100) < 10 &&
            ut - planning->getDeparture()->getLastDepartureTime() >=
            planning->getDeparture()->getAntiCollisionDelay()) {
            planning->setHasStarted(true);
            planning->setDepartureTime(ut);
            planning->getDeparture()->setLastDepartureTime(ut);
            std::cout << "An aircraft has departed from " << planning->getDeparture()->getName() << " at " << conversionUTHeure(ut) << std::endl;
        }
        planning->update(m_cells, ut);
    }

    static int totalCrashes = 0;
    for (auto &planning : m_flightPlanningList) {
        Aircraft *air = planning->getAircraft();
        if (!planning->hasStarted() || planning->hasArrived() || air->isCrash()) continue;
        for (auto &disruption : m_disruptionList) {
            Pair dPos = disruption->getPos();
            Pair aPos = air->getPos();
            int power = disruption->getPower();
            if (std::abs(aPos.first - dPos.first) <= power && std::abs(aPos.second - dPos.second) <= power) {
                if (totalCrashes < 3 && randomInt(0, 100) < 20) {
                    planning->crash();
                    totalCrashes++;
                    std::cout << "An aircraft crashed inside a storm!" << std::endl;
                }
                break;
            }
        }
    }
    changeAltitude();
    appelerDijkstra();
    for (int i = 0; i < static_cast<int>(m_aircraftList.size()); ++i) {
        if (randomInt(0, 10000) == 2300 && !m_aircraftList[i]->getFuite()) {
            m_aircraftList[i]->doubleConsumption();
        }
    }
}

void Network::loadData(const std::string &dataPath) {
    std::string name; Pair pos; int nbTracks, nbParkingSlots, groundWaitingTime, trackAccessDelay, antiCollisionDelay, landingTime, loopWaitingTime;
    int distance, size, ordre, nbAircrafts, type, consumption, tankCapacity;
    float speed; int altitude = 10000;
    std::string airport1, airport2;

    std::ifstream ifs{dataPath};
    if (!ifs) throw std::runtime_error("Unable to open file : " + dataPath);

    ifs >> ordre >> size >> nbAircrafts;
    for (int i = 0; i < ordre; ++i) {
        ifs >> name >> pos.first >> pos.second >> nbTracks >> nbParkingSlots >> groundWaitingTime >> trackAccessDelay >> antiCollisionDelay >> landingTime >> loopWaitingTime;
        pos.first  = map(pos.first,  0, 144, 0, GRID_WIDTH);
        pos.second = map(pos.second, 0, 80,  0, GRID_HEIGHT);
        m_airportList.push_back(new Airport(name, i, pos, nbTracks, nbParkingSlots, groundWaitingTime, trackAccessDelay, antiCollisionDelay, landingTime, loopWaitingTime));
    }
    for (int h = 0; h < size; h++) {
        ifs >> airport1 >> airport2 >> distance;
        for (auto &i: m_airportList) {
            if (i->getName() == airport1) {
                for (auto &j: m_airportList) {
                    if (j->getName() == airport2) {
                        i->addNeighbour(j, distance);
                        j->addNeighbour(i, distance);
                    }
                }
            }
        }
    }
    for (int i = 0; i < nbAircrafts; ++i) {
        ifs >> type;
        switch (type) {
            case 1: consumption = 80;  tankCapacity = 7000;  speed = 2; break;
            case 2: consumption = 100; tankCapacity = 10000; speed = 3; break;
            case 3: consumption = 120; tankCapacity = 15000; speed = 4; break;
            default: consumption = 0; tankCapacity = 0; speed = 0; break;
        }
        m_aircraftList.push_back(new Aircraft(type, consumption, tankCapacity, speed, altitude));
    }
}

void Network::print() {
    for (auto &i: m_airportList) { i->print(); i->printNeighbours(); }
    for (int i = 0; i < static_cast<int>(m_aircraftList.size()); ++i) {
        std::cout << "aircraft : " << i << std::endl;
        m_aircraftList[i]->print();
    }
}

void Network::initiateFlightPlanning() {
    m_flightPlanningList.resize(m_aircraftList.size());
    for (int j = 0; j < static_cast<int>(m_aircraftList.size()); j++) {
        m_flightPlanningList[j] = newFlightPlannings(m_aircraftList[j]);
    }
}

FlightsPlanning *Network::newFlightPlannings(Aircraft *withThisAirCraft) {
    auto *newFP = new FlightsPlanning(withThisAirCraft);
    do {
        do {
            newFP->clearSteps();
            newFP->setDeparture(m_airportList[randomInt(0, m_airportList.size() - 1)]);
            newFP->setArrival(m_airportList[randomInt(0, m_airportList.size() - 1)]);
        } while (newFP->getDeparture() == newFP->getArrival());
        newFP->setStep(newFP->dijkstra(m_airportList));
    } while (newFP->getSteps().back() != newFP->getArrival());
    newFP->calculateDistance();
    newFP->autoFillOtherInfo();
    return newFP;
}

void Network::defineCellTypes() {
    for (auto &cell: m_cells) for (auto &c: cell) c->setType(1);
    for (auto &disruption: m_disruptionList) {
        int power = disruption->getPower();
        for (int x = -power + disruption->getPos().first; x < power + disruption->getPos().first + 1; ++x) {
            for (int y = -power + disruption->getPos().second; y < power + disruption->getPos().second + 1; ++y) {
                if (inbound(x, y, 0, 0, GRID_WIDTH, GRID_HEIGHT)) m_cells[x][y]->setType(2);
            }
        }
    }
    for (auto &airport: m_airportList) {
        int time = airport->getLoopWaitingTime();
        for (int x = -time + airport->getPos().first; x < time + airport->getPos().first + 1; ++x) {
            for (int y = -time + airport->getPos().second; y < time + airport->getPos().second + 1; ++y) {
                if (inbound(x, y, 0, 0, GRID_WIDTH, GRID_HEIGHT)) m_cells[x][y]->setType(3);
            }
        }
    }
}

void Network::changeAltitude() {
    int n = static_cast<int>(m_aircraftList.size());
    for (int i = 0; i < n - 1; ++i) {
        bool iAtAirport = false;
        for (auto &ap : m_airportList) { if (m_aircraftList[i]->getPos() == ap->getPos()) { iAtAirport = true; break; } }
        if (iAtAirport) continue;
        for (int j = i + 1; j < n; ++j) {
            bool jAtAirport = false;
            for (auto &ap : m_airportList) { if (m_aircraftList[j]->getPos() == ap->getPos()) { jAtAirport = true; break; } }
            if (jAtAirport) continue;
            if (m_aircraftList[i]->getPos() == m_aircraftList[j]->getPos()) {
                m_aircraftList[i]->setAltitude(8000);
                m_aircraftList[j]->setAltitude(12000);
            }
        }
    }
}

void Network::appelerDijkstra() {
    for (auto &air : m_flightPlanningList) {
        // FIX: Skip aircraft that have already crashed or arrived
        if (air->hasArrived()) continue;
        if (!air->getAircraft()->getFuite()) continue;

        std::vector<std::pair<Airport *, int>> result = air->dijkstraInFlight(m_airportList);
        if (result.empty()) continue;
        int tempID = -1; int lowerDistance = std::numeric_limits<int>::max();
        for (int i = 0; i < static_cast<int>(result.size()); ++i) {
            if (result[i].first != air->getSteps()[air->getCurrentStep()]) {
                if (result[i].second < lowerDistance) { tempID = i; lowerDistance = result[i].second; }
            }
        }
        if (tempID == -1) continue;
        Airport *newDirection = result[tempID].first;
        Airport *OldDeparture = air->getSteps()[air->getCurrentStep()];
        Airport *OldArrival = air->getSteps()[air->getCurrentStep() + 1];
        air->clearSteps(); air->setCurrentStep(0); air->addStep(OldDeparture); air->addStep(newDirection);
        
        if (result[tempID].second > air->getAircraft()->getTankCapacity() || air->getSteps()[1] == OldArrival) {
            air->crash(); // Mark as crashed and stop the flight loop
        }
    }
}

std::vector<Airport*> Network::getAirportList() { return m_airportList; }
void Network::addAircraft(Aircraft *newAirCraft) { m_aircraftList.push_back(newAirCraft); }
std::vector<FlightsPlanning *> Network::getFlightsPlanningList() { return m_flightPlanningList; }
void Network::addFlightPlanning(FlightsPlanning* newFP) { m_flightPlanningList.push_back(newFP); }