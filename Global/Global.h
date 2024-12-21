#pragma once
#include <vector>
#include <array>
#include <string>
#include "ImGui/imgui.h"
#include <functional>
#include <map>
#include <sstream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
static std::string WorkingDirectory = "D:\\program\\Eng";
struct ObjectProperties;
struct TransformStruct;
inline ImGuiContext* Globalctx = nullptr;
class Objects {
public:
	virtual void Highlight() {}; // If item is selected then do stuff
	virtual void Restore() {}; //If item is unselected restore previous state
	inline static std::vector<short> GlobalIdPool;
	inline static int count = 0;
	struct Inherits {
		Objects* inheritedFrom = nullptr;
		std::vector<Objects*>InheritedObj;//list of all inherited objects;
		TransformStruct* InheritedTrans;
	}Inheritence;
	Objects(short id, std::string ObjName);
	short Id;
	std::string ObjName;
	void SetInheritence(Objects*& o);
	virtual std::vector<ObjectProperties*>* GetProperties() = 0;
};
struct ObjectProperties {
	static void PushToObjectPropertyPool(std::string name,std::function<ObjectProperties*(Objects*)> f, std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>& GlobalPropertiesPoolL);
	static std::multimap<std::string,std::function<ObjectProperties*(Objects*)>> GlobalPropertiesPool; //Implement this
	Objects* associatedObj;
	ObjectProperties(Objects* obj);
	ObjectProperties() {}; //Should never used directly
	virtual void InitPlayMode() {};
	virtual void DeInitPlayMode() {};
	virtual void show() = 0;
	virtual void inPlayMode() {};
	virtual void UpdateDependency(const void*& ptr) {};
	virtual ObjectProperties* GetPropertyRef() = 0;
	virtual const std::type_info& GetPropertyType() = 0;
};
struct TransformStruct :public ObjectProperties {
	TransformStruct(Objects* obj);

public:
	bool isChangedExternally;
	float l_position[3] = { 0,0,0 }; //last position
public:
	ObjectProperties* GetPropertyRef();
	const std::type_info& GetPropertyType();
	bool isMoving;
	void Update();
	float rotation[3] = { 0,0,0 };
	float position[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
	void show();
};
class Globals {
public:
	static bool* inPlayMode;
	static float* dT;
};
struct Vertex {
	struct {
		float x;
		float y;
		float z;
	}position;
	struct {
		float u;
		float v;
	}texcoord;
	struct {
		float x;
		float y;
		float z;
	}Normal;
	Vertex() = default;
	Vertex(std::vector<float> val) :position{ val[0],val[1],val[2] }, texcoord{ val[3],val[4] }, Normal{ val[5],val[6],val[7] } {};
	Vertex(std::initializer_list<float> val) {
		auto it = val.begin();
		position = { *(it++), *(it++), *(it++) };
		texcoord = { *(it++), *(it++) };
		Normal = { *(it++), *(it++), *(it++) };
	};
};
struct NormalPerObject {
	float x;
	float y;
	float z;
	NormalPerObject(float x, float y, float z) :x(x), y(y), z(z) {}
};
struct RefrencePassing {
	void* ObjectPtr;
	const std::type_info& id;
	RefrencePassing(void* ObjectPtr,const std::type_info& id) :ObjectPtr(ObjectPtr),id(id){};
};
std::string GetMemAddress(void* ptr);
//---------------------------------Console Stuff------------------------------//
//----------------------------------------------------------------------------//
class DebugConsole {
private:
	struct SharedData {
		std::queue<std::string>* Message_q;             ///-------------------------------------------------------------///
		std::mutex* mute;								///---|  Main exe    |------|  dll    |----|  dll   |-----------///
	};													///--------|-------------------|--------------|-----------------///
	static SharedData* Data;							///---|Console class|----|Console class|---|Console class|------///
	static std::mutex* mute;							///--------|-------------------|----------------|---------------///
	static std::condition_variable cv;                  ///________--------------------------------------_______________///
	static bool Pushed;									///____________________________|________________________________///
	static bool isFinished;								///_______________________|Shared Queue & mutex|________________///
	static std::thread DebugThread;
	static void RunThread();
	static std::queue<std::string>* Message_q;
	//-----------Shared Paging------------// 
	static HANDLE hMapFile;
	static LPVOID ptrToMem;
	//------------------------------------//
public:
	static DebugConsole* GetInstance();
	static void InitDebugConsole();
	static void DestroyConsole();
	static void Log(std::string Message);
};
#define CONSOLE_PRINT(str) DebugConsole::GetInstance()->Log(str)
//---------------------------------------------------------------------------//