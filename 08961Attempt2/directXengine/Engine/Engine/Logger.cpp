#include "Logger.h"


const string Logger::m_filename = "log.txt";
ofstream Logger::static_out;

Logger::Logger(void)
{
}

Logger::Logger(string _fromClass)
{
}

Logger::~Logger(void)
{
}

 void Logger::AppendToLog(const string info) 
{
	static_out.open(m_filename, ios_base::app);
	static_out << '\n' << TimeStamp() << info << endl;
	static_out.close();
}

void Logger::AppendInt(const int _intValue, const string _description)
{
	stringstream str;
	str << _intValue;
	string combine = _description + ": " + str.str();
	AppendToLog(combine);
}

void Logger::AppendErrorToLog(const string info)
{
	string combine = "ERROR " + info;
	AppendToLog(combine);
}

string Logger::TimeStamp()
{
   time_t t = time(0);   // get time now
   string result = ctime (&t);
   return result;

}