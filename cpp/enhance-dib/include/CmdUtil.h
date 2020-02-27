#ifndef _CMD_UTIL_H_
#define _CMD_UTIL_H_

#include <tchar.h>
#include <vector>
#include<thread>
#include<mutex>
#include <condition_variable>
#include <afxstr.h>
namespace vks_cmd
{

	class CmdUtil
	{
	public:
		CmdUtil();
		~CmdUtil();
	
		//! @brief pare the command line in InitInstant function	
		//! @detail 
		//!			-batch -json SensorData\00AC3818140A9200.json  -path d:\image
		//!	description:
		//!			if '-batch' do swtitching in command line mode
		//!			-json	SensorData\00AC3818140A9200.json,  the json file to load
		//!			-path	d:\image						the path to do stitching
		//! @return 0 success
		int ProcessArg(int argc, TCHAR *argv[]);

		//! @brief stop working thread
		void StopProcess();

	private:
		std::thread _workThread;
		std::condition_variable _condition;
		std::mutex	_mtx;
		bool	_bStopThread;

		std::vector<std::wstring> parseArgs(std::wstring args);
		//! @brief create new filename by using val to replace the 'replace' in old	
		//! @param[in] old, source filename
		//!	@param[in] replace, string to be replaced
		//! @param[in] val, the final string in new filename
		//! @return 0 success, -1 no replace string in old, -2 others
		std::wstring getNewFilename(std::wstring old, std::wstring replace, std::wstring val);
		int		dostitching(std::vector<std::wstring> vec);

		int nShowStichWidth;
		std::vector<unsigned char> dst_buf_;
		std::vector<unsigned char> src_buf_;
		int				width_;
		int				height_;
		int ReadImage(CString strFile);
		std::wstring Trim(const std::wstring& wstr, const wchar_t* chars = nullptr);
		void printProgress(double percentage);
	};
}

#endif //_CMD_UTIL_H_


