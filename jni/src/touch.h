#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <vector>
#include <memory>

#include "input.h"
#include "recognizer.h"

#define TOUCH_MAX_FINGERS 10

typedef int TouchID;

class Touch
{
public:
    enum EVENT {
        EVENT_BEGAN=1,
        EVENT_MOVED,
        EVENT_ENDED,
        EVENT_CANCELED,
    };

    ~Touch();

    static Touch * instance();

    void initialize();

    void touchEvent(TouchID touchId, FingerID fingerId, int action, float x, float y, float p);

    void reset();

protected:
    void addFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);
    void updateFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);
    void removeFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);

    void handleTouch(int action);

private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Touch);

    Finger fingers_[TOUCH_MAX_FINGERS];

    TouchID id_;
    int size_;

    size_t prevFingers_;
    std::shared_ptr<Input> firstInput_;
    std::shared_ptr<Input> firstMultiple_;
    std::shared_ptr<Input> lastInterval_;

    std::vector< std::shared_ptr<Recognizer> > recognizers_;
};


#endif /* _TOUCH_H_ */
