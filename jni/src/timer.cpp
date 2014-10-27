#include "timer.h"


Timer::Timer()
{
    gettimeofday(&startTime_, NULL);
}


Timer::~Timer()
{
}


Timer * Timer::instance()
{
    static Timer timer;
    return &timer;
}


void Timer::initialize()
{
}


unsigned int Timer::getTicks() const
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - startTime_.tv_sec) * 1000 + (now.tv_usec - startTime_.tv_usec) / 1000;
}


double Timer::getTime() const
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000. + now.tv_usec / 1000.;
}


int Timer::setTimeout(const Callback& callback, int interval)
{
    return 0;
}

void Timer::clearTimeout(int id)
{
}


int Timer::setInterval(const Callback &callback, int interval)
{
}

int Timer::clearInterval(int id)
{
    return 0;
}

