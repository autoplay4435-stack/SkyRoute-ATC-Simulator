#include "HEADERS/Libraries.h"
#include "HEADERS/Core.h"
#include <iostream>

int main() {
    try {
        Core core; 
        core.initialisation();
    } 
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    std::cout << "\nProgram finished. Press Enter to close..." << std::endl;
    std::cin.get(); 
    return 0;
}
