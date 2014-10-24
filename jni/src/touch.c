#include <string.h>
#include "touch.h"
#include "log.h"


#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_CANCEL 3
#define ACTION_OUTSIDE 4
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6


typedef struct Touch
{
    TouchID id;
    int num_fingers;
    int max_fingers;
    Finger fingers[10];
} Touch;


static Touch touchDevice = { .id= 0, .num_fingers= 0, .max_fingers= 10 };


static void add_finger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(touchDevice.id != touchid)
    {
        touchDevice.id = touchid;
        touchDevice.num_fingers = 0;
    }

    if(touchDevice.num_fingers < touchDevice.max_fingers)
    {
        Finger* finger = touchDevice.fingers + touchDevice.num_fingers;

        finger->id = fingerid;
        finger->x = x;
        finger->y = y;
        finger->pressure = pressure;

        touchDevice.num_fingers += 1;
    }
}


static void update_finger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(touchDevice.id == touchid)
    {
        int i;
        for(i = 0; i < touchDevice.num_fingers; ++i)
        {
            Finger * finger = touchDevice.fingers + i;
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


static void remove_finger(TouchID touchid, FingerID fingerid, float x, float y, float pressure)
{
    if(touchDevice.id == touchid)
    {
        int i;
        for(i = 0; i < touchDevice.num_fingers; ++i)
        {
            Finger * finger = touchDevice.fingers + i;
            if(finger->id == fingerid)
            {
                touchDevice.num_fingers -= 1;
                memcpy(finger, finger+1, sizeof(Finger) * (touchDevice.num_fingers-i));
                break;
            }
        }
    }
}


void touch_event(TouchID touch_device_id, FingerID pointer_finger_id, int action, float x, float y, float p)
{
    switch(action)
    {
        case ACTION_DOWN:
        case ACTION_POINTER_DOWN:
            add_finger(touch_device_id, pointer_finger_id, x, y, p);
            Log("[touch] begin: %d", touchDevice.num_fingers);
            break;
        case ACTION_MOVE:
            if(pointer_finger_id < 0)
            {
                Log("[touch] move: %d", touchDevice.num_fingers);
            }
            else
            {
                update_finger(touch_device_id, pointer_finger_id, x, y, p);
            }
            break;
        case ACTION_UP:
        case ACTION_POINTER_UP:
            remove_finger(touch_device_id, pointer_finger_id, x, y, p);
            Log("[touch] end: %d", touchDevice.num_fingers);
            break;
        default:
            break;
    }
}


void reset_touch()
{
    touchDevice.id = 0;
    touchDevice.num_fingers = 0;
}
