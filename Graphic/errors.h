#pragma once
#include <Windows.h>
#include <exception>
#include <string>
#include "dxerr.h"
class error:public std::exception{
public:
	 std::string GeterrorDescription(HRESULT hr);
	 std::string GeterrorName(HRESULT hr);
	error(HRESULT hr,int linenumber);
	
};
