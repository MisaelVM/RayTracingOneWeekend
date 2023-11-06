#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> Objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object);

	void Clear();
	void Add(std::shared_ptr<Hittable> object);
	virtual bool Hit(const Ray& r, Interval rayInterval, HitRecord& record) const override;
};
