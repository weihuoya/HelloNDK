#include <algorithm>
#include <string.h>

#include "glwrapper.h"
#include "recognizer.h"
#include "touch.h"
#include "timer.h"

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_CANCEL 3
#define ACTION_OUTSIDE 4
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6

#define COMPUTE_INTERVAL 25


static inline float getDistance(float x, float y)
{
    return sqrt(x * x + y * y);
}

static inline float getRotate(float x, float y)
{
    return atan2(y, x) * 180 / M_PI;
}

static inline uint32_t getDirection(float x, float y)
{
    uint32_t direction = Input::DIRECTION_NONE;

    if(x == y)
    {
        direction = Input::DIRECTION_NONE;
    }
    else if(abs(x) >= abs(y))
    {
        direction = x > 0 ? Input::DIRECTION_LEFT : Input::DIRECTION_RIGHT;
    }
    else
    {
        direction = y > 0 ? Input::DIRECTION_UP : Input::DIRECTION_DOWN;
    }

    return direction;
}


////////////////////////////////////////////////////////////////////////////////
// Touch
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
    int event = -1;

    switch(action)
    {
        case ACTION_DOWN:
        case ACTION_POINTER_DOWN:
            addFinger(touchId, fingerId, x, y, p);
            logWrite("[touch] begin: %d", this->size_);
            event = EVENT_BEGAN;
            break;
        case ACTION_MOVE:
            if(fingerId < 0)
            {
                logWrite("[touch] move: %d", this->size_);
                event = EVENT_MOVED;
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
            event = EVENT_ENDED;
            break;
        case ACTION_CANCEL:
            reset();
            break;
        default:
            break;
    }

    if(event > 0)
    {
        handleTouch(event);
    }
}

void Touch::handleTouch(int action)
{
    int i = 0;
    int eventType;
    int numFingers = this->size_;
    const Finger * finger = NULL;
    float deltaX, deltaY;
    bool isFirst = false;
    bool isFinal = false;

    std::shared_ptr<Input> input(new Input);

    switch(action)
    {
        case EVENT_BEGAN:
            isFirst = (prevFingers_ == 0);
            eventType = Input::INPUT_START;
            break;
        case EVENT_MOVED:
            eventType = Input::INPUT_MOVE;
            break;
        case EVENT_ENDED:
            isFinal = (numFingers == 0);
            eventType = Input::INPUT_END;
            break;
        case EVENT_CANCELED:
            isFinal = true;
            eventType = Input::INPUT_CANCEL;
            break;
        default:
            break;
    }

    // compute input
    input->isFirst = isFirst;
    input->isFinal = isFinal;
    input->type = eventType;

    input->setFingers(this->fingers_, numFingers);

    if(!firstInput_)
    {
        firstInput_ = input;
    }

    if(numFingers == 1)
    {
        finger = input->fingers[0].get();
        input->centerX = finger->x;
        input->centerY = finger->y;

        if(firstMultiple_)
        {
            firstMultiple_.reset();
            firstInput_ = input;
        }
    }
    else if(numFingers > 1)
    {
        float x = 0, y = 0;

        for (i = 0; i < numFingers; ++i)
        {
            finger = input->fingers[i].get();
            x += finger->x;
            y += finger->y;
        }

        input->centerX = x / numFingers;
        input->centerY = y / numFingers;

        if(!firstMultiple_)
        {
            firstMultiple_ = input;
        }
    }

    input->timeStamp = Timer::instance()->getTicks();
    input->deltaTime = input->timeStamp - firstInput_->timeStamp;

    if(firstMultiple_ && numFingers > 1)
    {
        //scale
        float firstDistance;
        float currDistance;

        deltaX = firstMultiple_->fingers[0]->x - firstMultiple_->fingers[1]->x;
        deltaY = firstMultiple_->fingers[0]->y - firstMultiple_->fingers[1]->y;
        firstDistance = getDistance(deltaX, deltaY);

        deltaX = input->fingers[0]->x - input->fingers[1]->x;
        deltaY = input->fingers[0]->y - input->fingers[1]->y;
        currDistance = getDistance(deltaX, deltaY);

        input->scale = currDistance / firstDistance;

        //rotate
        float firstRotate;
        float currRotate;

        deltaX = firstMultiple_->fingers[0]->x - firstMultiple_->fingers[1]->x;
        deltaY = firstMultiple_->fingers[0]->y - firstMultiple_->fingers[1]->y;
        firstRotate = getRotate(deltaX, deltaY);

        deltaX = input->fingers[0]->x - input->fingers[1]->x;
        deltaY = input->fingers[0]->y - input->fingers[1]->y;
        currRotate = getRotate(deltaX, deltaY);

        input->rotation = currRotate - firstRotate;


        input->deltaX = deltaX = input->centerX - firstMultiple_->centerX;
        input->deltaY = deltaY = input->centerY - firstMultiple_->centerY;
    }
    else
    {
        input->deltaX = deltaX = input->centerX - firstInput_->centerX;
        input->deltaY = deltaY = input->centerY - firstInput_->centerY;
    }

    input->angle = getRotate(deltaX, deltaY);
    input->distance = getDistance(deltaX, deltaY);
    input->offsetDirection = getDirection(deltaX, deltaY);

    // compute interval
    if(!lastInterval_)
    {
        lastInterval_ = input;
    }

    float deltaTime = input->timeStamp - lastInterval_->timeStamp;
    float velocity, velocityX, velocityY;
    uint32_t direction;

    if (deltaTime > COMPUTE_INTERVAL || lastInterval_->velocity < 0)
    {
        deltaX = lastInterval_->deltaX - input->deltaX;
        deltaY = lastInterval_->deltaY - input->deltaY;

        velocityX = abs(deltaX / deltaTime);
        velocityY = abs(deltaY / deltaTime);
        velocity = std::max(velocityX, velocityY);

        direction = getDirection(deltaX, deltaY);
    }
    else
    {
        // use latest velocity info if it doesn't overtake a minimum period
        velocity = lastInterval_->velocity;
        velocityX = lastInterval_->velocityX;
        velocityY = lastInterval_->velocityY;
        direction = lastInterval_->direction;
    }

    input->velocity = velocity;
    input->velocityX = velocityX;
    input->velocityY = velocityY;
    input->direction = direction;

    Recognizer::instance()->recognize(input.get());

    prevFingers_ = numFingers;

    if(isFinal)
    {
        logWrite("final reset recognizer");
        Recognizer::instance()->reset();
        firstInput_.reset();
        firstMultiple_.reset();
        lastInterval_.reset();
    }
}


void Touch::reset()
{
    id_ = 0;
    size_ = 0;

    prevFingers_ = 0;

    Recognizer::instance()->reset();

    firstInput_.reset();
    firstMultiple_.reset();
    lastInterval_.reset();
}
