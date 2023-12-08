#ifndef BSCENE_H
#define BSCENE_H

#include "Component.h"
#include "KeyInput.h"

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! SCENE Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class SCENE BScene.h "BScene.h"
/*! \brief More detailed description.
 *  \brief 
 */
class SCENE
{
private:
	using Items		= std::list<std::string>;
	using Objects	= std::map<std::string, std::shared_ptr<COMPONENT>>;

	static Objects	m_objects;

protected:
	SCENE*	m_pParent;
	SCENE*	m_pSubScene;
	Items	m_Items;
	float	m_fTime;

public:
	SCENE() :
		m_pParent(nullptr),
		m_pSubScene(nullptr)
	{}
	virtual ~SCENE();
	// シーン操作関数
	template<class T> T* AddSubScene();
	void RemoveSubScene();

	// オブジェクト操作関数
	template<class T> T* CreateObj(const char* name);
	template<class T> T* GetObj(const char* name);
	void DestroyObj(const char* name);

	void _update(float tick);
	void _draw();

	// 継承シーンの一通りの処理
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float tick) = 0;
	virtual void Draw() = 0;
};

///--------------------------------------------------
template<class T> T* SCENE::AddSubScene()
{
	RemoveSubScene();
	T* pScene = new T;
	m_pSubScene = pScene;
	pScene->m_pParent = this;
	pScene->Init();
	return pScene;
}

///--------------------------------------------------
template<class T> T* SCENE::CreateObj(const char* name)
{
	// デバッグ中のみ、名称ダブりがないかチェック
#ifdef _DEBUG
	Objects::iterator it = m_objects.find(name);
	if (it != m_objects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object!\nオブジェクト作成失敗！\n%s", name);
		MessageBoxA(NULL, buf, "ERROR", MB_OK | MB_ICONWARNING);
		return nullptr;
	}
#endif // _DEBUG

	// オブジェクト生成
	std::shared_ptr<T> ptr = std::make_shared<T>();
	m_objects.insert(std::pair<std::string, std::shared_ptr<COMPONENT>>(name, std::make_shared<TComponent<T>>(ptr)));
	m_Items.push_back(name);
	return ptr.get();
}

///--------------------------------------------------
template<class T> T* SCENE::GetObj(const char* name)
{
	// オブジェクトの探索
	Objects::iterator it = m_objects.find(name);
	if (it == m_objects.end()) return nullptr;

	// 型変換
	std::shared_ptr<TComponent<T>> ptr = std::reinterpret_pointer_cast<TComponent<T>>(it->second);
	return ptr->m_pObj.get();
}

#endif // !BSCENE_H