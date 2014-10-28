#include <math.h>
#include <stdlib.h>

#include "glwrapper.h"
#include "recognizer.h"
#include "glcontext.h"
#include "touch.h"


Recognizer::Recognizer() : move_(false), scale_(false), rotate_(false)
{
}

Recognizer::~Recognizer()
{
}

Recognizer * Recognizer::instance()
{
    static Recognizer recognizer;
    return &recognizer;
}

void Recognizer::recognize(const Input * input)
{
    int numFingers = input->fingers.size();
    int eventType = input->type;

    if(eventType == Input::INPUT_START)
    {
        if(input->isFirst)
        {
            GLContext::instance()->beginTransform();
        }
    }
    else if(eventType == Input::INPUT_MOVE)
    {
        if(numFingers == 1)
        {
            if(!move_)
            {
                if(input->distance > 16.0)
                {
                    move_ = true;
                }
            }

            if(move_)
            {
                logWrite("[touch] move: %.02f, %.02f, %.02f, %.02f", input->deltaX, input->deltaY, input->angle, input->distance);
                GLContext::instance()->translate(input->deltaX/100., input->deltaY/100.);
            }
        }
        else if(numFingers == 2)
        {
            if(!rotate_)
            {
                if(input->rotation > 16.0)
                {
                    rotate_ = true;
                }
            }

            if(!scale_ && !rotate_)
            {
                if(input->scale > 1.15 || input->scale < 0.95)
                {
                    scale_ = true;
                }
            }

            if(rotate_)
            {
                logWrite("[touch] rotate: %.02f", input->rotation);
                GLContext::instance()->rotate(input->scale * 90., input->scale, input->scale);
            }
            else if(scale_)
            {
                logWrite("[touch] pinch: %.02f", input->scale);
                GLContext::instance()->scale(input->scale);
            }
            else
            {
                logWrite("[touch] pan: %.02f", input->distance);
            }
        }
    }
    else if(eventType == Input::INPUT_END)
    {
        if(numFingers == 1)
        {
            if(input->deltaTime < 250)
            {
                logWrite("[touch] tab: %.02f, %.02f", input->centerX, input->centerY);
            }
        }
    }
}


void Recognizer::reset()
{
    move_ = false;
    scale_ = false;
    rotate_ = false;
}

