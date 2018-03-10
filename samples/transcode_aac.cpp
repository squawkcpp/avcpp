#include <iostream>

#include "../av/format.h"
#include "../av/resample.h"

int main(int argc, char* argv[]) {

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }

    av::Format format( argv[1] );
    auto metadata_ = format.metadata();
    std::cout << "Decode: " << argv[1] << ", " <<
                 "playtime: " << format.playtime() << "\n" <<
                  metadata_ << "\n";

    auto _codec = std::find_if( format.begin(), format.end(), av::is_audio );
    std::cout << *(*_codec) << std::endl;

    av::Codec _target_codec( av::CODEC::MP3, av::SAMPLE_FMT_FLTP, av::Options( { { "ar", "8000" }, {"ac", "2"}, {"b", "128000"} } ) );
    std::cout << _target_codec << std::endl;

    av::Resample _resampler(
        (*_codec)->channel_layout(), (*_codec)->sample_fmt(), (*_codec)->sample_rate(),
        _target_codec.channel_layout(), _target_codec.sample_fmt(), _target_codec.sample_rate() );

    av::Format target_format( argv[2], av::Format::WRITE );

    std::error_code errc = format.read( [&]( av::Packet& package ) {
        if( package.stream_index() == (*_codec)->index() ) {
            (*_codec)->decode( package, [&]( av::Frame& frame ) {
                //transcode frame
                errc = _resampler.resample( frame, _target_codec.frame_size(), [&]( av::Frame& resampled_frame ) {
            std::cout << "encode package " << std::endl;
                    errc = _target_codec.encode( resampled_frame, [&]( av::Packet& package ) {
            std::cout << "write package " << std::endl;
                       target_format.write( package );
                    });
    std::cout << errc.message() << std::endl;
                });
            });
        }
    });
    std::cout << errc.message() << std::endl;
}
