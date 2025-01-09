#pragma once
#ifndef EPRIMITVE_H
#define EPRIMITVE_H
#ifdef API_EXPORT
#define PRIMITVE_API __declspec(dllexport)
#else
#define PRIMITVE_API __declspec(dllimport)
#endif
#endif