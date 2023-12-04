#ifndef SRGBA_H
#define SRGBA_H

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Color Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*! \struct sRGBA srgba.h "srgba.h"
 *  \brief Color structure with operators.
*/
struct sRGBA {
	float r;
	float g;
	float b;
	float a;

	/*! \fn sRGBA()
	 *  \brief default constructor
	 *  \return { 1.0f, 1.0f, 1.0f, 1.0f }
	 */
	sRGBA() :
		r(1.0f),
		g(1.0f),
		b(1.0f),
		a(1.0f)
	{}

	sRGBA(int red, int green, int blue) :
		r(red / 255.0f),
		g(green / 255.0f),
		b(blue / 255.0f),
		a(255 / 255.0f)
	{}
	/*! \fn sRGBA(int red, int green, int blue, int alpha)
	 *  \brief set color as int values of sRGBA[0 - 255]
	 */
	sRGBA(int red, int green, int blue, int alpha) :
		r(red / 255.0f),
		g(green / 255.0f),
		b(blue / 255.0f),
		a(alpha / 255.0f)
	{}

	sRGBA(float red, float green, float blue) :
		r(red),
		g(green),
		b(blue),
		a(1.0f)
	{}
	/*! \fn sRGBA(float red, float green, float blue, float alpha)
	 *  \brief set color as float values [0.0f - 1.0f]
	 */
	sRGBA(float red, float green, float blue, float alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)
	{}

	/*! \fn sRGBA& operator+= (float value)
	 *  \brief increment operator
	 *  \param value	: increment value
	 *  \return sRGBA
	 */
	sRGBA& operator+ (const float value) {
		r += value;
		g += value;
		b += value;
		a += value;
		return *this;
	}
	/*! \fn sRGBA& operator-= (float value)
	 *  \brief decrement operator
	 *  \param value	: decrement value
	 *  \return sRGBA
	 */
	sRGBA& operator- (const float value) {
		r -= value;
		g -= value;
		b -= value;
		a -= value;
		return *this;
	}
};

#endif // !SRGBA_H
