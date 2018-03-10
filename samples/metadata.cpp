#include <iostream>

#include "../av/format.h"

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        exit(0);
    }

    std::cout << "Open File: " << argv[1] << "\n";
    av::Format format( argv[1] );
    if( !!format ) {
        std::cout << "Error: " << format.errc().message() << std::endl;
        exit(1);
    }

    for( auto codec : format ) {
        std::cout << (*codec) << "\n";
    }
    auto metadata_ = format.metadata();
    std::cout << metadata_ <<  std::endl;
    return 0;
}
