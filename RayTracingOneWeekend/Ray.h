#pragma once

#include <glm/glm.hpp>

struct Ray {
	glm::vec3 Origin;
	glm::vec3 Direction;

	glm::vec3 At(float t) const {
		return Origin + t * Direction;
	}
};
