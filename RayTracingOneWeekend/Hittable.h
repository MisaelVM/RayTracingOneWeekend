#pragma once

#include "Common.h"

class Material;

struct HitRecord {
	glm::vec3 p;
	glm::vec3 Normal;
	std::shared_ptr<Material> Material;
	float t;
	bool FrontFace;

	void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
		// Sets the hit record normal vector;

		FrontFace = glm::dot(r.Direction, outwardNormal) < 0;
		Normal = FrontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, Interval rayInterval, HitRecord& record) const = 0;
};
