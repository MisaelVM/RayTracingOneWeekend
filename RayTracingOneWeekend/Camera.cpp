#include "Camera.h"

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>

Camera::Camera(const CameraInfo& camInfo)
	: m_Position(camInfo.Position), m_LookFrom(camInfo.Position), m_LookAt(camInfo.LookAt),
	  m_AspectRatio(camInfo.AspectRatio), m_ImageWidth(camInfo.ImageWidth),
	  m_SamplesPerPixel(camInfo.SampleCount),m_MaxDepth(camInfo.MaxDepth), m_VerticalFOV(camInfo.VerticalFOV),
	  m_DefocusAngle(camInfo.DefocusAngle), m_FocusDist(camInfo.FocusDist)
{
	Initialize();
}

Camera::~Camera() {
	delete[] m_PixelData;
}

void Camera::Render(const Hittable& world, const char* imagePath) {
	for (uint32_t j = 0; j < m_ImageHeight; ++j) {
		std::clog << "\rScanlines remaining: " << (m_ImageHeight - j) << ' ' << std::flush;
		for (uint32_t i = 0; i < m_ImageWidth; ++i) {
			glm::vec4 pixelColor = glm::vec4(0.0f);
			for (uint32_t sample = 0; sample < m_SamplesPerPixel; ++sample) {
				Ray r = GetRay(i, j);
				pixelColor += RayColor(r, m_MaxDepth, world);
			}

			float scale = 1.0f / static_cast<float>(m_SamplesPerPixel);
			pixelColor *= scale;
			glm::vec4 gammaCorrectedColor = glm::sqrt(pixelColor);
			m_PixelData[j * m_ImageWidth + i] = ConvertToPixel(gammaCorrectedColor);
		}
	}
	std::clog << "\rDone.                 \n";
	stbi_write_png(imagePath, m_ImageWidth, m_ImageHeight, m_ImageChannels, m_PixelData, m_ImageWidth * m_ImageChannels);
}

void Camera::Initialize() {
	m_ImageHeight = std::max(static_cast<uint32_t>(m_ImageWidth / m_AspectRatio), 1u);

	// Determine viewport dimensions.
	float theta = glm::radians(m_VerticalFOV);
	float h = glm::tan(theta / 2.0f);
	float viewportHeight = 2 * h * m_FocusDist;
	float viewportWidth = viewportHeight * (static_cast<float>(m_ImageWidth) / m_ImageHeight);

	// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
	m_W = glm::normalize(m_LookFrom - m_LookAt);
	m_U = glm::normalize(glm::cross(m_Up, m_W));
	m_V = glm::cross(m_W, m_U);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	glm::vec3 viewportU = viewportWidth * m_U;
	glm::vec3 viewportV = viewportHeight * -m_V;

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	m_PixelDeltaU = viewportU / static_cast<float>(m_ImageWidth);
	m_PixelDeltaV = viewportV / static_cast<float>(m_ImageHeight);

	// Calculate the location of the upper left pixel.
	glm::vec3 viewportUpperLeft = m_Position - (m_FocusDist * m_W) - viewportU / 2.0f - viewportV / 2.0f;
	m_Pixel00Location = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);

	// Calculate the camera defocus disk basis vectors.
	float defocusRadius = m_FocusDist * glm::tan(glm::radians(m_DefocusAngle / 2.0f));
	m_DefocusDiskU = m_U * defocusRadius;
	m_DefocusDiskV = m_V * defocusRadius;

	m_PixelData = new uint32_t[m_ImageWidth * m_ImageHeight];
}

glm::vec4 Camera::RayColor(const Ray& r, uint32_t depth, const Hittable& world) const {
	HitRecord rec{};

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (world.Hit(r, { 0.001f, infinity }, rec)) {
		Ray scattered{};
		glm::vec3 attenuation{};
		if (rec.Material->Scatter(r, rec, attenuation, scattered))
			return { attenuation * glm::vec3(RayColor(scattered, depth - 1, world)), 1.0f };
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}


	glm::vec3 unitDirection = glm::normalize(r.Direction);
	float a = 0.5f * (unitDirection.y + 1.0f);
	return (1.0f - a) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a * glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
}

Ray Camera::GetRay(uint32_t i, uint32_t j) const {
	// Get a randomly-sampled camera ray for the pixel at location i,j, originating from
	// the camera defocus disk.
	glm::vec3 pixelCenter = m_Pixel00Location + (static_cast<float>(i) * m_PixelDeltaU) + (static_cast<float>(j) * m_PixelDeltaV);
	glm::vec3 pixelSample = pixelCenter + PixelSampleSquare();

	glm::vec3 rayOrigin = (m_DefocusAngle <= 0) ? m_Position : DefocusDiskSample();
	glm::vec3 rayDirection = pixelSample - rayOrigin;

	Ray r{};
	r.Origin = rayOrigin;
	r.Direction = rayDirection;

	return r;
}

glm::vec3 Camera::PixelSampleSquare() const {
	// Returns a random point in the square surrounding a pixel at the origin.
	float px = -0.5f + RandomFloat(0.0f, 1.0f);
	float py = -0.5f + RandomFloat(0.0f, 1.0f);
	return (px * m_PixelDeltaU) + (py * m_PixelDeltaV);
}

glm::vec3 Camera::DefocusDiskSample() const {
	// Returns a random point in the camera defocus disk.
	glm::vec3 p = RandomInUnitDisk();
	return m_Position + (p.x * m_DefocusDiskU) + (p.y * m_DefocusDiskV);
}

uint32_t Camera::ConvertToPixel(const glm::vec4& color) const {
	uint8_t r = static_cast<uint8_t>(std::clamp(color.r * 255.f, 0.0f, 255.0f));
	uint8_t g = static_cast<uint8_t>(std::clamp(color.g * 255.f, 0.0f, 255.0f));
	uint8_t b = static_cast<uint8_t>(std::clamp(color.b * 255.f, 0.0f, 255.0f));
	uint8_t a = static_cast<uint8_t>(std::clamp(color.a * 255.f, 0.0f, 255.0f));

	return (a << 24) | (b << 16) | (g << 8) | r;
}
