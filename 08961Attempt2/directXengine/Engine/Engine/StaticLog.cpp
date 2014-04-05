#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>

namespace static_log
{
	using namespace std;
	void AppendLog(const string _info){
	ofstream outfile;
	outfile.open("log.txt", ios_base::app);
	time_t t = time(0);   // get time now
	string result = ctime (&t);
	outfile << '\n' << result << _info << endl;
	}
}