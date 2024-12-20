#pragma once

#include "main.h"
///--------------------------------------------------
//! \def Main Projection Type
/*! \brief define whether to use PERSPECTIVE or ORTHOGRAPHIC projection
 *  \brief 適用する投影方法：透視投影（３Ｄ）か、正投影（２Ｄ）か
 */
#define PERSPECTIVE

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///　CONSTANTS
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
constexpr uint32_t	SCREEN_WIDTH = 1280;							//!< 画面の幅
constexpr uint32_t	SCREEN_HEIGHT = 720;							//!< 画面の高さ
constexpr float		RATIO_WIDTH = 16.0f;							//!< アスペクト比・幅
constexpr float		RATIO_HEIGHT = 9.0f;							//!< アスペクト比・高さ
constexpr float		ASPECT_RATIO = RATIO_WIDTH / RATIO_HEIGHT;		//!< 画面オフセットに使うアスペクト比

static uint64_t		g_Frequency;
static double		g_FrameRateCap = 60.0;
static float		g_fElapsedTime;



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///　MACROS
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ASSET_PATH( path )			"assets/" path
#define SHADER_PATH( path )			"assets/shaders/" path

//! \def ALIGN16
/*! \brief align data by 16 bytes
 *  \brief １６バイトにアラインメントする
 */
#define ALIGN16						_declspec(align(16))

///--------------------------------------------------
//! \def SAFE_RELEASE( a )
/*! \brief null check before release
 *  \brief ptrをリリースする前にnullのチェックを行う
 *  \param a : pointer
 */
#define SAFE_RELEASE( a )			if ( a != nullptr ){ a->Release(); a = nullptr; }

///--------------------------------------------------
//! \def CHECK_HR( expr )
/*! \brief null check before release
 *  \brief ptrをリリースする前にnullのチェックを行う
 *  \param a : pointer
 */
#define CHECK_HR( expr ) \
	do { \
		HRESULT hr_check = (expr); \
		if (FAILED(hr_check)) { \
			MessageBoxA(nullptr, "Failed at " #expr "\n", "ERROR", MB_OK | MB_ICONWARNING); \
		} \
	} while (0)


#define SHOW_ERROR( header, fileName ) \
	do { \
		static char buf[256]; \
		sprintf_s(buf, sizeof(buf), "Failed to load\n %s", fileName); \
		MessageBoxA(nullptr, buf, header, MB_OK | MB_ICONWARNING); \
	} while (0)