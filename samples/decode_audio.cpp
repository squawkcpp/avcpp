#include <iostream>

#include <fstream>

#include "../av/format.h"
#include "../av/frame.h"
#include "../av/packet.h"

int main(int argc, char* argv[]) {

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }

    av::Format format( argv[1] );
    if( !!format ) {
        std::cout << "Error: " << format.errc().message() << std::endl;
        exit(1);
    }

    std::ofstream outfile ( argv[2] );
    auto metadata_ = format.metadata();
    std::cout << "Decode: " << argv[1] << ", " <<
                 "playtime: " << format.playtime() << "\n" <<
                  metadata_ << "\n";

    auto _codec = std::find_if( format.begin(), format.end(), av::is_audio );
    std::cout << (*_codec) << std::endl;

    std::error_code errc = format.read( [_codec,&outfile]( av::Packet& package ) {
        if( package.stream_index() == (*_codec)->index() ) {
            (*_codec)->decode( package, [_codec,&outfile]( av::Frame& frame ) {
                //write to out file
                if( (*_codec)->is_planar() ) {
                    for( int i = 0; i < frame.nb_samples(); i++ )
                        for( int ch = 0; ch < (*_codec)->channels(); ch++ )
                            outfile.write( (char*)(frame.data(ch) + frame.data_size()*i), frame.data_size() );
                } else {
                    outfile.write( (char*)frame.extended_data()[0], frame.linesize(0) );
                }
            });
        }
    });
    std::cout << errc.message() << std::endl;
    outfile.close();
    return 0;
}
