#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stdarg.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif ///_WIN32

namespace utils{
#ifdef _WIN32
	static inline std::wstring _GetModulePath( int ){
		wchar_t tmp_path[MAX_PATH]={0};
		if( 0 == ::GetModuleFileNameW( ::GetModuleHandle( nullptr),tmp_path , MAX_PATH )){
			return std::wstring();
		}
		std::wstring path = tmp_path;
		auto pos1 = path.find_last_of('\\');
		//auto pos2 = path.find_last_of('/' );
		path = path.substr(0, pos1);
		return path;
	}
	
	static inline std::string _GetModulePath( char) {
		char tmp_path[MAX_PATH]={0};
		if( 0 == ::GetModuleFileNameA( ::GetModuleHandle( nullptr),tmp_path , MAX_PATH )){
			return std::string();
		}
		std::string path = tmp_path;
		auto pos1 = path.find_last_of('\\');
		//auto pos2 = path.find_last_of('/' );
		path = path.substr(0, pos1);
		return path;
	}
	
	template<typename T>
	inline T getModulePath(){
		static_assert(std::is_same<std::string,T>::value || std::is_same<std::wstring,T>::value, "this T must wstring or string" );
		return _GetModulePath( typename std::conditional<std::is_same<std::string,T>::value, char,int>::type()  );
	}
#endif ///_WIN32
	template<typename T, size_t _N>
	size_t array_size(   T ( & array)[_N] ){
		return _N;
	}
	template<typename T>
	size_t array_size( uint32_t size){
		return sizeof(T) * size;
	}

	#ifndef _MSC_VER
	template<int SIZE=128>
	#else
	template<int SIZE>
	#endif /// _MSC_VER
	
	std::string stdprintf(const char *format, ... ) {
		va_list args;
		std::string strValue;
		char value[SIZE]={0};
		va_start( args, format);
		std::vsnprintf( value,SIZE,format,args );
		va_end( args);
		return (strValue = std::string(value));
	}
	
	static inline void trim_left(std::string &s, char ch=' ') {
		if( s.empty() ) {
			return;
		}
		s.erase(0,s.find_first_not_of(ch));
	
	}
	static inline void trim_right(std::string &s, char ch=' ') {
		if (s.empty()) {
			return;
		}
		s.erase(s.find_last_not_of(ch) + 1);
	
	}
	
	static inline void trim( std::string &s, char ch= ' ') {
		trim_left( s ,ch );
		trim_right(s, ch );
	}
	
	
	
	static inline void  to_upper( std::string& str ) {
		std::transform(str.begin(), str.end(), str.begin(), toupper);
	}
	static inline void  to_lower( std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), tolower );
	}
	static inline bool  compare_no_case( std::string const &str1, std::string const & str2  ) {
		std::string _str1 = str1;
		to_lower(_str1 );
		std::string _str2 = str2;
		to_lower(_str2);
		return _str1==_str2;
	}
	
	static inline std::vector<std::string> split(std::string const & strtem,std::string const& div ) {
		std::vector<std::string> strvec;
		std::string::size_type pos1, start_pos,min_pos;
		min_pos = div.length();
		start_pos = std::string::size_type( 0 ); // 结束条件的判断
		bool not_find = false;
		while( false ==  not_find ) {
			not_find = true;
			min_pos = strtem.length();
			for( size_t i = 0 ; i < div.length();++i) {
				pos1 = strtem.find(div[i], start_pos );
				if(   pos1 != std::string::npos ) {
					not_find = false;
					if(  pos1 < min_pos )
					min_pos = pos1;
				}
			}
			if( 0 != min_pos - start_pos  ) {
				std::string sub_str= strtem.substr( start_pos, min_pos - start_pos ) ;
				trim_left( sub_str ,' ');
				trim_right(sub_str ,' ');
				strvec.push_back( sub_str );
	
			}
	#ifdef max
	#undef max
	#undef min
			start_pos += std::max(( min_pos -start_pos ),std::string::size_type(1) );
	#endif //max
	
		}
		return strvec;
	}

#ifdef _MSC_VER
	class Watch{
	public:
		Watch() {
			QueryPerformanceFrequency(&m_freq);
		}
		void start(){
			QueryPerformanceCounter(&m_begin);
		}
		
		long long end(){
			 QueryPerformanceCounter(&m_end);
			 return m_end.QuadPart - m_begin.QuadPart;
		}
		double toTime( long long t ){
			return (double)t / double( m_freq.QuadPart);	
		}

	private:

		LARGE_INTEGER m_freq;
		LARGE_INTEGER m_begin;
		LARGE_INTEGER m_end;

	};
#else 
	
#endif /// _MSC_VER
}
