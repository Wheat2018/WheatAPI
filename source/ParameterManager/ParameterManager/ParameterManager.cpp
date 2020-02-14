// ParameterManager.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include <mutex>
#include <iomanip>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")

OutputStream* pEs;
bool coutImmediate = OutputStream::Creat();

bool OutputStream::Creat()
{
	static std::once_flag flag;
	std::call_once(flag, []() {
		if (pEs == nullptr) pEs = new OutputStream();
		});
	return true;
}

void OutputStream::OutputImmediately() { RedefineBuf(std::_Ptr_cout); }

void OutputStream::OutputDelay() { RedefineBuf(&ss); }

size_t OutputStream::BranchNum() {
	size_t count = 0;
	Str buffer = std::move(ss.str());
	for (auto i : buffer)
		if (i == '\n') count++;
	return count;
}

void OutputStream::RedefineBuf(std::ostream* op)
{
	if (op == this->op) return;
	std::lock_guard<std::mutex> lk(mt);
	if (this->op == &ss)
	{
		(*op) << ss.str();
		ss.str("");
	}
	this->op = op;
}

ParameterManager::ParameterElement::ParameterElement(const Str& para_name, const Str& para_default, const ActFun& action)
{
	parameterName = para_name;
	parameterDefault = para_default;
	this->action = action;
}

//===============================

void ParameterManager::Print(const Str& retract)
{	//打印捕获参数
	int paraNameLen = 0;
	int tmp;
	for (ParameterElement& para : parameters)
	{
		tmp = para.parameterName.size();
		if (tmp > paraNameLen) paraNameLen = tmp;
	}
	paraNameLen += 3;


	for (int i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].parameterName.size())
			COUT << retract << std::left << std::setw(paraNameLen)
			<< Str("[") + parameters[i].parameterName + "]"
			<< ":"
			<< (i < argc ? operator[]<Str>(i) : parameters[i].parameterDefault)
			<< ENDL;
	}
}

void ParameterManager::Report(const Str& retract)
{	//打印参数格式
	int paraNameLen = 0;
	int tmp;
	for (ParameterElement& para : parameters)
	{
		tmp = para.parameterName.size();
		if (tmp > paraNameLen) paraNameLen = tmp;
	}
	paraNameLen += 3;

	if (parameters.size() == 0) COUT << retract << "[无]" << ENDL;

	for (ParameterElement& para : parameters)
	{
		if (para.parameterName.size())
			COUT << retract << std::left << std::setw(paraNameLen)
			<< Str("[") + para.parameterName + "]"
			<< "输入*为" << para.parameterDefault
			<< ENDL;
	}
}

int ParameterManager::Catch(bool report)
{
	for (int i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].parameterName.empty()) continue;

		if (i < argc && argv[i] && Str((char*)argv[i]) != "*")
			parameters[i].action(Str((char*)argv[i]));
		else
		{	//给定参数少于需求参数，或给定了"*"
			if (parameters[i].parameterDefault.size())
			{	//该参数支持默认参数
				parameters[i].action(parameters[i].parameterDefault);
			}
			else
			{	//该参数不支持默认参数
				if (report)
				{
					COUT << "ParameterManager::Catch Error: 参数" << i << "捕获失败! "
						<< "[" << parameters[i].parameterName << "]"
						<< "不支持默认参数"
						<< ENDL;
					COUT << "参数格式应为:" << ENDL;
					Report();
					FAULT_EXIT;
				}

			}
		}
	}
	TRUE_EXIT;
}

//参数检查捕获

int ParameterManager::Run(bool report)
{
	if (report && argc > parameters.size())
	{
		COUT << "ParameterManager::Run Error: 参数错误! 参数格式应为:" << ENDL;
		Report();
		FAULT_EXIT;
	}

	Catch();

	TRUE_EXIT;
}

