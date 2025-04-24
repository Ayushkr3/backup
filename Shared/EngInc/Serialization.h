#pragma once
#include "Global.h"
#include <fstream>
#include <iomanip>
#include <any>
struct Serial {
	void* ObjectPtr;
	const std::type_info& id;
	const char* name;
	Serial(const char* name, void* ObjectPtr, const std::type_info& id) :ObjectPtr(ObjectPtr), id(id), name(name) {};
};
namespace Serialization {
	struct PropertyBlock {
		std::string PropertyNames;
		std::string PropertyBuffer;
		PropertyBlock(std::string PropertyNames, std::string PropertyBuffer):PropertyNames(PropertyNames), PropertyBuffer(PropertyBuffer){}
	};
	struct ObjectBlocks {
		std::string ClassName;
		std::string blockBuffer;
		short Id;
		std::vector<PropertyBlock> propBlocks;
		ObjectBlocks(short Id,std::string classname,std::string blockBuffer):ClassName(classname),blockBuffer(blockBuffer),Id(Id){}
	};
	template <typename T>
	std::string getVal(T x) {
		if constexpr (std::is_same_v<T, std::string>) {
			return x;
		}
		else if constexpr (std::is_arithmetic_v<T>) {
			return std::to_string(x);
		}
		else {
			return "<unsupported>";
		}
	}
	template <typename T>
	std::string SerializeProperty(T& prop) {
		std::tuple x = prop.reflect();
		std::string s;
		std::ostringstream bytes;
		int i = 0;
		std::apply([&](auto&&... pairs) {
			((s += std::to_string(i++) + ":" + pairs.first.name + ":" + getVal(pairs.second) + "\n"), ...);
		}, x);
		s = s + "{/" + prop.GetPropertyClassName() + "}" + "\n";
		bytes << std::setw(5) << std::setfill('0') << (s.size());
		std::string buffer = "{" + prop.GetPropertyClassName() + ":" + bytes.str() + "}" + "\n";
		s = buffer + s;
		return s;
		//Add bytes to this
	}
	template <typename T>
	std::string SerializeProperty(T& prop, std::string val) {
		std::string s;
		std::ostringstream bytes;
		bytes << std::setw(5) << std::setfill('0') << (val.size() + prop.GetPropertyClassName().size() + 3);
		s = s + "{" + prop.GetPropertyClassName() + ":" + bytes.str() + "}" + "\n";
		s = s + val;
		s = s + "{/" + prop.GetPropertyClassName() + "}" + "\n";
		return s;
	}
	template <typename T>
	void DeSerializeObject(T*& Obj, PropertyBlock segment) {
		if (HotReloading::PropertiesPool == nullptr) {
			auto ctx = ImGui::GetCurrentContext();
			HotReloading::GetInstance();
			ImGui::SetCurrentContext(ctx);
		}
		auto functorIT = HotReloading::PropertiesPool->find(segment.PropertyNames);
		if (functorIT != HotReloading::PropertiesPool->end()) {
			ObjectProperties* prop = functorIT->second(Obj);
			//Pushing into list done by individual property
			prop->DeSerialize(segment.PropertyBuffer);
		}
		
	}
	template <typename T>
	std::string SerializeObject(T& Obj) {
		std::vector<ObjectProperties*> x = *(Obj.GetProperties());
		std::string s;
		std::string buffer;
		for (int i = 0; i < x.size();i++) {
			ObjectProperties* prop = x[i];
			buffer += prop->Serialize();
		}
		std::ostringstream bytes;
		bytes << std::setw(5) << std::setfill('0') << (buffer.size() + Obj.ObjName.size() + 3+Obj.Serialize().size());
		std::ostringstream index;
		index << std::setw(3) << std::setfill('0') <<Obj.Id;
		s = s + "[" + Obj.ObjName +":"+index.str()+":" + bytes.str() + "]\n";
		s = s + Obj.Serialize();
		s = s + buffer;
		s = s + "[/" + Obj.ObjName + "]" + "\n";
		return s;
	}
	std::vector<ObjectBlocks> ReadFromFile();
	void SaveToFile(std::string buffer);
}