#ifndef VTIMER_H
#define VTIMER_H


#include "vcallback.h"
#include <chrono>
#include <memory>

//=======================================================================================
class VTimer final
{
public:
    VCallBack<int> timeout_count;     // argument -- count of elapsed episodes.
    VCallBack<>    timeout;           // simple, without elapsed episodes.

    explicit VTimer();
    ~VTimer();

    void start( const std::chrono::nanoseconds& ns );
    void singleshot( const std::chrono::nanoseconds& ns );

    template<typename Duration>
    void start( const Duration& duration );

    template<typename Duration>
    void singleshot( const Duration& duration );

    void stop();

private:    
    class Pimpl; std::unique_ptr<Pimpl> p;
};
//=======================================================================================
//      IMPLEMENTATION
//=======================================================================================
template<typename Duration>
void VTimer::start( const Duration& duration )
{
    start( std::chrono::duration_cast<std::chrono::nanoseconds>(duration) );
}
//=======================================================================================
template<typename Duration>
void VTimer::singleshot( const Duration& duration )
{
    singleshot( std::chrono::duration_cast<std::chrono::nanoseconds>(duration) );
}
//=======================================================================================

#endif // VTIMER_H
