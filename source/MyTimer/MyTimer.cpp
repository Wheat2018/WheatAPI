// MyTimer.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include <iomanip>
#include <sstream>

using namespace std;

#ifdef window
#include <Windows.h>

time_t MyTimer::WinGetClock() {
	LARGE_INTEGER  t;
	QueryPerformanceCounter(&t);
	return t.QuadPart;
}

void MyTimer::WinInit() {
	LARGE_INTEGER temp;
	QueryPerformanceFrequency(&temp);
	clocks_per_second = temp.QuadPart;
}
#endif

time_t MyTimer::GetClock() {
#ifdef window
	return WinGetClock();
#else
#ifdef linux
	return clock();
#endif //linux
#endif //window
}

MyTimer::MyTimer() {
#ifdef window
	WinInit();
#else
#ifdef linux
	clocks_per_second = CLOCKS_PER_SEC;
#endif //linux
#endif //window
}

void MyTimer::DoInFPS(float fps, int argc, void* argv[], void(*func)(int argc, void** argv, float call_times), bool fps_limit, bool focus_on_interval) {
	if (last_call_times == 0) doInFPS_startTime = GetTime();
	float now_call_times = (GetTime() - doInFPS_startTime) / 1000.0f * fps;
	if (fps_limit) {
		if (now_call_times > last_call_times) {
			func(argc, argv, now_call_times);
			last_call_times = focus_on_interval ? (now_call_times + 1) : (floor(now_call_times + 1));
		}
	}
	else {
		func(argc, argv, now_call_times);
		last_call_times = now_call_times;
	}
}

void MyTimer::DoInFPS(float fps, std::function<void(float)> func, bool fps_limit, bool focus_on_interval) {
	DoInFPS(fps, 1, (void**)& func,
		[](int, void** argv, float call_times) {
			std::function<void(float)>& func = *((std::function<void(float)>*) argv);
			func(call_times);
		},
		fps_limit, focus_on_interval);
}


float MyTimer::GetTime() { return (GetClock() - startClock) * 1000.0 / clocks_per_second; }

void MyTimer::MarkFPS() {
	float nowTime = GetTime();
	fpsArr[(fpsCounter = (fpsCounter + 1) % aveTimes)] = 1 / (nowTime - lastTime) * 1000;
	lastTime = nowTime;
}

float MyTimer::GetFPS() {
	float fps = 0;
	for (size_t i = 0; i < aveTimes; i++) fps += fpsArr[i];
	return fps / aveTimes;
}

void MyTimer::Start() {
	last_call_times = 0;

	lastTime = 0;
	memset(fpsArr, 0, sizeof(fpsArr[0]) * aveTimes);
	fpsCounter = 0;

	states.clear();
	times.clear();
	states.push_back("Start");
	times.push_back(0);
	startClock = GetClock();
}

inline void GoSleep(unsigned int ms) {
#ifdef window
	Sleep(ms);
#else
#ifdef linux
	usleep(ms * 1000);
#endif //linux
#endif // window

}

void MyTimer::ControlFPS(float targetFPS) {
	for (float restTime = lastTime + 1000.0 / targetFPS - GetTime();
		restTime > 0;
		restTime = lastTime + 1000.0 / targetFPS - GetTime()) {
		if (restTime > 2) GoSleep(1);
	}
}

void MyTimer::ControlFPS_Rough(float targetFPS) {
	float nowTime = GetTime();
	if (lastTime + 1000.0 / targetFPS - nowTime > 1) GoSleep(round(lastTime + 1000.0 / targetFPS - nowTime));
}

void MyTimer::MarkTime(std::string state) {
	states.push_back(state);
	times.push_back(GetTime());
}

float MyTimer::operator()(std::string state) {
	for (int i = 1; i < states.size(); i++) {
		if (states[i] == state)
			return times[i] - times[i - 1];
	}
	return -1;
}

std::string MyTimer::TimeToString(std::string name, float time, int precision) {
	if (time < 0) return std::string();
	std::stringstream ss;
	ss << name << ":" << std::setw(precision + 2) << std::fixed << std::setprecision(precision) << time << "ms";
	return ss.str();
}

string MyTimer::ToString(int precision) {
	stringstream ss;
	for (int i = 1; i < states.size(); i++) {
		ss << TimeToString(states[i], times[i], precision);
		if (i != states.size() - 1) ss << ' ';
	}
	return ss.str();
}

string MyTimer::EachTimes(int precision) {
	stringstream ss;
	for (int i = 1; i < states.size(); i++) {
		ss << TimeToString(states[i], times[i] - times[i - 1], precision);
		if (i != states.size() - 1) ss << ' ';
	}
	return ss.str();
}

