#ifndef _TOUCH_H_
#define _TOUCH_H_

#define TOUCH_MAX_FINGERS 10

typedef int TouchID;
typedef int FingerID;


class Touch
{
public:
    typedef struct Finger
    {
        FingerID id;
        float x;
        float y;
        float pressure;
    } Finger;

    ~Touch();

    static Touch * instance();

    void initialize();

    void touchEvent(TouchID touchId, FingerID fingerId, int action, float x, float y, float p);

    void reset();

protected:
    void addFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);
    void updateFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);
    void removeFinger(TouchID touchid, FingerID fingerid, float x, float y, float pressure);

private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Touch);

    TouchID id_;
    int size_;
    Finger fingers_[TOUCH_MAX_FINGERS];
};


#endif /* _TOUCH_H_ */
