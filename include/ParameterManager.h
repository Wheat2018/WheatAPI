/*
	By.Wheat
	2019.06.07

	文件：ParameterManager.h
	简述：命令行参数管理类，含泛型方法。
*/
#pragma once
#ifdef _cplusplus
extern "C"
{
#endif

#include <functional>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <ppl.h>
#include <sstream>

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"x64\\ParameterManager_Debug.lib") 
#else
#pragma comment(lib,"x64\\ParameterManager_Release.lib") 
#endif // _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"Win32\\ParameterManager_Debug.lib") 
#else
#pragma comment(lib,"Win32\\ParameterManager_Release.lib") 
#endif // _DEBUG
#endif

typedef std::string Str;
typedef std::vector<Str> StrVec;
typedef std::vector<float> FltVec;
typedef std::vector<int> IntVec;
typedef std::function<void(const Str&)> ActFun;

#define COUT  (*pEs)
#define ENDL std::endl
#define PARAMETER_ELEMENT_DEFAULT "","",[](const Str&){;}
#define FAULT_EXIT	return -1;
#define TRUE_EXIT	return 0;
#define IF_ERROR(x) (x < 0)
#define PAUSE_EXIT			  \
{							  \
	system("pause");		  \
	exit(-1);				  \
}
#define COUT_OUTPUT_IMMEDIATELY COUT.OutputImmediately();
#define COUT_OUTPUT_DELAY COUT.OutputDelay();
extern bool coutImmediate;


class OutputStream;
extern OutputStream* pEs;
class OutputStream
{
	class ContinueStream
	{
	public:
		template <typename Ty>
		ContinueStream& operator<<(Ty elem)
		{
			(*op) << elem;
			return *this;
		}
		ContinueStream& operator<<(std::ostream& (__cdecl* _Pfn)(std::ostream&))
		{
			_Pfn(*op);
			return *this;
		}
	private:
		std::ostream* op;
		std::unique_lock<std::mutex> slck;
		ContinueStream(std::unique_lock<std::mutex>&& slck, std::ostream* op) : slck(std::move(slck)), op(op) { ; }
		ContinueStream(ContinueStream&& cs)noexcept : slck(std::move(cs.slck)), op(cs.op) { ; }
		friend class OutputStream;
	};

public:
	template <typename Ty>
	ContinueStream operator<<(Ty elem)
	{
		std::unique_lock<std::mutex> ulck(mt);
		(*op) << elem;
		return ContinueStream(std::move(ulck), op);
	}
	ContinueStream operator<<(std::ostream& (__cdecl* _Pfn)(std::ostream&))
	{
		std::unique_lock<std::mutex> ulck(mt);
		_Pfn(*op);
		return ContinueStream(std::move(ulck), op);
	}
	static bool Creat();
	void OutputImmediately();
	void OutputDelay();
	size_t BranchNum();
	void RedefineBuf(std::ostream* op);
private:
	std::ostream* op;
	std::stringstream ss;
	std::mutex mt;
	OutputStream() { OutputImmediately(); }
	OutputStream(const OutputStream&) { OutputImmediately(); }
	void operator = (const OutputStream&) { ; }
};


class ParameterManager 
{
	struct ParameterElement
	{
	public:
		Str parameterName;
		Str parameterDefault;
		ActFun action;
	private:
		ParameterElement();
		ParameterElement(const Str& para_name, const Str& para_default, const ActFun& action);
		friend class ParameterManager;
	};

	std::vector<ParameterElement> parameters;
	int argc;
	void** argv;

public:
	ParameterManager(int argc, void* argv[]) :argc(argc), argv(argv) { ; }

	static void Convert(const Str& src, Str& dst) { dst = src; }
	static void Convert(const Str& src, bool& dst) { dst = atoi(src.c_str()); }
	static void Convert(const Str& src, int& dst) { dst = atoi(src.c_str()); }
	static void Convert(const Str& src, float& dst) { dst = atof(src.c_str()); }
	template <typename Ty>
	static void Convert(const Str& src, std::vector<Ty>& dst)
	{
		StrVec WcharSplit(const Str & target, const wchar_t ptr[]);
		StrVec subStrs = WcharSplit(src, L"()（）<>《》 ,，、/t");

		dst = std::vector<Ty>(subStrs.size());
		for (int i = 0; i < subStrs.size(); i++) Convert(subStrs[i], dst[i]);
	}

