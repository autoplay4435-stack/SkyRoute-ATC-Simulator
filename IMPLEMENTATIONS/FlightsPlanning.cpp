#include "../HEADERS/FlightsPlanning.h"

FlightsPlanning::FlightsPlanning(Aircraft *aircraft) {
    m_hasStarted = false; m_hasArrived = false; m_isDirect = false; m_distance = 0; m_nbSteps = 0;
    m_currentStep = 0; m_departureTime = 0; m_departure = nullptr; m_arrival = nullptr;
    m_currentAirport = nullptr; m_aircraft = aircraft;
}

FlightsPlanning::~FlightsPlanning() = default;

void FlightsPlanning::update(std::vector<std::vector<Cell *>> &cells, int ut) {
    if (m_hasStarted && !m_hasArrived) {
        Astar(cells, ut);
        if (m_path.empty()) return;
        Pair a_pos = m_aircraft->getPos();
        int speed = m_aircraft->getSpeed(cells[a_pos.first][a_pos.second]);
        for (int i = 1; i <= speed && i < static_cast<int>(m_path.size()); ++i) {
            m_aircraft->setPos(m_path[i]->getPos());
        }
        Pair updated_pos = m_aircraft->getPos();
        if (m_currentStep + 1 < static_cast<int>(m_steps.size()) && updated_pos == m_steps[m_currentStep + 1]->getPos()) {
            if (m_currentStep + 1 < m_nbSteps - 1) m_currentStep++;
            if (randomInt(0, 100) < 10) m_aircraft->doubleConsumption();
        }
        if (updated_pos == m_arrival->getPos()) {
            m_hasArrived = true;
            std::cout << "An aircraft has arrived at " << m_arrival->getName() << " at " << conversionUTHeure(ut) << std::endl;
        }
    }
}

std::vector<Airport *> FlightsPlanning::dijkstra(const std::vector<Airport *> &airportList) {
    std::vector<Airport *> error(1, nullptr);
    if (m_departure == nullptr || m_arrival == nullptr || m_departure == m_arrival) return error;

    int tankCapacity = m_aircraft->getTankCapacity();
    std::vector<bool> visited(airportList.size(), false);
    std::vector<Airport *> predecesor(airportList.size(), nullptr);
    std::vector<int> distances(airportList.size(), std::numeric_limits<int>::max());
    distances[m_departure->getListID()] = 0;
    std::priority_queue<Pair, std::vector<Pair>, std::less<>> queue;
    queue.push({0, m_departure->getListID()});

    while (!queue.empty()) {
        int idCurrentAirport = queue.top().second;
        queue.pop();
        if (visited[idCurrentAirport]) continue;
        visited[idCurrentAirport] = true;
        for (const auto &neighbour: airportList[idCurrentAirport]->getNeighbours()) {
            if (!visited[neighbour.first->getListID()]) {
                if (distances[idCurrentAirport] + neighbour.second < distances[neighbour.first->getListID()]) {
                    if (neighbour.second <= tankCapacity) {
                        distances[neighbour.first->getListID()] = distances[idCurrentAirport] + neighbour.second;
                        predecesor[neighbour.first->getListID()] = airportList[idCurrentAirport];
                    }
                }
                queue.push({distances[neighbour.first->getListID()], neighbour.first->getListID()});
            }
        }
    }

    std::vector<Airport *> toReturn;
    toReturn.push_back(m_arrival);
    while (toReturn.back()->getName() != m_departure->getName()) {
        int temp = toReturn.back()->getListID();
        if (predecesor[temp] == nullptr) return error;
        toReturn.push_back(predecesor[temp]);
    }
    std::reverse(toReturn.begin(), toReturn.end());
    if (toReturn.size() > 1) {
        displayPath(toReturn);
        m_currentStep = 0;
        m_aircraft->setPos(m_departure->getPos());
    } else {
        m_currentStep = -1;
        return error;
    }
    return toReturn;
}

