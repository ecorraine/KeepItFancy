#pragma once

#include "main.h"
///--------------------------------------------------
//! \def 
/*! \brief define whether to use PERSPECTIVE or ORTHOGRAPHIC projection
 *  \brief 適用する投影方法：透視投影（３Ｄ）か、正投影（２Ｄ）か
 */
#define		PERSPECTIVE

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///　MACROS
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ASSET_PATH( path )			"assets/" path
#define SHADER_PATH( path )			"assets/shaders/" path

//! \def ALIGN16
/*! \brief align data by 16 bytes
 *  \brief １６バイトにアラインメントする
 */
#define		ALIGN16					_declspec(align(16))

///--------------------------------------------------
//! \def SAFE_RELEASE( a )
/*! \brief null check before release
 *  \brief ptrをリリースする前にnullのチェックを行う
 *  \param a : pointer
 */
#define		SAFE_RELEASE( a )		if ( a != nullptr ){ a->Release(); a = nullptr; }

#define CHECK_HR(expr) \
	do { \
		HRESULT hr_check = (expr); \
		if (FAILED(hr_check)) { \
			MessageBoxA(nullptr, "Failed at " #expr "\n", "ERROR", MB_OK | MB_ICONERROR); \
		} \
	} while (0)

#define STRINGIFY(expr) \
	return #expr


///--------------------------------------------------
///　CONSTANTS
///--------------------------------------------------
constexpr uint32_t	SCREEN_WIDTH = 1280;							//!< 画面の幅
constexpr uint32_t	SCREEN_HEIGHT = 720;							//!< 画面の高さ
constexpr float_t	RATIO_WIDTH = 16.0f;							//!< 
constexpr float_t	RATIO_HEIGHT = 9.0f;							//!< 
constexpr float_t	ASPECT_RATIO = RATIO_WIDTH / RATIO_HEIGHT;		//!< 画面オフセットに使うアスペクト比

static bool g_bTesselate = false;

static uint64_t	g_DeltaTime;
static float g_Tick;
