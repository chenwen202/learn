#include <thread>
#include <iostream>
#include <io.h>
#include "CmdUtil.h"
#include "CImg.h"
#include "StringUtils.h"

using namespace cimg_library;
using namespace vks_cmd;

CmdUtil::CmdUtil()
{
	width_ = 0;
	height_ =0;
}


CmdUtil::~CmdUtil()
{
}

int	CmdUtil::ProcessArg(int argc, TCHAR *argv[])
{
	int result = 0;
	if (argc < 2)
	{
		std::cout << _T("Your input argument is error! please input json file!") << std::endl;
		return -1;
	}
	std::vector<std::wstring> vec;
	if (argc == 6)
	{
		vec.push_back(std::wstring((wchar_t*) argv[3]));
		vec.push_back(std::wstring((wchar_t*) argv[5]));
	}	
	if (vec.size() != 2) return -2;
	dostitching(vec);	
	return result;
}

void CmdUtil::StopProcess()
{
	_bStopThread = true;
	_condition.notify_all();
	if (_workThread.joinable())
	{
		_workThread.join();
	}
}

std::vector<std::wstring> CmdUtil::parseArgs(std::wstring args)
{
	std::vector<std::wstring> vec;
	std::size_t found = args.find((wchar_t*)L"-batch");
	if (found != std::wstring::npos) {
		found = args.find((wchar_t*)L"-json", found + 1);
		if (found != std::wstring::npos)
		{
			std::size_t j_pos = found + 6;
			std::size_t image = args.find((wchar_t*)L"-image", j_pos);

			std::size_t j_end = image;
			std::wstring json = Trim(args.substr(j_pos, j_end - j_pos));

			vec.push_back(json);

			j_pos = j_end + 7;
			std::wstring path = Trim(args.substr(j_pos, args.length() - j_pos));
			vec.push_back(path);
		}
	}
	return vec;
}
std::wstring ParentPath(const std::wstring& fileFullName)
{
	if (!fileFullName.empty())
	{
		wchar_t delimiter = '\\';
		if (fileFullName.find('/') != std::wstring::npos)
			delimiter = '/';

		size_t pos = fileFullName.rfind(delimiter);
		if (pos != std::wstring::npos)
			return fileFullName.substr(0, pos);
	}

	return L"";
}
bool CreatePath(const std::wstring& path)
{
	if (path.empty()) return false;

	if (_waccess((wchar_t*)path.c_str(), 0) != 0 && _wmkdir((wchar_t*)path.c_str()) != 0)
	{
		std::wstring parentPath = ParentPath(path);
		CreatePath(parentPath);
		return _wmkdir(path.c_str()) == 0 ? true : false;
	}

	return true;
}


std::wstring CmdUtil::getNewFilename(std::wstring old, std::wstring replace, std::wstring val)
{
	std::size_t found = old.find(replace, 0);
	if (found == std::wstring::npos)
	{
		return L"";
	}
	std::wstring name = old.replace(0, replace.length(), val);
	std::wstring parent = ParentPath(name);
	CreatePath(parent);

	return name;
}

int CmdUtil::ReadImage(CString strFile)
{
	// TODO: Add your implementation code here.
	if (strFile.Find(".bmp") != -1)
	{
		CImg<unsigned char> img;
		img.load(strFile);
		width_ = img.width();
		height_=img.height();
		if(src_buf_.size() != width_ * height_)
			src_buf_.resize(width_ * height_);
		memcpy(&src_buf_[0],img.data(), width_ * height_);	
	}

	return 0;
}

bool ReversePath(const std::wstring& path, std::vector<std::wstring>& files, std::vector<std::wstring>& folders, bool bRecursive, std::function<bool(const std::wstring&)> filter)
{
	intptr_t hFile;
	_wfinddata_t fileinfo;
	const std::wstring& pattern = path + L"\\*.*";
	if ((hFile = _wfindfirst(pattern.c_str(), &fileinfo)) != -1)
	{
		do
		{
			std::wstring pathName = path + L"\\" + fileinfo.name;

			if (fileinfo.attrib & _A_SUBDIR)
			{
				if (wcscmp(fileinfo.name, L".") != 0 && wcscmp(fileinfo.name, L"..") != 0)
				{				
					folders.push_back(pathName);

					if (bRecursive)
					{
						ReversePath(pathName, files, folders, bRecursive, filter);
					}
				}
			}
			else
			{
				if (filter)
				{
					if (filter(pathName))
						files.push_back(pathName);
				}
				else {
					files.push_back(pathName);
				}
			}


		} while (_wfindnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
		return true;
	}

	return false;
}

int		CmdUtil::dostitching(std::vector<std::wstring> vec)
{
	int errorCode = 0;	
	std::wstring imgPath = vec.at(0);
	auto filter = [](const std::wstring& file) {
		int pos = file.rfind('.');
		if (pos != -1)
		{
			std::wstring ext = file.substr(pos, file.length() - pos);
			/*if (ext == L".bmp")
				return true;
			else
				return false;*/
			return true;
		}
		return false;
	};

	_workThread = std::thread([this, imgPath, filter]() {
		std::vector<std::wstring> files;
		std::vector<std::wstring> folders;
		ReversePath(imgPath, files, folders, true, filter);
		std::size_t total = files.size();
		std::size_t pos = 0;

		for (auto& item : files) {
			std::unique_lock <std::mutex> lck(_mtx);
			_condition.wait_for(lck, std::chrono::milliseconds(20));
			if (0 == ReadImage(item.c_str()))
			{
				// if (StitchImage() == VKS_RV::VKS_RV_OK)
				// {
				// 	std::wstring data_path = std::wstring(GetFilePath()) + L"data";
				// 	std::wstring new_item = getNewFilename(item, imgPath, data_path);
				// 	SaveAsBitmap(&dst_buf_[0], width_, height_, new_item.c_str());
				// }
			}
			pos++;
			double percent = double(pos)/ total;
			this->printProgress(percent);

			if (this->_bStopThread)
				break;
		}
	});

	this->_workThread.join();	
	
	return 0;
}

std::wstring CmdUtil::Trim(const std::wstring& wstr, const wchar_t* chars)
{
	std::wstring result = wstr;

	if (wstr.empty()) return result;

	if (chars == nullptr)
	{
		chars = L" \n\r\t";
	}

	size_t pos = result.find_first_not_of(chars);
	if (pos != std::wstring::npos)
	{
		result.erase(0, pos);
		pos = result.find_last_not_of(chars);
		if (pos != std::wstring::npos)
		{
			result.erase(pos + 1);
		}
	}
	else
	{
		result.clear();
	}

	return result;
}

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void CmdUtil::printProgress(double percentage)
{
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
}