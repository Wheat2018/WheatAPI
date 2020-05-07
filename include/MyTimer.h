/*
	��    ��: Wheat
	����޸�: 2019.03.02
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
	//��ʱ���
	time_t clocks_per_second;
	time_t startClock;

	//DoInFPS���
	float last_call_times;
	float doInFPS_startTime;

	//FPS���
	float lastTime;
	static const int aveTimes = 5;
	float fpsArr[aveTimes];
	int fpsCounter;

#ifdef window
	time_t WinGetClock();
	void WinInit();
#endif //window

	//��ȡϵͳʱ��
	time_t GetClock();

	//��ӡ���
	static std::string TimeToString(std::string name, float time, int precision = 3);

public:
	/*===========================================================================================================*/
	/*===========================================     �û�̬     ================================================*/
	/*===========================================================================================================*/
	
	/********************************��������***********************************/

	/*
		��������:	states
		��    ��:	��¼��ÿ��ʱ��ڵ��ַ����������ƣ�Start()������������գ�
					������һ��"Start"�ڵ㡣
		�Ķ�����:	Start, MarkTime
		��������:	operator(), ToString, EachTimes
		��    ע:
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	std::vector<std::string> states;

	/*
		��������:	times
		��    ��:	��¼��ÿ��ʱ��ڵ���뼶ʱ�䣬Start()������������գ�����
					��һ��0�ڵ㡣
		�Ķ�����:	Start, MarkTime
		��������:	operator(), ToString, EachTimes
		��    ע:
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	std::vector<float> times;


	/********************************��Ա����***********************************/
	/*
		��������:	MyTimer
		��    ��:	���캯������Ҫ����Ϊ��ȡʱ�Ӳ����ȡ�ͨ����MyTimerʵ����ʱ
					�����޹ؽ�Ҫ���ڵ���Start()����ǰ��MyTimerʵ��������λ��
					����������ģ���ȷ��������������ǰ���£���
		�������:	��
		��������:	��
		�޸ı���:	clocks_per_second
		��������:	��
		��������:	WinInit
		��    ע:
		ʾ    ��:	MyTimer timer;
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	MyTimer();

	/*
		��������:	Start
		��    ��:	��ʱ��ʼ����������սڵ�����������׸���ʱ�ڵ�"Start"����ʱ
					Ϊ0��ͬʱ�����ڵײ㣬��¼��ǰʱ�����ݡ���ʼ��DoInFPS��ر�
					����FPS��ر�����
		�������:	��
		��������:	��
		�޸ı���:	states, times, startClock
		��������:	��
		��������:	GetClock
		��    ע:
		ʾ    ��:	
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void Start();

	/*
		��������:	GetTime
		��    ��:	��ȡ��׼ʱ�䡣�������㵱ǰϵͳʱ�����¼ֵ֮���ô�Start()
					��ʼ�󵽵��øú�����ʱ������
		�������:	��
		��������:	��Start()��ʼ�󵽵��øú�����ʱ�������������ͣ���λΪms��
		�޸ı���:	�� 
		��������:	startClock, clocks_per_second
		��������:	GetClock
		��    ע:	Windows�£���ȡʱ��ľ���ԼΪ1/2500001�룬����ʱ������time_t
					λ��(unsigned long long 64λ)��ϵͳʱ�����ݵ����ƣ�������ȡʱ
					��ֵ����ԼΪ7��10^12s��ԼΪ23���ꡣ
					Start()����ʱ�䣬���Ի�Ӱ��GetTime()��ʹ��ʱӦ��ע�⡣
		ʾ    ��:	MyTimer timer;
					timer.Start();
					...Do something...
					cout << timer.GetTime() << endl;
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	float GetTime();



	/*==================================================||
	||	��	�ܣ���ʱ									||
	||	ʾ	�����Ƚ�ѭ����ֵ��memset�ٶȲ���			||
	||	��	�룺										||
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
	||		//ѭ����ֵ									||
	||		for(int i = 0; i < 1e+5; i++) arr[i] = 0;	||
	||		timer.MarkTime("ѭ����ֵ");					||
	||													||
	||		//memset									||
	||		memset(arr, 0, (int)1e+5);					||
	||		timer.MarkTime("memset");					||
	||													||
	||		cout << timer.EachTimes() << endl;			||
	||	}												||
	||==================================================*/

	/*
		��������:	MarkTime
		��    ��:	���ʱ��ڵ㡣������ڵ���������ӽڵ����Ƽ���ǰGetTime()����
					ֵ��
		�������:	state �ڵ�����
		��������:	��
		�޸ı���:	states, times
		��������:	��
		��������:	GetTime
		��    ע:	
		ʾ    ��:	MyTimer timer;
					timer.Start();
					...Do something...
					timer.MarkTime("�ڵ�1");
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void MarkTime(std::string state);

	/*
		��������:	operator()
		��    ��:	������������أ�����Ϊ��ȡ��ǰ�ڵ�������ĳ�����ƵĽڵ����һ��
					�ڵ��ʱ���ֵ�����ڵ㲻���ڣ�����-1
		�������:	state �ڵ�����
		��������:	�ڵ�ʱ��ֵ
		�޸ı���:	��
		��������:	states, times
		��������:	��
		��    ע:
		ʾ    ��:	float t = timer("�ڵ�1");
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	float operator()(std::string state);

	/*
		��������:	ToString
		��    ��:	��ȫ���ڵ����ض���ʽת�����ַ�����
		�������:	precision ���ʱ�侫�ȣ���Ч���֣�
		��������:	ת�����ַ������
		�޸ı���:	��
		��������:	states, times
		��������:	TimeToString
		��    ע:	ÿ���ڵ㶼�Ǵӿ�ʼ���ڵ��ǵ��ۻ�ʱ�䡣
		ʾ    ��:	cout << timer.ToString();
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	std::string ToString(int precision = 3);

	/*
		��������:	ToString
		��    ��:	��ȫ���ڵ����ض���ʽת�����ַ�����
		�������:	precision ���ʱ�侫�ȣ���Ч���֣�
		��������:	ת�����ַ������
		�޸ı���:	��
		��������:	states, times
		��������:	TimeToString
		��    ע:	ÿ���ڵ㶼�Ǵ���һ���ڵ㵽�ýڵ�ļ��ʱ�䣬Start�ڵ㲻�����
		ʾ    ��:	cout << timer.EachTimes();
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	std::string EachTimes(int precision = 3);



	/*======================================================================================||
	||	��	�ܣ���ʱ����																	||
	||	ʾ	����ÿ��0.5��(FPS=2)����������swȡ��											||
	||	��	�룺																			||
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
		��������:	DoInFPS
		��    ��:	�Ը�����֡��(FPS)���ú���ָ��func�����������argc, argv��
		�������:	fps   ����֡�ʣ�����FPSֵ��
					argc  func��θ���
					argv  func����ָ������
					func  ������
						func�����б�
						int		argc ��DoPerSpell�β�argc����
						void**	argv ��DoPerSpell�β�argv����
						float	call_times �Ե�һ�ε���DoPerSpell��ʼ����ǰ����ӦΪ
											�ڼ��ε���(���ܷ�����)
					fps_limit
						  fps����  true  ���� ����ģʽ
								   false ���� �Ա���������Ƶ��(���ܳ�������FPS��Ƶ
											  ��)������������ͨ�����call_times
											  ����ʹ�á���ģʽ��focus_on_intervalʧ
											  Ч��
					focus_on_interval
						  ģʽ���� true  ���� ����ע�ȼ������
								   false ���� ����עǡʱ����

								   �����һ�ε���Ϊ0msʱ���������ü��Ϊ10ms����ʵ
								   �ʵڶ��ε���Ϊ1.12ms��
									��true  ��������������ʱ��Ϊ2.12ms
									��false ��������������ʱ��Ϊ2.00ms

								   ��Զ��ԣ�trueģʽ֡�ʸ��ȶ�����֡�ʲ������ܸ���
								   �ָ�����������������������Ԥ��ֵ�������������
								   ������ĳЩͼ�񲶻������δ��������磬С�ڲ�
								   ��Ƶ�ʣ�������������ģ��������������Ե��ô���
								   ����ȡ�������ۻ���������£�����ĳЩ�ض�֡������
								   ����falseģʽ����ע֡�ʣ���ģʽ�����۵��ö���
								   �ζ����ᷢ����Ϊʱ��ƫ�ơ����Ƽ�falseģʽ��

								   �ں�������Ƶ�ʽӽ�����FPSʱ������ʹ��falseģʽ��
								   ��ʱtrueģʽ�����ȶ���
		��������:	��
		�޸ı���:	last_call_times, doInFPS_startTime(����һ�ε���ʱ�޸�)
		��������:	last_call_times, doInFPS_startTime
		��������:	GetTime
		��    ע:	�������Ƿ������ģ�����Ҫʵ�ֶ�ʱ���ã���Ҫ����ѭ���С�����ʹ
					�������������ԣ�����ʹ������ʽ�ǳ���ࡣ
					ʹ�ý��飺
					�����ϱ���������Ƶ��Խ�ߣ�func�ĵ���Ƶ��Խ�ӽ�����FPSֵ
					������������Ƶ�ʴ��ڸ���֡��ʱ�������Ը���֡�ʵ���func������
					������������Ƶ��С�ڸ���֡��ʱ��func��������Ƶ�ʵ��ڱ�������
					��Ƶ�ʣ�func�����ڲ����Ը��ݴ����doInFPS_startTimeֵ�жϵ�ǰ
					��ӦΪ�ڼ��ε��ã��Դ�����ȱʧ��֡��
					>>��һ�е����غ����Ǿ���C++ Lambda����֧�ֵĺ������塣�ھɱ�׼
					�±��룬Ӧ��ɾ���ú�����
		ʾ    ��:	MyTimer timer;
					timer.Start();
					while(1) {
						timer.DoInFPS(120, [](float){
							cout << "Hello World!" << endl;
						});
						Sleep(1);
					}
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void DoInFPS(float fps, std::function<void(float)> func,bool fps_limit = true , bool focus_on_interval = false);
	void DoInFPS(float fps, int argc, void* argv[], void(*func)(int argc, void** argv, float call_times),bool fps_limit = true , bool focus_on_interval = false);




	/*======================================================================================||
	||	��	�ܣ�FPS����������																||
	||	ʾ	������ѭ��������ÿ��120�Σ�ÿ��0.5��(FPS=2)��ӡһ��FPSֵ						||
	||	��	�룺																			||
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
		��������:	MarkFPS
		��    ��:	����ĳ��FPSֵ
		�������:	��
		��������:	��
		�޸ı���:	fpsArr, counter, lastTime
		��������:	counter, lastTime�� aveTimes
		��������:	GetTime
		��    ע:	Ҫ����FPS��Start()�����������ȵ��ù����Ҳ��ܳ����ڲ���������
					ΪStart()��Ӱ��GetTime()��
		ʾ    ��:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.MarkFPS();
					}
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void MarkFPS();

	/*
		��������:	GetFPS
		��    ��:	��ȡ������FPSֵ�����ȡƽ����ƽ�������ɾ�̬��ֵ����aveTimes��
					����
		�������:	��
		��������:	FPSֵ
		�޸ı���:	��
		��������:	fpsArr, aveTimes
		��������:	��
		��    ע:	
		ʾ    ��:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.MarkFPS();
						cout << timer.GetFPS() << endl;
					}
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	float GetFPS();

	/*
		��������:	ControlFPS
		��    ��:	������ʱ�������ܹ���׼���غ�����ڴ���FPSֵ�������MarkFPS()ʹ
					�á������ڼ���ѡ����ͷ��߳�ռ�á�
		�������:	targetFPS Ŀ��
		��������:	��
		�޸ı���:	lastTime
		��������:	lastTime
		��������:	GetTime
		��    ע:	Ҫ����FPS���ú���������MarkFPS()ǰ����
		ʾ    ��:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.ControlFPS(120);
						timer.MarkFPS();
					}
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void ControlFPS(float targetFPS);

	/*
		��������:	ControlFPS_Rough
		��    ��:	������ʱ�������ܹ����Ե��غ�����ڴ���FPSֵ�������MarkFPS()ʹ
					�á������ڼ��ͷ��߳�ռ�á��÷������ȼ�CPUռ���ʾ�����ControlFPS��
		�������:	targetFPS Ŀ��
		��������:	��
		�޸ı���:	lastTime
		��������:	lastTime
		��������:	GetTime
		��    ע:	Ҫ����FPS���ú���������MarkFPS()ǰ����
		ʾ    ��:	MyTimer timer;
					timer.Start();
					while(1) {
						...Do something...
						timer.ControlFPS_Rough(120);
						timer.MarkFPS();
					}
		��    ��:	Wheat
		����޸�:	2019.03.02
	*/
	void ControlFPS_Rough(float targetFPS);


};

#ifdef _cplusplus
}
#endif