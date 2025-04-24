#pragma once
#ifndef ECORE_H
#define ECORE_H
#ifdef API_EXPORT
#define CORE_EXP __declspec(dllexport)
#else
#define CORE_EXP __declspec(dllimport)
#endif
#endif