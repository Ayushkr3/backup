#include "ColliderController.h"
#include"errors.h"
//TODO: Instead of Triangle vertices we need dupilcate vertices from Triangles collider object
using namespace DirectX;
using namespace Collision;
using namespace std;
#define FNORTHEAST 0
#define FNORTHWEST 1
#define FSOUTHWEST 2
#define FSOUTHEAST 3
#define BNORTHEAST 4
#define BNORTHWEST 5
#define BSOUTHWEST 6
#define BSOUTHEAST 7



#define EXTENT 5
ColliderController::~ColliderController() {
	//delete baseTree;
	delete baseOcTree;
}
void ColliderController::ConstructTree() {
	//baseTree = new QuadTree(Triglist, collList);
	baseOcTree = new OcTree(Triglist, collList);
	
}
Collision::AABBProjection ColliderController::calcAABB(Triangle& tri) {
	AABBProjection proj;
	XMFLOAT3 icap = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 jcap = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 kcap = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR i = XMLoadFloat3(&icap);
	XMVECTOR j = XMLoadFloat3(&jcap);
	XMVECTOR k = XMLoadFloat3(&kcap);
	for (unsigned int s = 0; s < tri.GetVertices().size(); s++) {
		XMFLOAT3 pos = XMFLOAT3(tri.GetVertices()[s].position.x,tri.GetVertices()[s].position.y,tri.GetVertices()[s].position.z);
		XMVECTOR posvec = XMLoadFloat3(&pos);
		float proji = XMVectorGetX(XMVector3Dot(posvec, i)) + tri.position[0];
		float projj = XMVectorGetX(XMVector3Dot(posvec, j)) + tri.position[1];
		float projk = XMVectorGetX(XMVector3Dot(posvec, k)) + tri.position[2];
		proj.maxI = max(proji, proj.maxI);
		proj.minI = min(proji, proj.minI);
		proj.maxJ = max(projj, proj.maxJ);
		proj.minJ = min(projj, proj.minJ);
		proj.maxK = max(projk, proj.maxK);
		proj.minK = min(projk, proj.minK);
	}
	//midpoints.push_back((proj.minI + proj.maxI) / 2);
	return proj;
}
void ColliderController::InitalizePosition() {
	AABBProjection proj;
	for (auto &Triangle:Triglist) {
		proj = calcAABB(*Triangle);
		collList[Triangle->id] = proj;
	}
}
void ColliderController::AddTriangle(Triangle* tri) {
	Triglist.push_back(tri);
}
void ColliderController::Update(Triangle* tri) {
	AABBProjection proj;
	proj = calcAABB(*tri);
	//ColliderController::QuadTree::QuadNode* Node = baseTree->QueryndDelete(&baseTree->BaseQuad, tri);
	ColliderController::OcTree::OcNode* Node = baseOcTree->QnD(tri,&baseOcTree->BaseOc);
	collList[tri->id] = proj;
	//baseTree->UpdatePoisition(Node,tri);
	baseOcTree->UpdatePosition(tri, Node);
	//baseTree->MoveUp(baseTree->idk);
	baseOcTree->MoveUp(baseOcTree->lastNode);
	//CheckCollision(tri,&baseTree->BaseQuad);
	CheckCollision(tri, &baseOcTree->BaseOc);
}
bool ColliderController::isCollidingAABB(Triangle* first, Triangle* second) {
	AABBProjection firstobjproj = collList[first->id];
	AABBProjection secondobjproj = collList[second->id];
	if (firstobjproj.maxI< secondobjproj.minI || firstobjproj.minI > secondobjproj.maxI) return false;
	if (firstobjproj.maxJ < secondobjproj.minJ || firstobjproj.minJ > secondobjproj.maxJ) return false;
	if (firstobjproj.maxK < secondobjproj.minK || firstobjproj.minK > secondobjproj.maxK) return false;
	return true;
}
//Implementation of  O(nlogn) time complexity algortihm
//---------------Broad Phase---------------------//
ColliderController::QuadTree::QuadTree(vector<Triangle*>triglist, std::map<short, Collision::AABBProjection>& AABBList) :
	Triglist(triglist),collList(AABBList)
{
	ConstructQuadTree();
}
void ColliderController::QuadTree::ConstructQuadTree() {
	for (auto& Tri : Triglist) {
		BaseQuad.Vol.maxI = max(BaseQuad.Vol.maxI,collList[Tri->id].maxI);
		BaseQuad.Vol.minI = min(BaseQuad.Vol.minI, collList[Tri->id].minI);
		BaseQuad.Vol.maxJ = max(BaseQuad.Vol.maxJ, collList[Tri->id].maxJ);
		BaseQuad.Vol.minJ = min(BaseQuad.Vol.minJ, collList[Tri->id].minJ);
	}
	BaseQuad.Vol.maxI = BaseQuad.Vol.maxI*5;
	BaseQuad.Vol.minI = BaseQuad.Vol.minI*5;
	BaseQuad.Vol.maxJ = BaseQuad.Vol.maxJ*5;
	BaseQuad.Vol.minJ = BaseQuad.Vol.minJ*5;
	BaseQuad.CenterX = (BaseQuad.Vol.maxI + BaseQuad.Vol.minI) / 2;
	BaseQuad.CenterY = (BaseQuad.Vol.maxJ + BaseQuad.Vol.minJ) / 2;
	BaseQuad.isNode = true;
	BaseQuad.isLeaf = false;
	for (auto& Tri : Triglist) {
		Insert(Tri,&BaseQuad);
	}
}
//Query and delete
ColliderController::QuadTree::QuadNode* ColliderController::QuadTree::QueryndDelete(QuadNode* Node,Triangle* tri) {
	if (Node->isNode) {
		bool found;
		found = [&]()->bool {
			auto it = find(Node->objects.begin(), Node->objects.end(), tri);
			bool yes = (it == Node->objects.end()) ? false : true;
			if (yes) {
				Node->objects.erase(it);
			}
			return yes;
		}();
		if (found)
			return Node; // Found object is in node
		else {
			if (CheckContained(collList[tri->id], { Node->CenterX,Node->Vol.minI,Node->Vol.maxJ,Node->CenterY })) {
				return QueryndDelete(Node->NorthWest, tri);
			}
			else if (CheckContained(collList[tri->id], { Node->Vol.maxI,Node->CenterX,Node->Vol.maxJ,Node->CenterY })) {
				return QueryndDelete(Node->NorthEast, tri);
			}
			else if (CheckContained(collList[tri->id], { Node->CenterX,Node->Vol.minI,Node->CenterY,Node->Vol.minJ })) {
				return QueryndDelete(Node->SouthWest, tri);
			}
			else if(CheckContained(collList[tri->id],{ Node->Vol.maxI,Node->CenterX,Node->CenterY,Node->Vol.minJ})) {
				return QueryndDelete(Node->SouthEast, tri); //FIXing
			}
			else {
				return Node; //It Should never reach this
			}
		}
	}
	else {
		Node->objects.clear();
		return Node; //is Leaf
	}
}
ColliderController::QuadTree::QuadNode* ColliderController::QuadTree::getSingleNonNullChild(QuadNode* node, int& count) {
	QuadNode* nonNullChild = nullptr;
	count = 0;
	if (node->NorthEast != nullptr) {
		nonNullChild = node->NorthEast;
		count++;
	}
	if (node->NorthWest != nullptr) {
		nonNullChild = node->NorthWest;
		count++;
	}
	if (node->SouthEast != nullptr) {
		nonNullChild = node->SouthEast;
		count++;
	}
	if (node->SouthWest != nullptr) {
		nonNullChild = node->SouthWest;
		count++;
	}

	return nonNullChild;
}
void ColliderController::QuadTree::MoveUp(QuadNode*& Node) {
	//should start from Parent
	//check adjcaent node if lead nodes parent is empty move up the node
	//everything should be changed with for loop
	int count;
	if (Node == nullptr)
		return;
	QuadNode* Orphanchild = getSingleNonNullChild(Node,count) ;
	if (Node->objects.size() > 0) {
		return;
	}
		if (count == 1&&Node->ParentNode!=nullptr) {
			Node->objects = Orphanchild->objects;
			Node->NorthEast = Orphanchild->NorthEast;
			Node->NorthWest = Orphanchild->NorthWest;
			Node->SouthEast = Orphanchild->SouthEast;
			Node->SouthWest = Orphanchild->SouthWest;
			Node->isLeaf = true;
			Node->isNode = false;
			MoveUp(Node->ParentNode);
		}
}
void ColliderController::QuadTree::UpdatePoisition(QuadNode* Node,Triangle* tri) {
	//QuadNode* Node =  Query(&BaseQuad, tri);
	Parent = Node->ParentNode;
	if (Node->isNode == true) {
		ReInsert(tri, Node);
		//if (Node->NorthEast == nullptr&&Node->NorthWest == nullptr&&Node->SouthEast == nullptr&&Node->SouthWest == nullptr)
			//delete Node;
	}
	else {
		Query(&BaseQuad, tri, Parent, Node); //nulifying the leaf node so it can be inserted here again
		ReInsert(tri, Parent);
		//if (Parent->NorthEast == nullptr&&Parent->NorthWest == nullptr&&Parent->SouthEast == nullptr&&Parent->SouthWest == nullptr)
			//delete Parent;
	}
}
void ColliderController::QuadTree::Query(QuadNode* Node, Triangle* tri, QuadNode* Parent,QuadNode* child) {
	if (Parent->NorthWest == child) {
		Parent->NorthWest = nullptr;
	}
	else if (Parent->NorthEast == child) {
		Parent->NorthEast = nullptr;
	}
	else if (Parent->SouthWest == child) {
		Parent->SouthWest = nullptr;
	}
	else {
		Parent->SouthEast = nullptr;
	}
}
void ColliderController::QuadTree::ReInsert(Triangle* tri,QuadNode* Node){
	try {
		if (CheckContained(collList[tri->id], Node->Vol)) {
			Insert(tri, Node);
		}
		else {
			ReInsert(tri, Node->ParentNode);
			Node = nullptr;
		}
	}
	catch (...) {
		return;
	}
}
bool ColliderController::QuadTree::CheckContained(AABBProjection Is,AABBProjection In) {
	if (Is.maxI <= In.maxI && Is.minI >= In.minI &&Is.maxJ <= In.maxJ && Is.minJ >= In.minJ /*&&Is.maxK <= In.maxK && Is.minK >= In.minK*/) {
		return true;
	}
	return false;
}
void ColliderController::QuadTree::Reshuffle(QuadNode* Node) {
	Node->isNode = true;
	Node->isLeaf = false;
	Triangle* tri = *Node->objects.begin();
	Node->objects.erase(Node->objects.begin());
	Insert(tri, Node);
}
void ColliderController::QuadTree::Insert(Triangle* tri, QuadNode* BaseQuad) {
	//NorthWest
	//BaseQuad->objects.push_back(tri);
	if (CheckContained(collList[tri->id],{ BaseQuad->CenterX,BaseQuad->Vol.minI,BaseQuad->Vol.maxJ,BaseQuad->CenterY })){
		if (BaseQuad->NorthWest == nullptr) {
			QuadNode* northWest = new QuadNode;
			northWest->Vol = { BaseQuad->CenterX,BaseQuad->Vol.minI,BaseQuad->Vol.maxJ,BaseQuad->CenterY };
			northWest->CenterX = (northWest->Vol.maxI + northWest->Vol.minI) / 2;
			northWest->CenterY = (northWest->Vol.maxJ + northWest->Vol.minJ) / 2;
			BaseQuad->NorthWest = northWest;
			BaseQuad->NorthWest->objects.push_back(tri);
			BaseQuad->NorthWest->ParentNode = BaseQuad;
			idk = BaseQuad->NorthWest->ParentNode;
		}
		else {
			if (!BaseQuad->NorthWest->isNode) {
				Reshuffle(BaseQuad->NorthWest);
			}
			Insert(tri, BaseQuad->NorthWest);
		}
	}
	//NorthEast
	else if (CheckContained(collList[tri->id], { BaseQuad->Vol.maxI,BaseQuad->CenterX,BaseQuad->Vol.maxJ,BaseQuad->CenterY })) {
		if (BaseQuad->NorthEast == nullptr) {
			QuadNode* northEast = new QuadNode;
			northEast->Vol = { BaseQuad->Vol.maxI,BaseQuad->CenterX,BaseQuad->Vol.maxJ,BaseQuad->CenterY };
			northEast->CenterX = (northEast->Vol.maxI + northEast->Vol.minI) / 2;
			northEast->CenterY = (northEast->Vol.maxJ + northEast->Vol.minJ) / 2;
			BaseQuad->NorthEast = northEast;
			BaseQuad->NorthEast->objects.push_back(tri);
			BaseQuad->NorthEast->ParentNode = BaseQuad;
			idk = BaseQuad->NorthEast->ParentNode;
		}
		else {
			if (!BaseQuad->NorthEast->isNode) {
				Reshuffle(BaseQuad->NorthEast);
			}
			Insert(tri, BaseQuad->NorthEast);
		}
	}
	//SouthWest
	else if (CheckContained(collList[tri->id], { BaseQuad->CenterX,BaseQuad->Vol.minI,BaseQuad->CenterY,BaseQuad->Vol.minJ })) {
	if (BaseQuad->SouthWest == nullptr) {
			QuadNode* southWest = new QuadNode;
			southWest->Vol = { BaseQuad->CenterX,BaseQuad->Vol.minI,BaseQuad->CenterY,BaseQuad->Vol.minJ };
			southWest->CenterX = (southWest->Vol.maxI + southWest->Vol.minI) / 2;
			southWest->CenterY = (southWest->Vol.maxJ + southWest->Vol.minJ) / 2;
			BaseQuad->SouthWest = southWest;
			BaseQuad->SouthWest->objects.push_back(tri);
			BaseQuad->SouthWest->ParentNode = BaseQuad;
			idk = BaseQuad->SouthWest->ParentNode;
	}
	else {
		if (!BaseQuad->SouthWest->isNode) {
			Reshuffle(BaseQuad->SouthWest);
		}
		Insert(tri, BaseQuad->SouthWest);
	}
	}
	//SouthEast
	else if(CheckContained(collList[tri->id], { BaseQuad->Vol.maxI,BaseQuad->CenterX,BaseQuad->CenterY,BaseQuad->Vol.minJ })){
		if (BaseQuad->SouthEast == nullptr) {
			QuadNode* southEast = new QuadNode;
			southEast->Vol = { BaseQuad->Vol.maxI,BaseQuad->CenterX,BaseQuad->CenterY,BaseQuad->Vol.minJ };
			southEast->CenterX = (southEast->Vol.maxI + southEast->Vol.minI) / 2;
			southEast->CenterY = (southEast->Vol.maxJ + southEast->Vol.minJ) / 2;
			BaseQuad->SouthEast = southEast;
			BaseQuad->SouthEast->objects.push_back(tri);
			BaseQuad->SouthEast->ParentNode = BaseQuad; 
			idk = BaseQuad->SouthEast->ParentNode;
		}
		else {
			if (!BaseQuad->SouthEast->isNode) {
				Reshuffle(BaseQuad->SouthEast);
			}
			Insert(tri, BaseQuad->SouthEast);
		}
	}
	else {
		// Insert in Node
		idk = nullptr;
		BaseQuad->objects.push_back(tri);
	}
}
void ColliderController::CheckCollision(Triangle* tri, QuadTree::QuadNode* Node, ColliderController::QuadTree* baseTree) {
	//goto northwest with all previous node object
	//goto northeast  "    "    "        "   "
	//goto southewest
	//goto southeast
	if (Node->isNode) {
		bool found = false;
		for (unsigned int k = 0; k < ObjectToCheckWith.size(); k++) {
			for (unsigned int l = 0; l < Node->objects.size(); l++) {
				if (isCollidingAABB(ObjectToCheckWith[k], Node->objects[l])) {
					ObjectToCheckWith[k]->UpdateCollider();
					Node->objects[l]->UpdateBuffers();
					GlobalCollide = ObjectToCheckWith[k]->coll.CheckCollision(Node->objects[l]->coll) && Node->objects[l]->coll.CheckCollision(ObjectToCheckWith[k]->coll);
					//go to narrow phase collision check
				}
				else {
					GlobalCollide = false;
				}
			}
		}
		for (unsigned int i = 0; i < Node->objects.size(); i++) {
			ObjectToCheckWith.push_back(Node->objects[i]);
			if (Node->objects[i]->id == tri->id)
				found = true;
			if (i == Node->objects.size() - 1)
				break;
			for (unsigned int j = i + 1; j < Node->objects.size(); j++) {
				if (isCollidingAABB(Node->objects[i], Node->objects[j])) {
					Node->objects[i]->UpdateCollider();
					Node->objects[j]->UpdateBuffers();
					GlobalCollide = Node->objects[i]->coll.CheckCollision(Node->objects[j]->coll) && Node->objects[j]->coll.CheckCollision(Node->objects[i]->coll);
					//go to narrow phase collision check
				}
				else {
					GlobalCollide = false;
				}
			}
		}
		if (found) {
			ObjectToCheckWith.clear();
			return;
		}
		if (baseTree->CheckContained(collList[tri->id], { Node->CenterX,Node->Vol.minI,Node->Vol.maxJ,Node->CenterY })) {
			CheckCollision(tri, Node->NorthWest, baseTree);
		}
		else if (baseTree->CheckContained(collList[tri->id], { Node->Vol.maxI,Node->CenterX,Node->Vol.maxJ,Node->CenterY })) {
			CheckCollision(tri, Node->NorthEast, baseTree);
		}
		else if (baseTree->CheckContained(collList[tri->id], { Node->Vol.maxI,Node->CenterX,Node->CenterY,Node->Vol.minJ })) {
			CheckCollision(tri, Node->SouthEast, baseTree);
		}
		else if (baseTree->CheckContained(collList[tri->id], { Node->CenterX,Node->Vol.minI,Node->CenterY,Node->Vol.minJ })) {
			CheckCollision(tri, Node->SouthWest, baseTree);
		}
		else {
			Node = nullptr; //should never reach here
		}
	}
	else {
		if (Node->objects[0]->id == tri->id) {
			for (unsigned int k = 0; k < ObjectToCheckWith.size(); k++) {
				for (unsigned int l = 0; l < Node->objects.size(); l++) {
					if (isCollidingAABB(ObjectToCheckWith[k], Node->objects[l])) {
						ObjectToCheckWith[k]->UpdateCollider();
						Node->objects[l]->UpdateBuffers();
						GlobalCollide = ObjectToCheckWith[k]->coll.CheckCollision(Node->objects[l]->coll) && Node->objects[l]->coll.CheckCollision(ObjectToCheckWith[k]->coll);
						//go to narrow phase collision check
					}
					else {
						GlobalCollide = false;
					}
				}
			}
			ObjectToCheckWith.clear();
		}
		else {
			throw tri->id; //should never reach here
		}
	}
}
ColliderController::OcTree::OcTree(vector<Triangle*>triglist, std::map<short, AABBProjection>& AABBList)
	:Triglist(triglist),collList(AABBList)
{
	ConstructOcTree();
}
bool ColliderController::OcTree::CheckContained(AABBProjection Is, AABBProjection In) {
	if (Is.maxI <= In.maxI && Is.minI >= In.minI &&Is.maxJ <= In.maxJ && Is.minJ >= In.minJ &&Is.maxK <= In.maxK && Is.minK >= In.minK) {
		return true;
	}
	return false;
}
void ColliderController::OcTree::ConstructOcTree() {
	for (auto& Tri : Triglist) {
		BaseOc.Vol.maxI = max(BaseOc.Vol.maxI, collList[Tri->id].maxI);
		BaseOc.Vol.minI = min(BaseOc.Vol.minI, collList[Tri->id].minI);
		BaseOc.Vol.maxJ = max(BaseOc.Vol.maxJ, collList[Tri->id].maxJ);
		BaseOc.Vol.minJ = min(BaseOc.Vol.minJ, collList[Tri->id].minJ);
		BaseOc.Vol.maxK = max(BaseOc.Vol.maxK, collList[Tri->id].maxK);
		BaseOc.Vol.minK = min(BaseOc.Vol.minK, collList[Tri->id].minK);
	}
	BaseOc.Vol*EXTENT;
	BaseOc.CenterX = (BaseOc.Vol.maxI + BaseOc.Vol.minI) / 2;
	BaseOc.CenterY = (BaseOc.Vol.maxJ + BaseOc.Vol.minJ) / 2;
	BaseOc.CenterZ = (BaseOc.Vol.maxK + BaseOc.Vol.minK) / 2;
	BaseOc.childs[FNORTHWEST] = new OcNode({BaseOc.CenterX,BaseOc.Vol.minI,BaseOc.Vol.maxJ,BaseOc.CenterY,BaseOc.CenterZ,BaseOc.Vol.minK });
	BaseOc.childs[FNORTHEAST] = new OcNode({BaseOc.Vol.maxI,BaseOc.CenterX,BaseOc.Vol.maxJ,BaseOc.CenterY,BaseOc.CenterZ,BaseOc.Vol.minK });
	BaseOc.childs[FSOUTHWEST] = new OcNode({BaseOc.CenterX,BaseOc.Vol.minI,BaseOc.CenterY,BaseOc.Vol.minJ,BaseOc.CenterZ,BaseOc.Vol.minK });
	BaseOc.childs[FSOUTHEAST] = new OcNode({BaseOc.Vol.maxI,BaseOc.CenterX,BaseOc.CenterY,BaseOc.Vol.minJ,BaseOc.CenterZ,BaseOc.Vol.minK });
	BaseOc.childs[BNORTHWEST] = new OcNode({ BaseOc.CenterX,BaseOc.Vol.minI,BaseOc.Vol.maxJ,BaseOc.CenterY,BaseOc.Vol.maxK,BaseOc.CenterZ});
	BaseOc.childs[BNORTHEAST] = new OcNode({ BaseOc.Vol.maxI,BaseOc.CenterX,BaseOc.Vol.maxJ,BaseOc.CenterY,BaseOc.Vol.maxK,BaseOc.CenterZ});
	BaseOc.childs[BSOUTHWEST] = new OcNode({ BaseOc.CenterX,BaseOc.Vol.minI,BaseOc.CenterY,BaseOc.Vol.minJ,BaseOc.Vol.maxK,BaseOc.CenterZ});
	BaseOc.childs[BSOUTHEAST] = new OcNode({ BaseOc.Vol.maxI,BaseOc.CenterX,BaseOc.CenterY,BaseOc.Vol.minJ,BaseOc.Vol.maxK,BaseOc.CenterZ});
	BaseOc.isNode = true;
	BaseOc.isLeaf = false;
	for (auto& Tri : Triglist) {
		Insert(Tri, &BaseOc);
	}
}
void ColliderController::OcTree::ReShuffle(OcNode* Node) {
	Node->isNode = true;
	Node->isLeaf = false;
	Triangle* tri = *Node->objects.begin();
	Node->objects.erase(Node->objects.begin());
	Insert(tri, Node);
}
void ColliderController::OcTree::Insert(Triangle* tri, OcNode* BaseOc) {
	bool consumed = false;
	for (int i = 0; i < 8; i++) {
		if (CheckContained(collList[tri->id], BaseOc->childs[i]->Vol)) {
			consumed = true;
			if (BaseOc->childs[i]->objects.empty()&&BaseOc->childs[i]->isNode == false) {
				BaseOc->childs[i]->CenterX = (BaseOc->childs[i]->Vol.maxI + BaseOc->childs[i]->Vol.minI) / 2;
				BaseOc->childs[i]->CenterY = (BaseOc->childs[i]->Vol.maxJ + BaseOc->childs[i]->Vol.minJ) / 2;
				BaseOc->childs[i]->CenterZ = (BaseOc->childs[i]->Vol.maxK + BaseOc->childs[i]->Vol.minK) / 2;
				BaseOc->childs[i]->childs[FNORTHWEST] = new OcNode({ BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.minI,BaseOc->childs[i]->Vol.maxJ,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->CenterZ,BaseOc->childs[i]->Vol.minK });
				BaseOc->childs[i]->childs[FNORTHEAST] = new OcNode({ BaseOc->childs[i]->Vol.maxI,BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.maxJ,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->CenterZ,BaseOc->childs[i]->Vol.minK });
				BaseOc->childs[i]->childs[FSOUTHWEST] = new OcNode({ BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.minI,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.minJ,BaseOc->childs[i]->CenterZ,BaseOc->childs[i]->Vol.minK });
				BaseOc->childs[i]->childs[FSOUTHEAST] = new OcNode({ BaseOc->childs[i]->Vol.maxI,BaseOc->childs[i]->CenterX,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.minJ,BaseOc->childs[i]->CenterZ,BaseOc->childs[i]->Vol.minK });
				BaseOc->childs[i]->childs[BNORTHWEST] = new OcNode({ BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.minI,BaseOc->childs[i]->Vol.maxJ,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.maxK,BaseOc->childs[i]->CenterZ });
				BaseOc->childs[i]->childs[BNORTHEAST] = new OcNode({ BaseOc->childs[i]->Vol.maxI,BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.maxJ,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.maxK,BaseOc->childs[i]->CenterZ });
				BaseOc->childs[i]->childs[BSOUTHWEST] = new OcNode({ BaseOc->childs[i]->CenterX,BaseOc->childs[i]->Vol.minI,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.minJ,BaseOc->childs[i]->Vol.maxK,BaseOc->childs[i]->CenterZ });
				BaseOc->childs[i]->childs[BSOUTHEAST] = new OcNode({ BaseOc->childs[i]->Vol.maxI,BaseOc->childs[i]->CenterX,BaseOc->childs[i]->CenterY,BaseOc->childs[i]->Vol.minJ,BaseOc->childs[i]->Vol.maxK,BaseOc->childs[i]->CenterZ });
				BaseOc->childs[i]->objects.push_back(tri);
				BaseOc->childs[i]->ParentNode = BaseOc;
				lastNode = BaseOc;
				break;
			}
			else {
				if (!BaseOc->childs[i]->isNode) {
					ReShuffle(BaseOc->childs[i]);
				}
				Insert(tri, BaseOc->childs[i]);
			}
		}
	}
	if (!consumed) {
		BaseOc->objects.push_back(tri);
	}
}
ColliderController::OcTree::OcNode* ColliderController::OcTree::QnD(Triangle* tri,OcNode* Node) {
	if (Node->isNode) {
		bool found;
		found = [&]()->bool {
			auto it = find(Node->objects.begin(), Node->objects.end(), tri);
			bool yes = (it == Node->objects.end()) ? false : true;
			if (yes) {
				Node->objects.erase(it);
			}
			return yes;
		}();
		if (found)
			return Node; // Found object is in node
		else {
			bool debugCheck = false;
			try {
				for (int i = 0; i < 8; i++) {
					if (CheckContained(collList[tri->id], Node->childs[i]->Vol)) {
						debugCheck = true;
						return QnD(tri, Node->childs[i]);
					}
				}
				if (!debugCheck) {
					throw debugCheck;
				}
			}
			catch (bool e) {
				NONDX_ERROR("Unexpected outcome in Nodes " + to_string(e));
			}
		}
		return nullptr;  //should never reach here
	}
	else {
		Node->objects.clear();
		return Node; //is Leaf
	}
}
void ColliderController::OcTree::UpdatePosition(Triangle* tri, OcNode* Node) {
	Parent = Node->ParentNode;
	if (Node->isNode == true) {
		ReInsert(tri, Node);
	}
	else {
		 //nulifying the leaf node so it can be inserted here again
		ReInsert(tri, Parent);
	}
}
ColliderController::OcTree::OcNode* ColliderController::OcTree::getSingleNonNullChild(OcNode* node,int& count) {
	OcNode* nonNullChild = nullptr;
	count = 0;
	for (int i = 0; i < 8; i++) {
		if (node->childs[i]->isNode == true) {
			count = 9;
			return nonNullChild;
		}
		if (node->childs[i]->objects.size() != 0) {
			count++;
			nonNullChild = node->childs[i];
		}
	}
	return nonNullChild;
}
void ColliderController::OcTree::MoveUp(OcNode* Node) {
	int count;
	if (Node == nullptr)
		return;
	OcNode* Orphanchild = getSingleNonNullChild(Node, count);
	if (Node->objects.size() > 0) {
		return;
	}
	if (count == 1 && Node->ParentNode != nullptr) {
		Node->objects = Orphanchild->objects;
		for (int i = 0; i < 8; i++) {
			delete Orphanchild->childs[i];
			Orphanchild->childs[i] = nullptr;
		}
		Orphanchild->objects.clear();
		Node->isLeaf = true;
		Node->isNode = false;
		MoveUp(Node->ParentNode);
	}
}
void ColliderController::OcTree::ReInsert(Triangle* tri,OcNode*& Node) {
	try {
		if (Node == nullptr)
			throw 1;
		if (CheckContained(collList[tri->id], Node->Vol)) {
			Insert(tri, Node);
		}
		else {
			ReInsert(tri, Node->ParentNode);//?????
		}
	}
	catch (...) {
		NONDX_ERROR("Out of Bound");
	}
}
void ColliderController::CheckCollision(Triangle* tri, OcTree::OcNode* Node) {
	if (Node->isNode) {
		bool found = false;
		for (unsigned int k = 0; k < ObjectToCheckWith.size(); k++) {
			for (unsigned int l = 0; l < Node->objects.size(); l++) {
				if (isCollidingAABB(ObjectToCheckWith[k], Node->objects[l])) {
					ObjectToCheckWith[k]->UpdateCollider();
					Node->objects[l]->UpdateBuffers();
					GlobalCollide = ObjectToCheckWith[k]->coll.CheckCollision(Node->objects[l]->coll) && Node->objects[l]->coll.CheckCollision(ObjectToCheckWith[k]->coll);
					if(GlobalCollide)
						Physics_Body::gravity = 0.0f;
					//go to narrow phase collision check
				}
				else {
					GlobalCollide = false;
				}
			}
		}
		for (unsigned int i = 0; i < Node->objects.size(); i++) {
			ObjectToCheckWith.push_back(Node->objects[i]);
			if (Node->objects[i]->id == tri->id)
				found = true;
			if (i == Node->objects.size() - 1)
				break;
			for (unsigned int j = i + 1; j < Node->objects.size(); j++) {
				if (isCollidingAABB(Node->objects[i], Node->objects[j])) {
					Node->objects[i]->UpdateCollider();
					Node->objects[j]->UpdateBuffers();
					GlobalCollide = Node->objects[i]->coll.CheckCollision(Node->objects[j]->coll) && Node->objects[j]->coll.CheckCollision(Node->objects[i]->coll);
					//go to narrow phase collision check
					if (GlobalCollide)
						Physics_Body::gravity = 0.0f;
				}
				else {
					GlobalCollide = false;
				}
			}
		}
		if (found) {
			ObjectToCheckWith.clear();
			return;
		}
		bool debugCheck = false;
		for (int i = 0; i < 8; i++) {
			if (baseOcTree->CheckContained(collList[tri->id],Node->childs[i]->Vol)) {
				debugCheck = true;
				CheckCollision(tri,Node->childs[i]);
			}
		}
		if (!debugCheck) {
			throw debugCheck;
		}
	}
	else {
		if (Node->objects[0]->id == tri->id) {
			for (unsigned int k = 0; k < ObjectToCheckWith.size(); k++) {
				for (unsigned int l = 0; l < Node->objects.size(); l++) {
					if (isCollidingAABB(ObjectToCheckWith[k], Node->objects[l])) {
						ObjectToCheckWith[k]->UpdateCollider();
						Node->objects[l]->UpdateBuffers();
						GlobalCollide = ObjectToCheckWith[k]->coll.CheckCollision(Node->objects[l]->coll) && Node->objects[l]->coll.CheckCollision(ObjectToCheckWith[k]->coll);
						//go to narrow phase collision check
						if (GlobalCollide)
							Physics_Body::gravity = 0.0f;
					}
					else {
						GlobalCollide = false;
					}
				}
			}
			ObjectToCheckWith.clear();
		}
		else {
			throw tri->id; //should never reach here
		}
	}
}
//---------------------------------------------//