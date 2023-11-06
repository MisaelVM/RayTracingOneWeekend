#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material)
	: m_Center(center), m_Radius(radius), m_Material(material) {}

bool Sphere::Hit(const Ray& r, Interval rayInterval, HitRecord& record) const {
	glm::vec3 oc = r.Origin - m_Center;

	float a = glm::dot(r.Direction, r.Direction);
	float half_b = glm::dot(oc, r.Direction);
	float c = glm::dot(oc, oc) - m_Radius * m_Radius;

	float discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return false;

	// Find the nearest root that lies in the acceptable range.
	float sqrtd = glm::sqrt(discriminant);
	float t = (-half_b - sqrtd) / a;
	if (!rayInterval.Surrounds(t)) {
		t = (-half_b + sqrtd) / a;
		if (!rayInterval.Surrounds(t))
			return false;
	}

	record.t = t;
	record.p = r.At(record.t);
	glm::vec3 outwardNormal = (record.p - m_Center) / m_Radius;
	record.SetFaceNormal(r, outwardNormal);
	record.Material = m_Material;

	return true;
}
