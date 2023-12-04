#ifndef COMPONENT_H
#define COMPONENT_H

#include <map>
#include <memory>
#include <vector>
#include <list>
#include <string>

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! COMPONENT
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class COMPONENT Component.h "Component.h"
/*! \brief Parent Class for bindable components:
 *  \brief Shaders
 *  \brief Buffers
 *  \brief
 */
class COMPONENT
{
public:
	virtual ~COMPONENT() {}
};

///--------------------------------------------------
//! TComponent Class
///--------------------------------------------------
//! \class TComponent Component.h "Component.h"
/*! \brief Templated Child Class of COMPONENT
 *  \brief
 */
template<class T>
class TComponent : public COMPONENT
{
public:
	std::shared_ptr<T> m_pObj;

	TComponent(std::shared_ptr<T> ptr)
		: m_pObj(ptr)
	{}
	virtual ~TComponent() {}
};

#endif // !COMPONENT_H
