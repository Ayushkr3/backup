#pragma once
#include <Windows.h>
#include <exception>
#include <string>
#include "dxerr.h"
#define NONDX_ERROR(s) error::error(s,__LINE__)
class error:public std::exception{
public:
	 std::string GeterrorDescription(HRESULT hr);
	 std::string GeterrorName(HRESULT hr);
	 error(HRESULT hr,int linenumber);
	 error(std::string hr, int linenumber);
	 char* what();
};
