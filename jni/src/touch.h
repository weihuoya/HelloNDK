#ifndef _TOUCH_H_
#define _TOUCH_H_


typedef int TouchID;
typedef int FingerID;


typedef struct Finger
{
    FingerID id;
    float x;
    float y;
    float pressure;
} Finger;


void touch_event(TouchID touch_device_id, FingerID pointer_finger_id, int action, float x, float y, float p);

void reset_touch();


#endif /* _TOUCH_H_ */