void FlightsPlanning::Astar(std::vector<std::vector<Cell *>> &cells, int ut) {
    m_path.clear();
    Pair source = m_steps[m_currentStep + 1]->getPos();
    Pair destination = m_aircraft->getPos();
    if (source == destination) return;
    std::vector<std::vector<bool>> closedList(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
    for (auto &col: cells) for (auto &c: col) c->resetAstarValues();
    int x = source.first; int y = source.second;
    cells[x][y]->setFCost(0); cells[x][y]->setGCost(0); cells[x][y]->setParent({x, y});
    std::priority_queue<Tuple, std::vector<Tuple>, std::greater<Tuple>> openList;
    openList.emplace(0, x, y);
    while (!openList.empty()) {
        const Tuple &t = openList.top();
        x = std::get<1>(t); y = std::get<2>(t);
        openList.pop();
        closedList[x][y] = true;
        for (auto &neighbour: cells[x][y]->getNeighbours()) {
            if (neighbour->getPos() == destination) {
                cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                int row = destination.first; int col = destination.second;
                while (!(row == source.first && col == source.second)) {
                    m_path.push_back(cells[row][col]);
                    Pair parent = cells[row][col]->getParent();
                    row = parent.first; col = parent.second;
                }
                m_path.push_back(cells[row][col]);
                return;
            } else if (!closedList[neighbour->getPos().first][neighbour->getPos().second] && cells[neighbour->getPos().first][neighbour->getPos().second]->getType() != 0) {
                double gNew = cells[x][y]->getGCost() + 1;
                double fNew = gNew + std::sqrt(std::pow((neighbour->getPos().first - destination.first), 2) + std::pow((neighbour->getPos().second - destination.second), 2));
                if (cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() == -1 || cells[neighbour->getPos().first][neighbour->getPos().second]->getFCost() > fNew) {
                    openList.emplace(fNew, neighbour->getPos().first, neighbour->getPos().second);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setGCost(gNew);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setFCost(fNew);
                    cells[neighbour->getPos().first][neighbour->getPos().second]->setParent({x, y});
                }
            }
        }
    }
}

void FlightsPlanning::displayPath(const std::vector<Airport *> &toDisplay) {
    for (int i = 0; i < static_cast<int>(toDisplay.size()); ++i) {
        std::cout << toDisplay[i]->getName();
        if (i != static_cast<int>(toDisplay.size()) - 1) std::cout << " --> ";
    }
    std::cout << std::endl << std::endl;
}

void FlightsPlanning::lostFuel() {}
void FlightsPlanning::setDeparture(Airport *departure) { m_departure = departure; }
void FlightsPlanning::setArrival(Airport *arrival) { m_arrival = arrival; }
void FlightsPlanning::setStep(const std::vector<Airport *> &steps) { m_steps = steps; }
const std::vector<Airport *> &FlightsPlanning::getSteps() const { return m_steps; }
void FlightsPlanning::setPath(const std::vector<Cell *> &path) { m_path = path; }
const std::vector<Cell *> &FlightsPlanning::getPath() const { return m_path; }
int FlightsPlanning::getDistance() const { return m_distance; }
void FlightsPlanning::setDistance(int newDistance) { m_distance = newDistance; }
void FlightsPlanning::calculateDistance() {
    m_distance = 0;
    for (int i = 0; i < static_cast<int>(m_steps.size()) - 1; ++i) {
        int d = m_steps[i]->getDistanceTo(m_steps[i + 1]);
        if (d > 0) m_distance += d;
    }
}
void FlightsPlanning::autoFillOtherInfo() {
    m_currentAirport = m_departure; m_hasStarted = false; m_hasArrived = false;
    m_departureTime = randomInt(5, 40); m_nbSteps = static_cast<int>(m_steps.size()); m_isDirect = (m_nbSteps == 2);
}
Airport *FlightsPlanning::getDeparture() const { return m_departure; }
Airport *FlightsPlanning::getArrival() const { return m_arrival; }
void FlightsPlanning::clearSteps() { m_steps.clear(); }
Aircraft *FlightsPlanning::getAircraft() const { return m_aircraft; }
int FlightsPlanning::getCurrentStep() const { return m_currentStep; }
long double FlightsPlanning::getRotation() const {
    if (m_currentStep + 1 >= static_cast<int>(m_steps.size())) return 0.0L;
    int x2 = m_steps[m_currentStep + 1]->getPos().first; int y2 = m_steps[m_currentStep + 1]->getPos().second;
    int x1 = m_aircraft->getPos().first; int y1 = m_aircraft->getPos().second;
    long double angle = std::atan2(static_cast<long double>(y2 - y1), static_cast<long double>(x2 - x1));
    return angle * (180.0L / M_PI) + 90.0L;
}
bool FlightsPlanning::hasStarted() const { return m_hasStarted; }
bool FlightsPlanning::hasArrived() const { return m_hasArrived; }
void FlightsPlanning::setHasStarted(bool mHasStarted) { m_hasStarted = mHasStarted; }
void FlightsPlanning::setDepartureTime(int mDepartureTime) { m_departureTime = mDepartureTime; }
void FlightsPlanning::setCurrentStep(int newStep) { m_currentStep = newStep; }
void FlightsPlanning::addStep(Airport *newStep) { m_steps.push_back(newStep); m_nbSteps = static_cast<int>(m_steps.size()); }
void FlightsPlanning::crash() {
    m_hasArrived = true; m_hasStarted = false; m_aircraft->crash();
    Pair crashPos = m_aircraft->getPos();
    std::cout << "\n*** AIRCRAFT CRASHED ***\nLocation : (" << crashPos.first << ", " << crashPos.second << ")" << std::endl;
}
Airport *FlightsPlanning::foundAirportByID(int thisID, const std::vector<Airport *> &airportList) const {
    for (auto &airport : airportList) { if (airport->getListID() == thisID) return airport; }
    return nullptr;
}
std::vector<std::pair<Airport *, int>> FlightsPlanning::dijkstraInFlight(const std::vector<Airport *> &airportList) {
    std::vector<std::pair<Airport *, int>> result;
    if (airportList.empty()) return result;
    Airport *source = m_steps[m_currentStep];
    if (source == nullptr) return result;
    int tankCapacity = m_aircraft->getTankCapacity();
    std::vector<bool> visited(airportList.size(), false);
    std::vector<int> distances(airportList.size(), std::numeric_limits<int>::max());
    distances[source->getListID()] = 0;
    std::priority_queue<Pair, std::vector<Pair>, std::less<>> queue;
    queue.push({0, source->getListID()});
    while (!queue.empty()) {
        int idCurrent = queue.top().second; queue.pop();
        if (visited[idCurrent]) continue;
        visited[idCurrent] = true;
        for (const auto &neighbour : airportList[idCurrent]->getNeighbours()) {
            int neighbourID = neighbour.first->getListID(); int edgeDist = neighbour.second;
            if (!visited[neighbourID] && edgeDist <= tankCapacity) {
                int newDist = distances[idCurrent] + edgeDist;
                if (newDist < distances[neighbourID]) { distances[neighbourID] = newDist; queue.push({newDist, neighbourID}); }
            }
        }
    }
    for (auto &airport : airportList) {
        int id = airport->getListID();
        if (distances[id] != std::numeric_limits<int>::max() && airport != source) result.push_back({airport, distances[id]});
    }
    std::sort(result.begin(), result.end(), [](const std::pair<Airport *, int> &a, const std::pair<Airport *, int> &b) { return a.second < b.second; });
    return result;
}
