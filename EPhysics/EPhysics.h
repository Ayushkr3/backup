#pragma once
#ifndef EPHYSICS_H
#define EPHYSICS_H
#ifdef API_EXPORT
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif
#endif
