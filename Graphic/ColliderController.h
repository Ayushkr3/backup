#pragma once
#include "Collider.h"
#include "Triangle.h"
#include <map>
#include <algorithm>
class ColliderController {
private:
	//---------------------------------------------//
	class QuadTree {
	private:
		std::vector<Triangle*>Triglist;
		std::map<short, Collision::AABBProjection>& collList;
	public:
		QuadTree(std::vector<Triangle*>triglist, std::map<short, Collision::AABBProjection>& AABBList);
		struct QuadNode {
			QuadNode* NorthWest = nullptr;
			QuadNode* NorthEast = nullptr;
			QuadNode* SouthWest = nullptr;
			QuadNode* SouthEast = nullptr;
			Collision::AABBProjection Vol;
			float CenterX = 0;
			float CenterY = 0;
			std::vector<Triangle*> objects = {};
			bool isLeaf = true;
			bool isNode = false;
			QuadNode* ParentNode = nullptr;
		};
		QuadNode BaseQuad;
		QuadNode* idk;
		//int getMedian();
		bool CheckContained(Collision::AABBProjection Is, Collision::AABBProjection In);
	public:
		QuadNode* QueryndDelete(QuadNode* Node,Triangle* tri);
		void Query(QuadNode* Node, Triangle* tri, QuadNode* Parent, QuadNode* child);
		void UpdatePoisition(QuadNode* Node,Triangle* tri);
		void Reshuffle(QuadNode* Node);
		void Insert(Triangle* tri, QuadNode* BaseQuad);
		void ReInsert(Triangle* tri, QuadNode* BaseQuad);
		void MoveUp(QuadNode*& Node);
		void ConstructQuadTree();
	private:
		QuadNode* getSingleNonNullChild(QuadNode* node, int& count);
		QuadNode* Parent;
	};
	//--------------------------------------------//
	class OcTree {
		std::vector<Triangle*>& Triglist;
		std::map<short, Collision::AABBProjection>& collList;
	public:
		struct OcNode {
			OcNode* childs[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
			Collision::AABBProjection Vol;
			float CenterX = 0;
			float CenterY = 0;
			float CenterZ = 0;
			std::vector<Triangle*> objects = {};
			bool isLeaf = true;
			bool isNode = false;
			OcNode* ParentNode = nullptr;
			OcNode() {};
			OcNode(Collision::AABBProjection inVol) :Vol(inVol) {};
		};
	private:
		OcNode* Parent;
		OcNode* getSingleNonNullChild(OcNode* node, int& count);
	public:
		bool CheckContained(Collision::AABBProjection Is, Collision::AABBProjection In);
		OcNode* lastNode;
		void UpdatePosition(Triangle* tri, OcNode* Node);
		void ReInsert(Triangle* tri, OcNode*& Node);
		OcNode BaseOc;
		void Insert(Triangle* tri, OcNode* BaseOc);
		OcNode* QnD(Triangle* tri,OcNode* Node);
		void ReShuffle(OcNode* Node);
		void MoveUp(OcNode* Node);
		OcTree(std::vector<Triangle*>& triglist, std::map<short, Collision::AABBProjection>& AABBList);
		void ConstructOcTree();
	private:
		std::vector<Triangle*>::iterator getIterator(Triangle* tri);
	public:
		void deleteObject(Triangle* tri);
	};
private:
	std::vector<Triangle*> ObjectToCheckWith;
	void CheckCollision(Triangle* tri, QuadTree::QuadNode* Node,ColliderController::QuadTree* baseTree);
	void CheckCollision(Triangle* tri, OcTree::OcNode* Node);
	//QuadTree* baseTree;
	OcTree* baseOcTree;
	bool isCollidingAABB(Triangle* first,Triangle* second);
	std::vector<Triangle*> Triglist;
	std::map<short, Collision::AABBProjection> collList;
public:
	//
	//-Constrcutor for tree
	//-Creates new tree object
	void ConstructTree();
	//
	//-Push Tree in tree
	//-Should be called with InitalizePosition
	void AddTriangle(Triangle* tri);
	//
	//-Add projection to object maps
	void InitalizePosition();
	void Update(Triangle* tri);
	void deleteObject(Triangle* tri);
	Collision::AABBProjection calcAABB(Triangle& tri);
	bool GlobalCollide = false;
	~ColliderController();
};