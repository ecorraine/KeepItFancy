#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "MPrimitive3D.h"

class FreeCamera final : public SSphere
{
public:
	FreeCamera() {}
	~FreeCamera() {}

	void Create(float radius = 0.1f, int segments = 32);
	void Draw();
	void Update(float tick);
};

#endif // !FREECAMERA_H
