#ifndef SRT_VECTOR4_H
#define SRT_VECTOR4_H

#include <math.h>

namespace srt
{

	class Vec4
	{
	public:
		Vec4( ) : m_v{ 0.0f, 0.0f, 0.0f, 0.0f } {}
		explicit Vec4( const float x, const float y, const float z, const float w  ) : m_v{ x, y, z, w } {}

		inline Vec4 & operator = ( const Vec4 & other );

		inline Vec4 & operator += ( const float f );
		inline Vec4 & operator -= ( const float f );
		inline Vec4 & operator *= ( const float f );
		inline Vec4 & operator /= ( const float f );

		inline Vec4 & operator += ( const Vec4 & other );
		inline Vec4 & operator -= ( const Vec4 & other );
		inline Vec4 & operator *= ( const Vec4 & other );
		inline Vec4 & operator /= ( const Vec4 & other );

		float	m_v[ 4 ];
	};


	inline Vec4 & Vec4::operator = ( const Vec4 & other )
	{
		m_v[ 0 ] = other.m_v[ 0 ];
		m_v[ 1 ] = other.m_v[ 1 ];
		m_v[ 2 ] = other.m_v[ 2 ];
		m_v[ 3 ] = other.m_v[ 3 ];

		return *this;
	}

	inline Vec4 & Vec4::operator += ( const float f )
	{
		m_v[ 0 ] += f;
		m_v[ 1 ] += f;
		m_v[ 2 ] += f;
		m_v[ 3 ] += f;

		return *this;
	}

	inline Vec4 & Vec4::operator -= ( const float f )
	{
		m_v[ 0 ] -= f;
		m_v[ 1 ] -= f;
		m_v[ 2 ] -= f;
		m_v[ 3 ] -= f;

		return *this;
	}

	inline Vec4 & Vec4::operator *= ( const float f )
	{
		m_v[ 0 ] *= f;
		m_v[ 1 ] *= f;
		m_v[ 2 ] *= f;
		m_v[ 3 ] *= f;

		return *this;
	}

	inline Vec4 & Vec4::operator /= ( const float f )
	{
		m_v[ 0 ] /= f;
		m_v[ 1 ] /= f;
		m_v[ 2 ] /= f;
		m_v[ 3 ] /= f;

		return *this;
	}

	inline Vec4 & Vec4::operator += ( const Vec4 & other )
	{
		m_v[ 0 ] += other.m_v[ 0 ];
		m_v[ 1 ] += other.m_v[ 1 ];
		m_v[ 2 ] += other.m_v[ 2 ];
		m_v[ 3 ] += other.m_v[ 3 ];

		return *this;
	}

	inline Vec4 & Vec4::operator -= ( const Vec4 & other )
	{
		m_v[ 0 ] -= other.m_v[ 0 ];
		m_v[ 1 ] -= other.m_v[ 1 ];
		m_v[ 2 ] -= other.m_v[ 2 ];
		m_v[ 3 ] -= other.m_v[ 3 ];

		return *this;
	}

	inline Vec4 & Vec4::operator *= ( const Vec4 & other )
	{
		m_v[ 0 ] *= other.m_v[ 0 ];
		m_v[ 1 ] *= other.m_v[ 1 ];
		m_v[ 2 ] *= other.m_v[ 2 ];
		m_v[ 3 ] *= other.m_v[ 3 ];

		return *this;
	}

	inline Vec4 & Vec4::operator /= ( const Vec4 & other )
	{
		m_v[ 0 ] /= other.m_v[ 0 ];
		m_v[ 1 ] /= other.m_v[ 1 ];
		m_v[ 2 ] /= other.m_v[ 2 ];
		m_v[ 3 ] /= other.m_v[ 3 ];

		return *this;
	}

	inline Vec4 operator + ( const Vec4 & v, const float f )
	{
		return Vec4( v.m_v[ 0 ] + f, v.m_v[ 1 ] + f, v.m_v[ 2 ] + f, v.m_v[ 3 ] + f );
	}

	inline Vec4 operator - ( const Vec4 & v, const float f )
	{
		return Vec4( v.m_v[ 0 ] - f, v.m_v[ 1 ] - f, v.m_v[ 2 ] - f, v.m_v[ 3 ] - f );
	}

	inline Vec4 operator * ( const Vec4 & v, const float f )
	{
		return Vec4( v.m_v[ 0 ] * f, v.m_v[ 1 ] * f, v.m_v[ 2 ] * f, v.m_v[ 3 ] * f );
	}

	inline Vec4 operator / ( const Vec4 & v, const float f )
	{
		return Vec4( v.m_v[ 0 ] / f, v.m_v[ 1 ] / f, v.m_v[ 2 ] / f, v.m_v[ 3 ] / f );
	}

	inline Vec4 operator + ( const Vec4 & v1, const Vec4 & v2 )
	{
		return Vec4( v1.m_v[ 0 ] + v2.m_v[ 0 ], v1.m_v[ 1 ] + v2.m_v[ 1 ], v1.m_v[ 2 ] + v2.m_v[ 2 ], v1.m_v[ 3 ] + v2.m_v[ 3 ] );
	}

	inline Vec4 operator - ( const Vec4 & v1, const Vec4 & v2 )
	{
		return Vec4( v1.m_v[ 0 ] - v2.m_v[ 0 ], v1.m_v[ 1 ] - v2.m_v[ 1 ], v1.m_v[ 2 ] - v2.m_v[ 2 ], v1.m_v[ 3 ] - v2.m_v[ 3 ] );
	}

	inline Vec4 operator * ( const Vec4 & v1, const Vec4 & v2 )
	{
		return Vec4( v1.m_v[ 0 ] * v2.m_v[ 0 ], v1.m_v[ 1 ] * v2.m_v[ 1 ], v1.m_v[ 2 ] * v2.m_v[ 2 ], v1.m_v[ 3 ] * v2.m_v[ 3 ] );
	}

	inline Vec4 operator / ( const Vec4 & v1, const Vec4 & v2 )
	{
		return Vec4( v1.m_v[ 0 ] / v2.m_v[ 0 ], v1.m_v[ 1 ] / v2.m_v[ 1 ], v1.m_v[ 2 ] / v2.m_v[ 2 ], v1.m_v[ 3 ] / v2.m_v[ 3 ] );
	}
}

#endif
