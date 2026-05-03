#include "../HEADERS/Core.h"

Core::Core() = default;
Core::~Core() = default;

void Core::initialisation() {
    std::cout << "------------------- MENU -----------------" << std::endl;
    std::cout << " 1 : Display airport and aircraft data" << std::endl;
    std::cout << " 2 : Run Simulation (Terminal Output)" << std::endl;
    std::cout << " Enter choice: ";

    int choice;
    if (!(std::cin >> choice)) return;

    if (choice == 1) {
        try {
            m_network.loadData("../DATA/defaultConfig.txt");
            m_network.print();
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
    } 
    else if (choice == 2) {
        try {
            std::cout << "Loading data and starting simulation..." << std::endl;
            m_network.loadData("../DATA/defaultConfig.txt");
            m_network.defineCellTypes();
            m_network.initiateFlightPlanning();

            // Run the simulation for 1000 time units
            // This is where altitude changes, storms, and crashes are processed
            for (int ut = 0; ut < 1000; ++ut) {
                m_network.update(ut);
            }
            std::cout << "Simulation complete." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
    } 
    else {
        std::cout << "Invalid choice. Exiting." << std::endl;
    }
}
