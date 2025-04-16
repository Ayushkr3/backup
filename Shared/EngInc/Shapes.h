#pragma once
#include "EPrimitives.h"
#include <DirectXMath.h>
#include "Global.h"
namespace Primitives {
	class BasicShapes {
	public:
		class Circle {
		public:
			PRIMITVE_API static void GetCircleofLongNLat(short Lat,short Long,std::vector<Vertex>& vertices,std::vector<unsigned int>& index);
		};
		class Cube {
		public:
			PRIMITVE_API static void GetBox(std::vector<Vertex>& vertices, std::vector<unsigned int>& index);
		};
		class Plane {
		public:
			PRIMITVE_API static void GetPlane(std::vector<Vertex>& vertices, std::vector<unsigned int>& index);
		};
	};
}
