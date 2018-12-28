#ifndef VCHRONO_WEEKS_H
#define VCHRONO_WEEKS_H

#include <chrono>

//=======================================================================================
//  Определение недели в терминах C++11
namespace std
{
    namespace chrono
    {
        static constexpr auto _seconds_in_week = 7 * 24 * 3600;
        using weeks = std::chrono::duration<int64_t, std::ratio<_seconds_in_week>>;
    } // chrono namespace

} // std namespace
//=======================================================================================


//=======================================================================================
//  В помощь определения времени навигаторам.
//  Интервал между 01.01.1970 (Unix UTC) и 06.01.1980 (GPS).
//static constexpr std::chrono::seconds gps_to_epoch_secs( 315964800 );
//static constexpr std::chrono::milliseconds ms_between_gps_and_utc( gps_to_epoch_secs );
//static constexpr std::chrono::microseconds us_between_gps_and_utc( gps_to_epoch_secs );
//=======================================================================================



#endif // VCHRONO_WEEKS_H
