#include "Material.h"

#include "Hittable.h"

Lambertian::Lambertian(const glm::vec3& albedo)
	: m_Albedo(albedo) {}

bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
	glm::vec3 scatterDirection = rec.Normal + RandomUnitVector();

	float s = 1e-8f;
	bool nearZero = (fabs(scatterDirection.x) < s) && (fabs(scatterDirection.y) < s) && (fabs(scatterDirection.z) < s);
	if (nearZero)
		scatterDirection = rec.Normal;

	scattered = Ray{ rec.p, scatterDirection };
	attenuation = m_Albedo;
	return true;
}

Metal::Metal(const glm::vec3& albedo, float fuzz)
	: m_Albedo(albedo), m_Fuzz(fuzz < 1 ? fuzz : 1) {}

bool Metal::Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
	glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.Direction), rec.Normal);
	scattered = Ray{ rec.p, reflected + m_Fuzz * RandomUnitVector() };
	attenuation = m_Albedo;
	return (glm::dot(scattered.Direction, rec.Normal) > 0);
}

Dielectric::Dielectric(float indexOfRefraction)
	: m_IndexOfRefraction(indexOfRefraction) {}

bool Dielectric::Scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
	attenuation = glm::vec3(1.0f);
	float refractionRatio = rec.FrontFace ? (1.0f / m_IndexOfRefraction) : m_IndexOfRefraction;


	glm::vec3 unitDirection = glm::normalize(rayIn.Direction);
	float cosTheta = fmin(glm::dot(-unitDirection, rec.Normal), 1.0f);
	float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

	bool cannotRefract = refractionRatio * sinTheta > 1.0f;
	glm::vec3 direction{};

	if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomFloat())
		direction = glm::reflect(unitDirection, rec.Normal);
	else
		direction = glm::refract(unitDirection, rec.Normal, refractionRatio);

	glm::vec3 refracted = glm::refract(unitDirection, rec.Normal, refractionRatio);
	scattered = Ray{ rec.p, direction };
	return true;
}

float Dielectric::Reflectance(float cosine, float refIdx) {
	// Use Schlick's approximation for reflectance
	float r0 = (1.0f - refIdx) / (1.0f + refIdx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * glm::pow((1.0f - cosine), 5.0f);
}
