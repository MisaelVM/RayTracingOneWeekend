#pragma once

#include "Common.h"

#include "Hittable.h"
#include "Material.h"

struct CameraInfo {
	glm::vec3 Position;
	glm::vec3 LookAt;

	float AspectRatio;
	uint32_t ImageWidth;
	uint32_t SampleCount;
	uint32_t MaxDepth;
	float VerticalFOV;

	float DefocusAngle;
	float FocusDist;
};

class Camera {
public:
	Camera(const CameraInfo& camInfo);
	~Camera();

	void Render(const Hittable& world, const char* imagePath);

private:
	void Initialize();
	glm::vec4 RayColor(const Ray& r, uint32_t depth, const Hittable& world) const;
	Ray GetRay(uint32_t i, uint32_t j) const;
	glm::vec3 PixelSampleSquare() const;
	glm::vec3 DefocusDiskSample() const;
	uint32_t ConvertToPixel(const glm::vec4& color) const;

private:
	float m_AspectRatio;
	float m_VerticalFOV = 90.0f;
	uint32_t m_SamplesPerPixel = 10;
	uint32_t m_MaxDepth = 10;
	uint32_t m_ImageWidth;
	uint32_t m_ImageHeight;
	uint32_t m_ImageChannels = 4;
	glm::vec3 m_Position;
	glm::vec3 m_Pixel00Location;
	glm::vec3 m_PixelDeltaU;
	glm::vec3 m_PixelDeltaV;

	glm::vec3 m_LookFrom;
	glm::vec3 m_LookAt;
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_U, m_V, m_W;

	float m_DefocusAngle = 0.0f;
	float m_FocusDist = 10.0f;
	glm::vec3 m_DefocusDiskU;
	glm::vec3 m_DefocusDiskV;

	uint32_t* m_PixelData = nullptr;
};
