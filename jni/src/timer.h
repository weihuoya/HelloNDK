#ifndef _TIMER_H_
#define _TIMER_H_


#include <sys/time.h>
#include <time.h>

#include <functional>
#include <vector>

#include "glwrapper.h"


class Timer
{
public:
    typedef std::function<void(unsigned int)> Callback;

    typedef struct TimerSlot
    {
        int type;
        int id;
        int interval;
        int timestamp;
        Callback callback;
    } TimerSlot;

    ~Timer();

    static Timer * instance();

    void initialize();

    unsigned int getTicks() const;
    double getTime() const;

    int setTimeout(const Callback& callback, int interval);
    void clearTimeout(int id);

    int setInterval(const Callback &callback, int interval);
    int clearInterval(int id);

private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Timer);

    struct timeval startTime_;
    std::vector<TimerSlot> handlers_;
};


#endif /* _TIMER_H_ */
