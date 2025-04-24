#pragma once
#include <string>
#include <vector>
#ifdef OBJPARSER_EXPORT
#define OBJPARSER_API __declspec(dllexport)
#else
#define OBJPARSER_API __declspec(dllimport)
#endif
//
//-Gets Last GetFile call vertices

extern OBJPARSER_API std::vector<std::vector<float>> Vertexformated;

//- Gets File by location
//- Works with obj file only
extern "C" OBJPARSER_API void GetFile(std::string Loc);
void ProcessLine(std::string Line, std::vector<std::vector<float>>& vec);//Line to fill in vector
void AssembleFace(std::string Line, std::vector<std::vector<float>>& vec);//Assemeble face from vertcies tex and norm data
//
//- GetNormal of Object
extern OBJPARSER_API std::vector<std::vector<float>> ObjNormals; //vn