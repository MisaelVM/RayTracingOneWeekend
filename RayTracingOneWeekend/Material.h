#pragma once

#include "Common.h"

struct HitRecord;

class Material {
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const glm::vec3& albedo);

	bool Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	glm::vec3 m_Albedo;
};

class Metal : public Material {
public:
	Metal(const glm::vec3& albedo, float fuzz);

	bool Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	glm::vec3 m_Albedo;
	float m_Fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float indexOfRefraction);

	bool Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	static float Reflectance(float cosine, float refIdx);

private:
	float m_IndexOfRefraction;
};
