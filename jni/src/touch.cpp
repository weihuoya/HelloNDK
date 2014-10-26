#include <string.h>
#include "glwrapper.h"
#include "touch.h"


#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_CANCEL 3
#define ACTION_OUTSIDE 4
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6



Touch::Touch() : id_(0), size_(0)
{
}

Touch::~Touch()
{
}

Touch * Touch::instance()
{
    static Touch touch;
    return &touch;
}

void Touch::initialize()
{
}

void Touch::addFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(this->id_ != touchid)
    {
        this->id_ = touchid;
        this->size_ = 0;
    }

    if(this->size_ < TOUCH_MAX_FINGERS)
    {
        Finger* finger = this->fingers_ + this->size_;

        finger->id = fingerid;
        finger->x = x;
        finger->y = y;
        finger->pressure = pressure;

        this->size_ += 1;
    }
}


void Touch::updateFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(this->id_ == touchid)
    {
        int i;
        for(i = 0; i < this->size_; ++i)
        {
            Finger * finger = this->fingers_ + i;
            if(finger->id == fingerid)
            {
                finger->x = x;
                finger->y = y;
                finger->pressure = pressure;
                break;
            }
        }
    }
}


void Touch::removeFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(this->id_ == touchid)
    {
        int i;
        for(i = 0; i < this->size_; ++i)
        {
            Finger * finger = this->fingers_ + i;
            if(finger->id == fingerid)
            {
                this->size_ -= 1;
                memcpy(finger, finger+1, sizeof(Finger) * (this->size_-i));
                break;
            }
        }
    }
}


void Touch::touchEvent(TouchID touchId, FingerID fingerId, int action, float x, float y, float p)
{
    switch(action)
    {
        case ACTION_DOWN:
        case ACTION_POINTER_DOWN:
            addFinger(touchId, fingerId, x, y, p);
            logWrite("[touch] begin: %d", this->size_);
            break;
        case ACTION_MOVE:
            if(fingerId < 0)
            {
                logWrite("[touch] move: %d", this->size_);
            }
            else
            {
                updateFinger(touchId, fingerId, x, y, p);
            }
            break;
        case ACTION_UP:
        case ACTION_POINTER_UP:
            removeFinger(touchId, fingerId, x, y, p);
            logWrite("[touch] end: %d", this->size_);
            break;
        default:
            break;
    }
}


void Touch::reset()
{
    this->id_ = 0;
    this->size_ = 0;
}
