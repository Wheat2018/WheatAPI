/*
	作    者: Wheat
	最后修改: 2019.03.02
*/
#pragma once
#ifdef _cplusplus
extern "C"
{
#endif

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#define window

#ifdef window
#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM "x86"
#endif // _WIN64
#ifdef _DEBUG
#define CONFIGURATION "Debug"
#else
#define CONFIGURATION "Release"
#endif // _DEBUG

#pragma comment(lib,PLATFORM "\\" CONFIGURATION "\\MyTimer.lib") 
#endif // window

class MyTimer {
private:
	//计时相关
	time_t clocks_per_second;
	time_t startClock;

	//DoInFPS相关
	float last_call_times;
	float doInFPS_startTime;

	//FPS相关
	float lastTime;
	static const int aveTimes = 5;
	float fpsArr[aveTimes];
	int fpsCounter;

#ifdef window
	time_t WinGetClock();
	void WinInit();
#endif //window

	//获取系统时钟
	time_t GetClock();

	//打印相关
	static std::string TimeToString(std::string name, float time, int precision = 3);

public:
	/*===========================================================================================================*/
	/*===========================================     用户态     ================================================*/
	/*===========================================================================================================*/
	
	/********************************开放属性***********************************/

	/*
		变量名称:	states
		简    述:	记录了每个时间节点字符串类型名称，Start()函数将容器清空，
					并加入一个"Start"节点。
		改动函数:	Start, MarkTime
		依赖函数:	operator(), ToString, EachTimes
		备    注:
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	std::vector<std::string> states;

	/*
		变量名称:	times
		简    述:	记录了每个时间节点毫秒级时间，Start()函数将容器清空，并加
					入一个0节点。
		改动函数:	Start, MarkTime
		依赖函数:	operator(), ToString, EachTimes
		备    注:
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	std::vector<float> times;


	/********************************成员函数***********************************/
	/*
		函数名称:	MyTimer
		简    述:	构造函数，主要内容为获取时钟参数等。通常，MyTimer实例何时
					构造无关紧要。在调用Start()函数前，MyTimer实例的声明位置
					可以是任意的（在确保变量生命周期前提下）。
		传入参数:	无
		传出参数:	无
		修改变量:	clocks_per_second
		依赖变量:	无
		函数调用:	WinInit
		备    注:
		示    例:	MyTimer timer;
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	MyTimer();

	/*
		函数名称:	Start
		简    述:	计时开始。本函数清空节点容器，添加首个计时节点"Start"，计时
					为0。同时，对于底层，记录当前时钟数据。初始化DoInFPS相关变
					量和FPS相关变量。
		传入参数:	无
		传出参数:	无
		修改变量:	states, times, startClock
		依赖变量:	无
		函数调用:	GetClock
		备    注:
		示    例:	
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void Start();

	/*
		函数名称:	GetTime
		简    述:	获取精准时间。函数计算当前系统时钟与记录值之差，获得从Start()
					开始后到调用该函数的时间间隔。
		传入参数:	无
		传出参数:	从Start()开始后到调用该函数的时间间隔。浮点类型，单位为ms。
		修改变量:	无 
		依赖变量:	startClock, clocks_per_second
		函数调用:	GetClock
		备    注:	Windows下，获取时间的精度约为1/2500001秒，由于时间类型time_t
					位宽(unsigned long long 64位)对系统时钟数据的限制，函数获取时
					间值周期约为7×10^12s，约为23万年。
					Start()重置时间，所以会影响GetTime()，使用时应当注意。
		示    例:	MyTimer timer;
					timer.Start();
					...Do something...
					cout << timer.GetTime() << endl;
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	float GetTime();



	/*==================================================||
	||	功	能：计时									||
	||	示	例：比较循环赋值和memset速度差异			||
	||	代	码：										||
	||	#include <iostream>								||
	||	#include "MyTimer.h"							||
	||	using namespace std;							||
	||													||
	||	int main(){										||
	||		MyTimer timer;								||
	||		timer.Start();								||
	||													||
	||		int arr[10000];								||
	||													||
	||		//循环赋值									||
	||		for(int i = 0; i < 1e+5; i++) arr[i] = 0;	||
	||		timer.MarkTime("循环赋值");					||
	||													||
	||		//memset									||
	||		memset(arr, 0, (int)1e+5);					||
	||		timer.MarkTime("memset");					||
	||													||
	||		cout << timer.EachTimes() << endl;			||
	||	}												||
	||==================================================*/

	/*
		函数名称:	MarkTime
		简    述:	标记时间节点。函数向节点容器中添加节点名称及当前GetTime()返回
					值。
		传入参数:	state 节点名称
		传出参数:	无
		修改变量:	states, times
		依赖变量:	无
		函数调用:	GetTime
		备    注:	
		示    例:	MyTimer timer;
					timer.Start();
					...Do something...
					timer.MarkTime("节点1");
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void MarkTime(std::string state);

	/*
		函数名称:	operator()
		简    述:	括号运算符重载，作用为获取当前节点容器中某个名称的节点和上一个
					节点的时间差值，若节点不存在，返回-1
		传入参数:	state 节点名称
		传出参数:	节点时间值
		修改变量:	无
		依赖变量:	states, times
		函数调用:	无
		备    注:
		示    例:	float t = timer("节点1");
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	float operator()(std::string state);

	/*
		函数名称:	ToString
		简    述:	将全部节点以特定格式转换成字符串。
		传入参数:	precision 输出时间精度（有效数字）
		传出参数:	转换的字符串结果
		修改变量:	无
		依赖变量:	states, times
		函数调用:	TimeToString
		备    注:	每个节点都是从开始到节点标记的累积时间。
		示    例:	cout << timer.ToString();
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	std::string ToString(int precision = 3);

	/*
		函数名称:	ToString
		简    述:	将全部节点以特定格式转换成字符串。
		传入参数:	precision 输出时间精度（有效数字）
		传出参数:	转换的字符串结果
		修改变量:	无
		依赖变量:	states, times
		函数调用:	TimeToString
		备    注:	每个节点都是从上一个节点到该节点的间隔时间，Start节点不输出。
		示    例:	cout << timer.EachTimes();
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	std::string EachTimes(int precision = 3);



	/*======================================================================================||
	||	功	能：定时调用																	||
	||	示	例：每隔0.5秒(FPS=2)将布尔变量sw取非											||
	||	代	码：																			||
	||	#include <iostream>																	||
	||	#include "MyTimer.h"																||
	||	using namespace std;																||
	||																						||
	||	int main(){																			||
	||		MyTimer timer;																	||
	||		timer.Start();																	||
	||																						||
	||		bool sw = true;																	||
	||																						||
	||		while(1) {																		||
	||			timer.DoInFPS(2, [&](float){												||
	||				sw ^= 1;																||
	||			});																			||
	||			Sleep(1);																	||
	||		}																				||
	||																						||
	||	}																					||
	||==================================================*===================================*/

	/*
		函数名称:	DoInFPS
		简    述:	以给定的帧率(FPS)调用函数指针func，并传入参数argc, argv。
		传入参数:	fps   工作帧率（给定FPS值）
					argc  func入参个数
					argv  func参数指针数组
					func  任务函数
						func参数列表
						int		argc 由DoPerSpell形参argc传入
						void**	argv 由DoPerSpell形参argv传入
						float	call_times 自第一次调用DoPerSpell开始，当前理论应为
											第几次调用(可能非整数)
					fps_limit
						  fps限制  true  —— 正常模式
								   false —— 以本函数调用频率(可能超过给定FPS的频
											  率)调用任务函数。通常配合call_times
											  变量使用。该模式下focus_on_interval失
											  效。
					focus_on_interval
						  模式开关 true  —— 更关注等间隔调用
								   false —— 更关注恰时调用

								   假设第一次调用为0ms时，期望调用间隔为10ms；而实
								   际第二次调用为1.12ms。
									·true  第三次期望调用时间为2.12ms
									·false 第三次期望调用时间为2.00ms

								   相对而言，true模式帧率更稳定，在帧率波动中能更快
								   恢复，并且两次任务间隔更符合预期值，绝不过早调用
								   （对于某些图像捕获处理，两次处理间隔过早，小于捕
								   获频率，处理是无意义的），但若任务函数对调用次数
								   进行取整，在累积误差作用下，可能某些特定帧将被跳
								   过。false模式不关注帧率，该模式下无论调用多少
								   次都不会发生人为时钟偏移。更推荐false模式。

								   在函数调用频率接近给定FPS时，建议使用false模式，
								   此时true模式极不稳定。
		传出参数:	无
		修改变量:	last_call_times, doInFPS_startTime(仅第一次调用时修改)
		依赖变量:	last_call_times, doInFPS_startTime
		函数调用:	GetTime
		备    注:	本函数是非阻塞的，所以要实现定时调用，需要放在循环中。另外使
					用匿名函数特性，可以使调用形式非常简洁。
					使用建议：
					理论上本函数调用频率越高，func的调用频率越接近给定FPS值
					当本函数调用频率大于给定帧率时，尝试以给定帧率调用func函数。
					当本函数调用频率小于给定帧率时，func函数调用频率等于本函数调
					用频率，func函数内部可以根据传入的doInFPS_startTime值判断当前
					理应为第几次调用，以此跳过缺失的帧。
					>>第一行的重载函数是具有C++ Lambda特性支持的函数变体。在旧标准
					下编译，应该删除该函数。
		示    例:	MyTimer timer;
					timer.Start();
					while(1) {
						timer.DoInFPS(120, [](float){
							cout << "Hello World!" << endl;
						});
						Sleep(1);
					}
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void DoInFPS(float fps, std::function<void(float)> func,bool fps_limit = true , bool focus_on_interval = false);
	void DoInFPS(float fps, int argc, void* argv[], void(*func)(int argc, void** argv, float call_times),bool fps_limit = true , bool focus_on_interval = false);




	/*======================================================================================||
	||	功	能：FPS测量及调控																||
	||	示	例：将循环控制在每秒120次，每隔0.5秒(FPS=2)打印一次FPS值						||
	||	代	码：																			||
	||	#include <iostream>																	||
	||	#include "MyTimer.h"																||
	||	using namespace std;																||
	||																						||
	||	int main(){																			||
	||		MyTimer timer;																	||
	||		timer.Start();																	||
	||																						||
	||		while(1) {																		||
	||			timer.DoInFPS(2,[&](float t){												||
	||				cout << "\rFPS:" << timer.GetFPS();										||
	||				});																		||
	||			timer.ControlFPS(120);														||
	||			timer.MarkFPS();															||
	||		}																				||
	||																						||
	||	}																					||
	||======================================================================================*/

	/*
		函数名称:	MarkFPS
		简    述:	测量某处FPS值
		传入参数:	无
		传出参数:	无
		修改变量:	fpsArr, counter, lastTime
		依赖变量:	counter, lastTime， aveTimes
		函数调用:	GetTime
		备    注:	要测量FPS，Start()函数必须事先调用过，且不能出现在测量处，因
					为Start()会影响GetTime()。
		示    例:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.MarkFPS();
					}
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void MarkFPS();

	/*
		函数名称:	GetFPS
		简    述:	获取测量的FPS值，多次取平均，平均次数由静态常值变量aveTimes决
					定。
		传入参数:	无
		传出参数:	FPS值
		修改变量:	无
		依赖变量:	fpsArr, aveTimes
		函数调用:	无
		备    注:	
		示    例:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.MarkFPS();
						cout << timer.GetFPS() << endl;
					}
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	float GetFPS();

	/*
		函数名称:	ControlFPS
		简    述:	阻塞延时函数。能够精准调控函数入口处的FPS值，需配合MarkFPS()使
					用。阻塞期间有选择地释放线程占用。
		传入参数:	targetFPS 目标
		传出参数:	无
		修改变量:	lastTime
		依赖变量:	lastTime
		函数调用:	GetTime
		备    注:	要控制FPS，该函数必须在MarkFPS()前调用
		示    例:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.ControlFPS(120);
						timer.MarkFPS();
					}
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void ControlFPS(float targetFPS);

	/*
		函数名称:	ControlFPS_Rough
		简    述:	阻塞延时函数。能够粗略调控函数入口处的FPS值，需配合MarkFPS()使
					用。阻塞期间释放线程占用。该方法精度及CPU占用率均低于ControlFPS。
		传入参数:	targetFPS 目标
		传出参数:	无
		修改变量:	lastTime
		依赖变量:	lastTime
		函数调用:	GetTime
		备    注:	要控制FPS，该函数必须在MarkFPS()前调用
		示    例:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.ControlFPS_Rough(120);
						timer.MarkFPS();
					}
		作    者:	Wheat
		最后修改:	2019.03.02
	*/
	void ControlFPS_Rough(float targetFPS);


};

#ifdef _cplusplus
}
#endif