#pragma once
#include <iostream>
#include <vector>
#include <glad.h>
#include <glfw3.h>

enum EventType {
    MouseDown,
    MouseUp,
    MouseMove,
    KeyDown,
    KeyUp,
    _eventTypeLen
};

class Event {
    
};

typedef void (*_fnEPtr)(Event);

struct _evContainer {
    _fnEPtr fn;
    long handle;
    _evContainer(long hnd) : handle(hnd) {};
};

class Window {
private:
    std::vector<_evContainer> _listeners[_eventTypeLen] = {};
    static long _cHandle;
    GLFWwindow *wHandle;
    void  intCreate();
    bool running = false;
public:
    static bool winIni();
    int w, h;
    const char *title;

    Window() {}; //default constructor thingy
    Window(std::string title, int w, int h) : w(w), h(h), title(title.c_str()) {
        this->intCreate();
    }
    bool Update();
    bool isRunning();
    long AddEventListener(enum EventType ty, void (*fn)(Event));
    bool CancelEventListener(long handle);
};