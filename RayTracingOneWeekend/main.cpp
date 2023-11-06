#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"

#include "Camera.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"

#include <iostream>
#include <string>

void RenderScene1(std::string path);
void RenderScene2(std::string path);
void RenderScene3(std::string path);

int main()
{
	RenderScene1("../Images/RayTracingScene1.png");
	RenderScene2("../Images/RayTracingScene2.png");
	RenderScene3("../Images/RayTracingScene3.png");

	return 0;
}

void RenderScene1(std::string path) {
	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	auto materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	auto materialLeft = std::make_shared<Dielectric>(1.5f);
	auto materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f);

	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, materialCenter));
	world.Add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	world.Add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), -0.4f, materialLeft));
	world.Add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

	CameraInfo camInfo{};
	camInfo.Position = glm::vec3(-2.0f, 2.0f, 1.0f);
	camInfo.LookAt = glm::vec3(0.0f, 0.0f, -1.0f);

	camInfo.AspectRatio = 16.0f / 9.0f;
	camInfo.ImageWidth = 400;
	camInfo.SampleCount = 100;
	camInfo.MaxDepth = 50;
	camInfo.VerticalFOV = 20.0f;

	camInfo.DefocusAngle = 10.0f;
	camInfo.FocusDist = 3.4f;

	Camera cam(camInfo);

	cam.Render(world, path.c_str());
}

void RenderScene2(std::string path) {
	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(glm::vec3(0.4f, 0.6f, 0.8f));
	auto materialLambert = std::make_shared<Lambertian>(glm::vec3(0.3f, 0.2f, 0.6f));
	auto materialDielectric = std::make_shared<Dielectric>(1.5f);
	auto materialMetal = std::make_shared<Metal>(glm::vec3(0.8f, 0.7f, 0.1f), 0.0f);

	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.Add(std::make_shared<Sphere>(glm::vec3(5.0f, 0.6f, -3.0f), 1.2f, materialLambert));
	world.Add(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.0f, -1.0f), 0.5f, materialDielectric));
	world.Add(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.0f, -1.0f), -0.4f, materialDielectric));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.5f, -1.0f), 1.0f, materialMetal));
	world.Add(std::make_shared<Sphere>(glm::vec3(2.0f, 0.0f, 1.0f), 0.5f, materialDielectric));
	world.Add(std::make_shared<Sphere>(glm::vec3(2.0f, 0.0f, 1.0f), -0.4f, materialDielectric));
	world.Add(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.0f, 0.8f), 0.8f, materialLambert));
	world.Add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, 2.8f), 0.8f, materialLambert));

	CameraInfo camInfo{};
	camInfo.Position = glm::vec3(-1.0f, 1.0f, 1.0f);
	camInfo.LookAt = glm::vec3(0.0f, 0.0f, -1.0f);

	camInfo.AspectRatio = 16.0f / 9.0f;
	camInfo.ImageWidth = 800;
	camInfo.SampleCount = 100;
	camInfo.MaxDepth = 50;
	camInfo.VerticalFOV = 90.0f;

	camInfo.DefocusAngle = 0.0f;
	camInfo.FocusDist = 10.0f;

	Camera cam(camInfo);

	cam.Render(world, path.c_str());
}

void RenderScene3(std::string path) {
	HittableList world;

	auto groundMaterial = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, groundMaterial));

	for (int32_t a = -11; a < 11; ++a) {
		for (int32_t b = -11; b < 11; ++b) {
			float chooseMat = RandomFloat();
			glm::vec3 center = glm::vec3(a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat());

			if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8f) {
					// Diffuse
					glm::vec3 albedo = RandomVec3() * RandomVec3();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else if (chooseMat < 0.95f) {
					// Metal
					glm::vec3 albedo = RandomVec3(0.5f, 1.0f);
					float fuzz = RandomFloat(0.0f, 0.5f);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else {
					// Glass
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5f);
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
	world.Add(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
	world.Add(std::make_shared<Sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	CameraInfo camInfo{};

	camInfo.Position = glm::vec3(13.0f, 2.0f, 3.0f);
	camInfo.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);

	camInfo.AspectRatio = 16.0f / 9.0f;
	camInfo.ImageWidth = 1200;
	camInfo.SampleCount = 500;
	camInfo.MaxDepth = 50;
	camInfo.VerticalFOV = 20.0f;

	camInfo.DefocusAngle = 10.0f;
	camInfo.FocusDist = 10.0f;

	Camera cam(camInfo);

	cam.Render(world, path.c_str());
}
