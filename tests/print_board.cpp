#include "../src/fen.h"

int main() {
    Position position;

    std::cout << "Enter FEN or 0 for default fen: ";
    
    std::string input;
    std::getline(std::cin, input);

    auto error = FenUtils::setFen(position, (input == "0") ?  DEFAULT_FEN : input);
    if (error) {
        std::cerr << error->what() << std::endl;
        return 1;
    } 

    std::cout << "Board: " << std::endl;
    PositionUtils::printBoard(position);

    return 0;
    
}
