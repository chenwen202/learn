#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <algorithm>
#include <vector>

namespace Utils {
	std::string UnicodeToAnsi(const std::wstring& wstr);
	std::wstring AnsiToUnicode(const std::string& str);
	std::string UnicodeToUTF8(const std::wstring& wstr);
	std::wstring UTF8ToUnicode(const std::string& u8str);
	std::string AnsiToUTF8(const std::string& str);
	std::string UTF8ToAnsi(const std::string& u8str);

	template<typename T, typename N>
	class StringUtils {
	public:
		static int toInt(const T& str, size_t const mode = 10);
		static double toDouble(const T& wstr);

		static T fromInt(int n);
		static T fromDouble(double d, size_t const s = 2);

		static T toUpper(T str);
		static T toLower(T str);

		static std::vector<T> Split(const T& str, const T& sep);
		static T Replace(const T& str, const T& repalceStr, const T& destStr);

		static bool StartsWith(const T& str, const T& startStr, bool ignoreCase = false);
		static bool EndsWith(const T& str, const T& endStr, bool ignoreCase = false);

		static T Trim(const T& str, const N* chars = nullptr);

		static bool isNumber(const T& str);

		size_t hash(T& str);
	};

	template<typename T, typename N>
	int StringUtils<T, N>::toInt(const T& str, size_t const mode)
	{
		wchar_t* end;
		int result = ::_tcstol(str.c_str(), &end, mode);
		return result;
	}

	template<typename T, typename N>
	double StringUtils<T, N>::toDouble(const T& str)
	{
		wchar_t* end;
		double result = ::tcstod(str.c_str(), &end);
		return result;
	}

	template<typename T, typename N>
	T StringUtils<T, N>::fromInt(int n)
	{

		N buffer[32] = { 0 };
		_stprintf_s(buffer, 32, L"%d", n);
		return std::wstring(buffer);
	}

	template<typename T, typename N>
	T StringUtils<T, N>::fromDouble(double d, size_t const s)
	{
		N format[16] = { 0 };
		_stprintf_s(format, 16, L"%%.%df", s);
		N buffer[64] = { 0 };
		_stprintf_s(buffer, 64, format, d);
		return std::wstring(buffer);
	}

	template<typename T, typename N>
	T StringUtils<T, N>::toUpper(T str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str.c_str();
	}
	template<typename T, typename N>
	T StringUtils<T, N>::toLower(T str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str.c_str();
	}

	template<typename T, typename N>
	std::vector<T> StringUtils<T, N>::Split(const T& str, const T& sep)
	{
		std::vector<T> vec;
		wchar_t *next_token = NULL;
		wchar_t* p = _tcstok_s((N*)str.c_str(), sep.c_str(), &next_token);
		while (p)
		{
			vec.push_back(T(p));
			p = _tcstok_s(NULL, sep.c_str(), &next_token);
		}

		return vec;
	}

	template<typename T, typename N>
	T StringUtils<T, N>::Replace(const T& wstr, const T& repalceStr, const T& destStr)
	{
		size_t pos = wstr.find(repalceStr);
		if (pos == T::npos) return wstr;

		int len1 = repalceStr.length();
		int len2 = destStr.length();

		T temp = wstr;
		while (pos != T::npos)
		{
			temp.replace(pos, len1, destStr.c_str());
			pos = temp.find(repalceStr, pos + len2);
		}

		return temp;
	}

	template<typename T, typename N>
	bool StringUtils<T, N>::StartsWith(const T& str, const T& startStr, bool ignoreCase)
	{
		if (startStr.length() > 0 && startStr.length() <= str.length())
		{
			if (ignoreCase)
			{
				return _tcsnicmp(str.c_str(), startStr.c_str(), startStr.length()) == 0;
			}
			else
			{
				return _tcsnicmp(str.c_str(), startStr.c_str(), startStr.length()) == 0;
			}
		}

		return false;
	}

	template<typename T, typename N>
	bool StringUtils<T, N>::EndsWith(const T& str, const T& endStr, bool ignoreCase)
	{
		int len1 = str.length();
		int len2 = endStr.length();

		if (len2 > 0 && len2 <= len1)
		{
			if (ignoreCase)
			{
				return _tcscmp(str.c_str() + (len1 - len2), endStr.c_str()) == 0;
			}
			else
			{
				return _tcscmp(str.c_str() + (len1 - len2), endStr.c_str()) == 0;
			}
		}

		return false;
	}

	template<typename T, typename N>
	T StringUtils<T, N>::Trim(const T& wstr, const N* chars)
	{
		T result = wstr;

		if (wstr.empty()) return result;

		if (chars == nullptr)
		{
			chars = L" \n\r\t";
		}

		size_t pos = result.find_first_not_of(chars);
		if (pos != T::npos)
		{
			result.erase(0, pos);
			pos = result.find_last_not_of(chars);
			if (pos != T::npos)
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

	template<typename T, typename N>
	bool StringUtils<T, N>::isNumber(const T& str)
	{
		for (auto& item : str)
		{
			N c = item;
			bool isDigit = _istdigit(c);
			if (iswdigit(c))
				continue;
			else if (c == '.')
				continue;
			else if (c == '-')
				continue;
			else
			{
				return false;
			}
		}

		return true;
	}


	template<typename T, typename N>
	size_t StringUtils<T, N>::hash(T& str)
	{
		std::hash<T> _hash;
		return _hash(str);
	}


}

#endif //_STRING_UTIL_H_