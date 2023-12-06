#ifndef ROOTSCENE_H
#define ROOTSCENE_H

#include "BScene.h"

//! \enum Scene List
/*! \brief List of Scenes
 *  \brief ゲームシーンの列挙型
 */
enum class SceneList
{
	TITLE,
	DEBUG,
	STAGE,
	SCENE_MAX
};

///--------------------------------------------------
//! RootScene Class
///--------------------------------------------------
//! \class RootScene RootScene.h "RootScene.h"
/*! \brief The default root scene.
 *  \brief 基底シーン
 */
class RootScene : public SCENE
{
private:
	SceneList		m_eSceneIndex;

	void ChangeScene();

public:
	void Init();
	void Release();
	void Update(float tick);
	void Draw();

private:
	float			m_fTime = 0;
};

#endif // !ROOTSCENE_H
