#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <limits>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <random>

constexpr float infinity = std::numeric_limits<float>::infinity();

inline float RandomFloat() {
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float RandomFloat(float min, float max) {
	return min + (max - min) * RandomFloat();
}

inline glm::vec3 RandomVec3() {
	return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline glm::vec3 RandomVec3(float min, float max) {
	return glm::vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}

inline glm::vec3 RandomInUnitSphere() {
	while (true) {
		glm::vec3 p = RandomVec3(-1.0f, 1.0f);
		if (glm::dot(p, p) < 1.0f)
			return p;
	}
}

inline glm::vec3 RandomUnitVector() {
	return glm::normalize(RandomInUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal) {
	glm::vec3 onUnitSphere = RandomUnitVector();
	if (glm::dot(onUnitSphere, normal) > 0.0f) // In the same hemisphere as the normal
		return onUnitSphere;
	else
		return -onUnitSphere;
}

inline glm::vec3 RandomInUnitDisk() {
	while (true) {
		glm::vec3 p = glm::vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f);
		if (glm::dot(p, p) < 1.0f)
			return p;
	}
}

#include "Ray.h"
#include "Interval.h"
