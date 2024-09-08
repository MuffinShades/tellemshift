#pragma once
#include <iostream>
#include <chrono>

#define _chrono_time std::chrono::steady_clock::time_point
#define nano_to_fps(d_nano) (1000.0 / ((d_nano) / (double)1e6))
#define __t_now() std::chrono::high_resolution_clock::now()

class fps_counter {
	static _chrono_time f_start, p_start;
	static double t_collection;
	static int nFrames;
	static float target_fps;
public:
	static float fps;
	static void start() {
		f_start = __t_now();
	};
	static double end() {
		auto f_end = __t_now();
		double d_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(f_end - f_start).count();

		//compute fps from timer
		return nano_to_fps(d_nano);
	};
	static double nano_end() {
		auto f_end = __t_now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(f_end - f_start).count();
	}
	static bool f_tick() {
		nFrames++;

		t_collection += nano_end();
		start();

		fps = nano_to_fps(t_collection / (double)nFrames);

		auto f_cur = __t_now();
		const int _ds = std::chrono::duration_cast<std::chrono::seconds>(f_cur - p_start).count();

		if (_ds >= 1) {
			nFrames = 0;
			t_collection = 0;
			p_start = __t_now();
			return true;
		}

		return false;
	}
	static double GetTimeDelta() {
		return target_fps / fps;
	}
	static void SetFpsTarget(double fTarget) {
		if (fTarget <= 0.0)
			return;
		target_fps = fTarget;
	}
};