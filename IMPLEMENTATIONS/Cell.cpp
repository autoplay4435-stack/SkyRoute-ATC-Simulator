#include "../HEADERS/Cell.h"

Cell::Cell(Pair pos) : m_pos(pos){
    m_parent = {-1, -1};
    m_Fcost = -1;
    m_Gcost = -1;
    m_type = 1;
}

Cell::~Cell() = default;

void Cell::addNeighbours(const std::vector<std::vector<Cell *>> &cells, bool addDiagonals) {
    if (m_pos.first < GRID_WIDTH - 1) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second]);
    if (m_pos.first > 0) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second]);
    if (m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first][m_pos.second + 1]);
    if (m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first][m_pos.second - 1]);
    if (!addDiagonals) return;
    if (m_pos.first < GRID_WIDTH - 1 && m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second + 1]);
    if (m_pos.first < GRID_WIDTH - 1 && m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first + 1][m_pos.second - 1]);
    if (m_pos.first > 0 &&  m_pos.second < GRID_HEIGHT - 1) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second + 1]);
    if (m_pos.first > 0 &&  m_pos.second > 0) m_neighbours.push_back(cells[m_pos.first - 1][m_pos.second - 1]);
}

void Cell::resetAstarValues() {
    m_parent = {-1, -1};
    m_Fcost = -1;
    m_Gcost = -1;
}

void Cell::print() const {
    std::cout << "m_pos.first: " << m_pos.first << "\nm_pos.second: " << m_pos.second << std::endl;
}

const Pair &Cell::getPos() const { return m_pos; }
Pair Cell::getParent() const { return m_parent; }
void Cell::setParent(Pair parent) { m_parent = parent; }
double Cell::getFCost() const { return m_Fcost; }
void Cell::setFCost(double mFCost) { m_Fcost = mFCost; }
double Cell::getGCost() const { return m_Gcost; }
void Cell::setGCost(double mGCost) { m_Gcost = mGCost; }
int Cell::getType() const { return m_type; }
void Cell::setType(int type) { m_type = type; }
const std::vector<Cell *> &Cell::getNeighbours() const { return m_neighbours; }