#pragma once

#include "Hittable.h"

#include <glm/glm.hpp>

class Sphere : public Hittable {
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material);

	bool Hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;

private:
	glm::vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Material;
};
