#pragma once
#include <Windows.h>
#ifndef MACROS_H
#define MACROS_H
#ifdef API_EXPORT
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif
#define API_ERROR_INPUT(res) if(res==FALSE) throw 0;
#endif

