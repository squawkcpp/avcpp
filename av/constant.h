#ifndef CONSTANT_H
#define CONSTANT_H

#include <cstdint>
#include <sstream>
#include <string>

/** @brief av namespace */
namespace av {

/** \brief The loglevel enum. */
enum class LOG_LEVEL {
    /** \brief Show nothing at all; be silent. */
    QUIET=-8,
    /** \brief Only show fatal errors which could lead the process to crash,
               such as an assertion failure. This is not currently used for anything. */
    PANIC=0,
    /** \brief Only show fatal errors. These are errors after which the process absolutely cannot continue. */
    FATAL=8,
    /** \brief Show all errors, including ones which can be recovered from. */
    ERROR=16,
    /** \brief Show all warnings and errors. Any message related to possibly incorrect or unexpected events will be shown. */
    WARNING=24,
    /** \brief Show informative messages during processing. This is in addition to warnings and errors. This is the default value. */
    INFO=32,
    /** \brief Same as info, except more verbose. */
    VERBOSE=40,
    /** \brief Show everything, including debugging information. */
    DEBUGGING=48,
    /** \brief Show debugging information. */
    TRACE=56
};

/** \brief Codec id enum. */
struct CODEC {
    enum Enum { NONE = -1, MP2=0, MP3=1, FLAC=2, AAC=3, VORBIS=4 };
};
/** \brief Codec type enum. */
struct CODEC_TYPE {
    enum Enum { NONE = -1, VIDEO=0, AUDIO=1, DATA=2, SUBTITLE=3, ATTACHMENT=4, NB=5 };
};

/** \brief Sampleformat enum. */
enum SampleFormat {
    SAMPLE_FMT_NONE = -1, SAMPLE_FMT_U8, SAMPLE_FMT_S16, SAMPLE_FMT_S32,
    SAMPLE_FMT_FLT, SAMPLE_FMT_DBL, SAMPLE_FMT_U8P, SAMPLE_FMT_S16P,
    SAMPLE_FMT_S32P, SAMPLE_FMT_FLTP, SAMPLE_FMT_DBLP, SAMPLE_FMT_NB
};

/** \brief Channel Layout enum. */
class ChannelLayout {
public:
    enum Enum {
        CH_LAYOUT_MONO, CH_LAYOUT_STEREO, CH_LAYOUT_2POINT1, CH_LAYOUT_2_1, CH_LAYOUT_SURROUND,
        CH_LAYOUT_3POINT1, CH_LAYOUT_4POINT0, CH_LAYOUT_4POINT1, CH_LAYOUT_2_2, CH_LAYOUT_QUAD,
        CH_LAYOUT_5POINT0, CH_LAYOUT_5POINT1, CH_LAYOUT_5POINT0_BACK, CH_LAYOUT_5POINT1_BACK,
        CH_LAYOUT_6POINT0, CH_LAYOUT_6POINT0_FRONT, CH_LAYOUT_HEXAGONAL, CH_LAYOUT_6POINT1,
        CH_LAYOUT_6POINT1_BACK, CH_LAYOUT_6POINT1_FRONT, CH_LAYOUT_7POINT0, CH_LAYOUT_7POINT0_FRONT,
        CH_LAYOUT_7POINT1, CH_LAYOUT_7POINT1_WIDE, CH_LAYOUT_7POINT1_WIDE_BACK, CH_LAYOUT_OCTAGONAL,
        CH_LAYOUT_HEXADECAGONAL, CH_LAYOUT_STEREO_DOWNMIX
    };
    static int64_t get ( ChannelLayout::Enum format );
};

/** @brief convert milliseconds to human readable format. */
inline std::string time_to_string ( uint64_t playtime /** @param playtime the playtime in milliseconds */ ) {
    int _seconds = playtime / 1000;
    std::stringstream ss;

    if ( _seconds >= 3600 ) {
        int hours = _seconds / 3600;

        if ( hours < 10 ) {
            ss << "0";
        }

        ss << hours << ":";
        _seconds = _seconds - ( hours * 3600 );

    } else { ss << "00:"; }

    if ( _seconds >= 60 ) {
        int minutes = _seconds / 60;

        if ( minutes < 10 ) {
            ss << "0";
        }

        ss << minutes << ":";
        _seconds = _seconds - ( minutes * 60 );

    } else { ss << "00:"; }

    if ( _seconds < 10 ) {
        ss << "0";
    }

    ss << _seconds << ".";
    int _millis = ( playtime - ( ( playtime / 1000 ) * 1000 ) );

    if ( _millis < 100 )  { ss << "0"; }

    if ( _millis < 10 )  { ss << "0"; }

    ss << _millis;
    return ss.str();
}
}
#endif // CONSTANT_H
