#ifndef SCTITLE_H
#define SCTITLE_H

#include "BScene.h"

class ScTitle : public SCENE
{
public:
	void Init();
	void Release();
	void Update(float tick);
	void Draw();
};

#endif // !SCTITLE_H
