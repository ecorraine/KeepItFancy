#include "BScene.h"

SCENE::Objects SCENE::m_objects;

SCENE::~SCENE()
{
	// サブシーンを削除
	RemoveSubScene();

	// 削除
	Items::iterator it = m_Items.begin();
	while (it != m_Items.end())
	{
		m_objects.erase(*it);
		++it;
	}
	m_Items.clear();

	// 親の参照を削除
	if (m_pParent)
		m_pParent->m_pSubScene = nullptr;
}
void SCENE::_update(float tick)
{
	if (m_pSubScene)
		m_pSubScene->_update(tick);
	Update(tick);
}
void SCENE::_draw()
{
	if (m_pSubScene)
		m_pSubScene->_draw();
	Draw();
}

/// @brief サブシーンの削除
void SCENE::RemoveSubScene()
{
	// 削除するサブシーンが存在するか
	if (!m_pSubScene) return;

	// 階層内のサブシーンを優先して削除
	m_pSubScene->RemoveSubScene();

	// 直下のサブシーンを削除
	m_pSubScene->Release();
	delete m_pSubScene;
	m_pSubScene = nullptr;
}

void SCENE::DestroyObj(const char* name)
{
	m_objects.erase(name);
	m_Items.remove(name);
}