	//==============输入=============
	void Push(const Str& para_name, const Str& para_default, const ActFun& action)
	{
		parameters.push_back(ParameterElement(para_name, para_default, action));
	}

	template<typename Ty>
	void Push(const Str& para_name, const Str& para_default, Ty& para_ref)
	{
		parameters.push_back(ParameterElement(para_name, para_default, [&](const Str& para) {
			Convert(para, para_ref);
			}));
	}

	template<typename Ty>
	void PushBatch(const Str& para_name, const Str& para_default, std::vector<Ty>& para_vec)
	{
		for (int i = 0; i < para_vec.size(); i++)
		{
			parameters.push_back(ParameterElement(para_name, para_default, [&para_vec, i](const Str& para) {
				Convert(para, para_vec[i]);
				}));
		}
	}
	template<typename Ty>
	void PushBatch(const StrVec& para_name_vec, const Str& para_default, std::vector<Ty>& para_vec)
	{
		for (int i = 0; i < para_vec.size(); i++)
		{
			const Str para_name = (i < para_name_vec.size() ? (para_name_vec[i]) : Str());
			parameters.push_back(ParameterElement(para_name, para_default, [&para_vec, i](const Str& para) {
				Convert(para, para_vec[i]);
				}));
		}
	}
	template<typename Ty>
	void PushBatch(const Str& para_name, const StrVec& para_default_vec, std::vector<Ty>& para_vec)
	{
		for (int i = 0; i < para_vec.size(); i++)
		{
			const Str para_default = (i < para_default_vec.size() ? (para_default_vec[i]) : Str());
			parameters.push_back(ParameterElement(para_name, para_default, [&para_vec, i](const Str& para) {
				Convert(para, para_vec[i]);
				}));
		}
	}
	template<typename Ty>
	void PushBatch(const StrVec& para_name_vec, const StrVec& para_default_vec, std::vector<Ty>& para_vec)
	{
		for (int i = 0; i < para_vec.size(); i++)
		{
			const Str para_default = (i < para_default_vec.size() ? (para_default_vec[i]) : Str());
			const Str para_name = (i < para_name_vec.size() ? (para_name_vec[i]) : Str());
			parameters.push_back(ParameterElement(para_name, para_default, [&para_vec, i](const Str& para) {
				Convert(para, para_vec[i]);
				}));
		}
	}
	//===============================

	//==============获取=============
	size_t Size() { return parameters.size(); }

	template<typename Ty>
	Ty operator[](unsigned int idx)
	{
		Ty result;
		if (idx < argc) Convert(Str((char*)argv[idx]), result);
		else COUT << "ParameterManager::operator[] Warning: 参数不足!" << ENDL;
		return std::move(result);
	}
	//===============================
	//打印捕获参数
	void Print(const Str& retract = "");
	//打印参数格式
	void Report(const Str& retract = "");
	//参数捕获
	int Catch(bool report = true);
	//参数检查捕获
	int Run(bool report = true);
};

//【字符串分割函数】将字符串target按宽字符串ptr中的任一宽字符分割，返回到字符串容器
StrVec WcharSplit(const Str& target, const wchar_t ptr[]);

//【带计数器并行处理函数】并发出(last-first+1)个线程，处理回调函数func和计数器回显，info为打印信息字符串，maxCount为计数器最大值。
void ParallelWithCounter(int first, int last, const std::function<void(volatile long&, int)>& func, const Str& info, long maxCount = -1);
void ParallelWithCounter(int first, int last, const std::function<void(int)>& func, const Str& info);
//在ParallelWithCounter第一个重载中func回调函数体内，可使用以下宏定义使计数器自增。第二个重载无需该操作。
#define IncrementCounter(count) _InterlockedIncrement(&count);

//【创建文件夹】递归创建文件夹，report表示是否打印出创建信息
int Create_Directory(const Str& dir_path, bool report = false);

//【删除文件夹】递归删除文件夹及目录下所有文件
int Remove_Directory(const Str& szFileDir);

//【提取目录字符串中文件名】
Str GetFileName(const Str& str);

//【获取目录下所有文件名】
StrVec GetFileList(Str dir);

#ifdef _cplusplus
}
#endif