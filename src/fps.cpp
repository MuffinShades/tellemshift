#include "fps.hpp"

_chrono_time fps_counter::f_start;
_chrono_time fps_counter::p_start;
double fps_counter::t_collection = 0;
int fps_counter::nFrames = 0;
float fps_counter::target_fps = 60;
float fps_counter::fps = 0;