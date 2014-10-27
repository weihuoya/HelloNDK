#ifndef __hellosdl__input__
#define __hellosdl__input__

#include <vector>
#include <memory>
#include <utility>

typedef int FingerID;

typedef struct Finger
{
    FingerID id;
    float x;
    float y;
    float pressure;
} Finger;


struct Input
{
    enum DIRECTION {
        DIRECTION_NONE,
        DIRECTION_UP,
        DIRECTION_RIGHT,
        DIRECTION_DOWN,
        DIRECTION_LEFT,
        DIRECTION_HORIZONTAL,
        DIRECTION_VERTICAL,
        DIRECTION_ALL,
    };

    enum INPUT_TYPE {
        INPUT_START,
        INPUT_MOVE,
        INPUT_END,
        INPUT_CANCEL,
    };

    Input();
    Input(const Input& rhs);
    ~Input();

    void swap(Input& rhs);
    //void swap(Input&& rhs);
    void dump() const;

    void setFingers(const Finger * fingers, int numFingers);
    std::vector<std::shared_ptr<Finger>> fingers;

    uint32_t type;
    uint32_t timeStamp;

    float angle;
    float distance;
    float scale;
    float rotation;

    uint32_t deltaTime;
    uint32_t direction;

    float velocity;
    float velocityX;
    float velocityY;

    uint32_t offsetDirection;
    float deltaX;
    float deltaY;

    float centerX;
    float centerY;

    bool isFirst;
    bool isFinal;
};


#endif /* defined(__HelloSDL__input__) */
