#include "HittableList.h"

#include <glm/gtc/constants.hpp>

HittableList::HittableList(std::shared_ptr<Hittable> object) {
	Add(object);
}

void HittableList::Clear() {
	Objects.clear();
}

void HittableList::Add(std::shared_ptr<Hittable> object) {
	Objects.push_back(object);
}

bool HittableList::Hit(const Ray& r, Interval rayInterval, HitRecord& record) const {
	HitRecord tempRec{};
	bool hitAnything = false;
	float closestSoFar = rayInterval.UpperBound();

	for (const auto& object : Objects) {
		if (object->Hit(r, { rayInterval.LowerBound(), closestSoFar}, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			record = tempRec;
		}
	}

	return hitAnything;
}