StrVec WcharSplit(const Str& target, const wchar_t ptr[])
{
	StrVec separators;
	for (int i = 0; ptr[i] != L'\0'; i++)
	{
		wchar_t wch[2] = { ptr[i], 0 };
		_bstr_t t = wch;
		separators.push_back(Str((char*)t));
	}

	StrVec result;
	size_t last = 0, now = -1, tmp;
	bool isWchar;

	do
	{
		now = -1;
		for (const Str& sep : separators)
		{
			tmp = target.find(sep, last);
			if (tmp != -1 && tmp < now)
			{
				now = tmp;
				isWchar = (sep.size() > 1);
			}
		}
		if (now != -1)
		{
			if (now != last) result.push_back(target.substr(last, now - last));
			last = now + (isWchar ? 2 : 1);
		}
	} while (now != -1);
	if (last < target.size()) result.push_back(target.substr(last));

	return std::move(result);
}

void ParallelWithCounter(int first, int last, const std::function<void(volatile long&, int)>& func, const Str& info, long maxCount)
{
	if (maxCount < 0) maxCount = last - first;
	volatile bool finished = false;
	volatile long count = 0;
	COUT_OUTPUT_DELAY;
	concurrency::parallel_invoke([&]()
		{
			concurrency::parallel_for(first, last, [&](int idx)
				{
					func(count, idx);
				});
			finished = true;
		}, [&]()
		{
			while (!finished)
			{
				std::cout << '\r' << "正在处理[" << info << "]" << ":" << count << "/" << maxCount << "        ";
				if (count >= maxCount) break;
				Sleep(1);
			}
			std::cout << '\r' << "正在处理[" << info << "]" << ":" << count << "/" << maxCount << "        ";
		}
		);
	std::cout << "已完成" << ENDL;
	if (COUT.BranchNum() != 0)
	{
		std::cout << "但产生了" << COUT.BranchNum() << "条错误:" << ENDL;
		COUT << "共" << COUT.BranchNum() << "条错误" << ENDL;
	}
	COUT_OUTPUT_IMMEDIATELY;
}

void ParallelWithCounter(int first, int last, const std::function<void(int)>& func, const Str& info)
{
	ParallelWithCounter(first, last, [&](volatile long& count, int idx)
		{
			func(idx);
			IncrementCounter(count);
		}, info, last - first);
}

int Create_Directory(const Str& dir_path, bool report)
{
	if (CreateDirectory(dir_path.c_str(), NULL) <= 0)
	{
		if (GetLastError() == ERROR_PATH_NOT_FOUND) {
			if (dir_path.empty() || IF_ERROR(Create_Directory(dir_path.substr(0, dir_path.find_last_of("/\\")), report)))
			{
				COUT << "创建目录\"" << dir_path << "\"失败，任意键退出" << ENDL;
				FAULT_EXIT;
			}
			else Create_Directory(dir_path, report);
		}
	}
	else if (report) COUT << "创建了目录\"" << dir_path << "\"" << ENDL;
	TRUE_EXIT;
}

int Remove_Directory(const Str& szFileDir) {
	std::string strDir = szFileDir;
	if (strDir.at(strDir.length() - 1) != '\\')
		strDir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + "*.*").c_str(), &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		FAULT_EXIT;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_stricmp(wfd.cFileName, ".") != 0 &&
				_stricmp(wfd.cFileName, "..") != 0)
				Remove_Directory((strDir + wfd.cFileName).c_str());
		}
		else
		{
			DeleteFile((strDir + wfd.cFileName).c_str());
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	RemoveDirectory(szFileDir.c_str());
	TRUE_EXIT;
}

Str GetFileName(const Str& str)
{
	int idx = str.find_last_of("/\\") + 1;
	return str.substr(idx, str.size() - idx);
}

StrVec GetFileList(Str dir) {
	if (dir.at(dir.length() - 1) != '\\') dir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((dir + "*.*").c_str(), &wfd);

	StrVec result;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				result.push_back(GetFileName(wfd.cFileName));
		} while (FindNextFile(hFind, &wfd));
		FindClose(hFind);
	}

	return std::move(result);
}
