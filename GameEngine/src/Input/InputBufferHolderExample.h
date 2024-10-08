#pragma once
#include <stdio.h>
#include <iostream>
enum InputType {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ATTACK,
    NA

};

struct Input
{
    InputType type;
    int duration;
};

class CircularBufferTester
{
public:


    CircularBufferTester()
    {
        head = 0;
        tail = 0;
        duration = 1;
        arr[MAX_ELEMENTS];
    }

    void AddElement(Input element)
    {
        if ((tail + 1) % MAX_ELEMENTS == head)
        {
            return;
        }
        arr[tail] = element;
        arr[tail].duration = element.duration + 1;
        tail = (tail + 1) % MAX_ELEMENTS;

        std::cout << "Adding Element: " << GetTypeName(element.type) << " Tail is At: " << tail << " Head is At: " << head << std::endl;

        for (int i = 0; i < MAX_ELEMENTS; i++)
        {
            std::cout << "Array at: " << i << " is: " << GetTypeName(arr[i].type) << std::endl;
        }

    }

    void GoThrough()
    {
        if (head == tail) return;


        Input element = arr[head];


        if (element.type == previousInput)
        {
            duration++;
        }
        else
        {
            duration = 1;
        }

        element.duration = duration;

        previousInput = arr[head].type;


        head = (head + 1) % MAX_ELEMENTS;

        std::cout << "Getting Element: " << GetTypeName(element.type) << " head is At: " << head << "Input duration: " << element.duration << std::endl;

    }

    void GoThroughAll()
    {
        for (int i = 0; i < MAX_ELEMENTS; i++)
        {
            GoThrough();
        }
    }

    std::string GetTypeName(InputType inputType)
    {

        std::string result = "";

        switch (inputType)
        {
        case InputType::UP:
            result = "UP";
            break;
        case InputType::DOWN:
            result = "DOWN";
            break;
        case InputType::LEFT:
            result = "LEFT";
            break;
        case InputType::RIGHT:
            result = "RIGHT";
            break;
        case InputType::ATTACK:
            result = "ATTACK";
            break;

        }
        return result;
    }


    int head;
    int tail;
    InputType previousInput;
    int duration;
    const int MAX_ELEMENTS = 5;
    Input arr[];

private:


};