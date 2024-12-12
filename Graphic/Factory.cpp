#include "Factory.h"
void ObjectPropertiesFactory::Init() {
	NVPhysx::RigidBody::RegisterFactory();
	NVPhysx::BoxCollider::RegisterFactory();
}