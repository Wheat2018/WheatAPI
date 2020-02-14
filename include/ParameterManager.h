/*
	By.Wheat
	2019.06.07

	�ļ���ParameterManager.h
	�����������в��������࣬�����ͷ�����
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
		StrVec subStrs = WcharSplit(src, L"()����<>���� ,����/t");

		dst = std::vector<Ty>(subStrs.size());
		for (int i = 0; i < subStrs.size(); i++) Convert(subStrs[i], dst[i]);
	}

	//==============����=============
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

	//==============��ȡ=============
	size_t Size() { return parameters.size(); }

	template<typename Ty>
	Ty operator[](unsigned int idx)
	{
		Ty result;
		if (idx < argc) Convert(Str((char*)argv[idx]), result);
		else COUT << "ParameterManager::operator[] Warning: ��������!" << ENDL;
		return std::move(result);
	}
	//===============================
	//��ӡ�������
	void Print(const Str& retract = "");
	//��ӡ������ʽ
	void Report(const Str& retract = "");
	//��������
	int Catch(bool report = true);
	//������鲶��
	int Run(bool report = true);
};

//���ַ����ָ�������ַ���target�����ַ���ptr�е���һ���ַ��ָ���ص��ַ�������
StrVec WcharSplit(const Str& target, const wchar_t ptr[]);

//�������������д�������������(last-first+1)���̣߳�����ص�����func�ͼ��������ԣ�infoΪ��ӡ��Ϣ�ַ�����maxCountΪ���������ֵ��
void ParallelWithCounter(int first, int last, const std::function<void(volatile long&, int)>& func, const Str& info, long maxCount = -1);
void ParallelWithCounter(int first, int last, const std::function<void(int)>& func, const Str& info);
//��ParallelWithCounter��һ��������func�ص��������ڣ���ʹ�����º궨��ʹ�������������ڶ�����������ò�����
#define IncrementCounter(count) _InterlockedIncrement(&count);

//�������ļ��С��ݹ鴴���ļ��У�report��ʾ�Ƿ��ӡ��������Ϣ
int Create_Directory(const Str& dir_path, bool report = false);

//��ɾ���ļ��С��ݹ�ɾ���ļ��м�Ŀ¼�������ļ�
int Remove_Directory(const Str& szFileDir);

//����ȡĿ¼�ַ������ļ�����
Str GetFileName(const Str& str);

//����ȡĿ¼�������ļ�����
StrVec GetFileList(Str dir);

#ifdef _cplusplus
}
#endif