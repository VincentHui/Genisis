#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

class Logger
{
public:
	Logger(void);
	Logger(string _fromObject);
	~Logger(void);
	static void AppendToLog(const string _info);
	static void AppendInt(const int _value, const string _description);
	static void AppendErrorToLog(const string _info);
	static string TimeStamp();
private:
	const static string m_filename;
	static ofstream static_out;
};

