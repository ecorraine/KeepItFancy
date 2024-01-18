#ifndef SRGBA_H
#define SRGBA_H

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Color Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*! \struct sRGBA srgba.h "srgba.h"
 *  \brief Color structure with operators.
 *  \brief 色情報構造体
*/
struct sRGBA {
	float r, g, b, a;

	///--------------------------------------------------
	/// Constructors
	/*! \fn sRGBA()
	 *  \brief default constructor
	 *  \return { 1.0f, 1.0f, 1.0f, 1.0f }
	 */
	constexpr sRGBA() :
		r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}

	constexpr sRGBA(int red, int green, int blue) :
		r(red / 255.0f), g(green / 255.0f), b(blue / 255.0f), a(255 / 255.0f) {}

	/*! \fn sRGBA(int red, int green, int blue, int alpha)
	 *  \brief set color as int values of sRGBA[0 - 255]
	 */
	constexpr sRGBA(int red, int green, int blue, int alpha) :
		r(red / 255.0f), g(green / 255.0f), b(blue / 255.0f), a(alpha / 255.0f) {}

	constexpr sRGBA(float red, float green, float blue) :
		r(red), g(green), b(blue), a(1.0f) {}

	/*! \fn sRGBA(float red, float green, float blue, float alpha)
	 *  \brief set color as float values [0.0f - 1.0f]
	 */
	constexpr sRGBA(float red, float green, float blue, float alpha) :
		r(red), g(green), b(blue), a(alpha) {}

	///--------------------------------------------------
	/// Operators
	/*! \fn sRGBA& operator+ (float value)
	 *  \brief increment operator
	 *  \param value : increment value
	 *  \return sRGBA
	 */
	sRGBA& operator+ (const float value) {
		r += value;
		g += value;
		b += value;
		a += value;
		return *this;
	}
	/*! \fn DirectX::XMFLOAT4 operator+ (float4& value) const
	 *  \brief addition operator
	 *  \param value : addition value
	 *  \return DirectX::XMFLOAT4
	 */
	DirectX::XMFLOAT4 operator+ (const sRGBA& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x += value.r;
		result.y += value.g;
		result.z += value.b;
		result.w += value.a;
		return result;
	}
	DirectX::XMFLOAT4 operator+ (const DirectX::XMFLOAT4& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x += value.x;
		result.y += value.y;
		result.z += value.z;
		result.w += value.w;
		return result;
	}

	/*! \fn sRGBA& operator- (float value)
	 *  \brief decrement operator
	 *  \param value : decrement value
	 *  \return sRGBA
	 */
	sRGBA& operator- (const float value) {
		r -= value;
		g -= value;
		b -= value;
		a -= value;
		return *this;
	}
	/*! \fn DirectX::XMFLOAT4 operator- (float4& value) const
	 *  \brief subtraction operator
	 *  \param value : subtraction value
	 *  \return DirectX::XMFLOAT4
	 */
	DirectX::XMFLOAT4 operator- (const sRGBA& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x -= value.r;
		result.y -= value.g;
		result.z -= value.b;
		result.w -= value.a;
		return result;
	}
	DirectX::XMFLOAT4 operator- (const DirectX::XMFLOAT4& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x -= value.x;
		result.y -= value.y;
		result.z -= value.z;
		result.w -= value.w;
		return result;
	}

	/*! \fn sRGBA& operator* (float value)
	 *  \brief multiply operator
	 *  \param value : multiply value
	 *  \return sRGBA
	 */
	sRGBA& operator* (const float value) {
		r *= value;
		g *= value;
		b *= value;
		a *= value;
		return *this;
	}
	/*! \fn DirectX::XMFLOAT4 operator* (float4& value) const
	 *  \brief multiply operator
	 *  \param value : multiply value
	 *  \return DirectX::XMFLOAT4
	 */
	DirectX::XMFLOAT4 operator* (const sRGBA& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x *= value.r;
		result.y *= value.g;
		result.z *= value.b;
		result.w *= value.a;
		return result;
	}
	DirectX::XMFLOAT4 operator* (const DirectX::XMFLOAT4& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x *= value.x;
		result.y *= value.y;
		result.z *= value.z;
		result.w *= value.w;
		return result;
	}

	/*! \fn sRGBA& operator/ (float value)
	 *  \brief division operator
	 *  \param value : division value
	 *  \return sRGBA
	 */
	sRGBA& operator/ (const float value) {
		r /= value;
		g /= value;
		b /= value;
		a /= value;
		return *this;
	}
	/*! \fn DirectX::XMFLOAT4 operator/ (float4& value) const
	 *  \brief division operator
	 *  \param value : division value
	 *  \return DirectX::XMFLOAT4
	 */
	DirectX::XMFLOAT4 operator/ (const sRGBA& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x /= value.r;
		result.y /= value.g;
		result.z /= value.b;
		result.w /= value.a;
		return result;
	}
	DirectX::XMFLOAT4 operator/ (const DirectX::XMFLOAT4& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x /= value.x;
		result.y /= value.y;
		result.z /= value.z;
		result.w /= value.w;
		return result;
	}

	/*! \fn sRGBA& operator= (const float4& value)
	 *  \brief assignment operator
	 *  \param value : sRGBA to apply
	 *  \return sRGBA
	 */
	sRGBA& operator= (const sRGBA& value) {
		r = value.r;
		g = value.g;
		b = value.b;
		a = value.a;
		return *this;
	}
	//sRGBA& operator= (const DirectX::XMFLOAT4& value) {
	//	r = value.x;
	//	g = value.y;
	//	b = value.z;
	//	a = value.w;
	//	return *this;
	//}

	DirectX::XMFLOAT4 operator= (const sRGBA& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x = value.r;
		result.y = value.g;
		result.z = value.b;
		result.w = value.a;
		return result;
	}
	DirectX::XMFLOAT4 operator= (const DirectX::XMFLOAT4& value) const {
		DirectX::XMFLOAT4 result = { r, g, b, a };
		result.x = value.x;
		result.y = value.y;
		result.z = value.z;
		result.w = value.w;
		return result;
	}

	/*! \fn bool operator== (const float4& other) const
	 *  \brief equality operator
	 *  \param other : sRGBA to be compared
	 *  \return bool
	 */
	bool operator== (const sRGBA& other) const {
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}
	bool operator== (const DirectX::XMFLOAT4& other) const {
		return (r == other.x && g == other.y && b == other.z && a == other.w);
	}

	/*! \fn bool operator!= (const sRGBA& other) const
	 *  \brief inequality operator
	 *  \param other : sRGBA to be compared
	 *  \return bool
	 */
	bool operator!= (const sRGBA& other) const {
		return !(*this == other);
	}
	bool operator!= (const DirectX::XMFLOAT4& other) const {
		return !(*this == other);
	}
};

#endif // !SRGBA_H
