#ifndef SRT_VECTOR3_H
#define SRT_VECTOR3_H

#include <math.h>

namespace srt
{

	class Vec3
	{
	public:
		Vec3( ) : m_v{ 0.0f, 0.0f, 0.0f } {}
		explicit Vec3( const float x, const float y, const float z ) : m_v{ x, y, z } {}

		inline Vec3 & operator = ( const Vec3 & other );

		inline Vec3 & operator += ( const float f );
		inline Vec3 & operator -= ( const float f );
		inline Vec3 & operator *= ( const float f );
		inline Vec3 & operator /= ( const float f );

		inline Vec3 & operator += ( const Vec3 & other );
		inline Vec3 & operator -= ( const Vec3 & other );
		inline Vec3 & operator *= ( const Vec3 & other );
		inline Vec3 & operator /= ( const Vec3 & other );

		float X() const { return m_v[ 0 ]; }
		float Y() const { return m_v[ 1 ]; }
		float Z() const { return m_v[ 2 ]; }

		float	m_v[ 3 ];
	};


	inline Vec3 & Vec3::operator = ( const Vec3 & other )
	{
		m_v[ 0 ] = other.m_v[ 0 ];
		m_v[ 1 ] = other.m_v[ 1 ];
		m_v[ 2 ] = other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 & Vec3::operator += ( const float f )
	{
		m_v[ 0 ] += f;
		m_v[ 1 ] += f;
		m_v[ 2 ] += f;

		return *this;
	}

	inline Vec3 & Vec3::operator -= ( const float f )
	{
		m_v[ 0 ] -= f;
		m_v[ 1 ] -= f;
		m_v[ 2 ] -= f;

		return *this;
	}

	inline Vec3 & Vec3::operator *= ( const float f )
	{
		m_v[ 0 ] *= f;
		m_v[ 1 ] *= f;
		m_v[ 2 ] *= f;

		return *this;
	}

	inline Vec3 & Vec3::operator /= ( const float f )
	{
		m_v[ 0 ] /= f;
		m_v[ 1 ] /= f;
		m_v[ 2 ] /= f;

		return *this;
	}

	inline Vec3 & Vec3::operator += ( const Vec3 & other )
	{
		m_v[ 0 ] += other.m_v[ 0 ];
		m_v[ 1 ] += other.m_v[ 1 ];
		m_v[ 2 ] += other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 & Vec3::operator -= ( const Vec3 & other )
	{
		m_v[ 0 ] -= other.m_v[ 0 ];
		m_v[ 1 ] -= other.m_v[ 1 ];
		m_v[ 2 ] -= other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 & Vec3::operator *= ( const Vec3 & other )
	{
		m_v[ 0 ] *= other.m_v[ 0 ];
		m_v[ 1 ] *= other.m_v[ 1 ];
		m_v[ 2 ] *= other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 & Vec3::operator /= ( const Vec3 & other )
	{
		m_v[ 0 ] /= other.m_v[ 0 ];
		m_v[ 1 ] /= other.m_v[ 1 ];
		m_v[ 2 ] /= other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 operator + ( const Vec3 & v, const float f )
	{
		return Vec3( v.m_v[ 0 ] + f, v.m_v[ 1 ] + f, v.m_v[ 2 ] + f );
	}

	inline Vec3 operator - ( const Vec3 & v, const float f )
	{
		return Vec3( v.m_v[ 0 ] - f, v.m_v[ 1 ] - f, v.m_v[ 2 ] - f );
	}

	inline Vec3 operator * ( const Vec3 & v, const float f )
	{
		return Vec3( v.m_v[ 0 ] * f, v.m_v[ 1 ] * f, v.m_v[ 2 ] * f );
	}

	inline Vec3 operator * ( const float f, const Vec3 & v )
	{
		return Vec3( v.m_v[ 0 ] * f, v.m_v[ 1 ] * f, v.m_v[ 2 ] * f );
	}

	inline Vec3 operator / ( const Vec3 & v, const float f )
	{
		return Vec3( v.m_v[ 0 ] / f, v.m_v[ 1 ] / f, v.m_v[ 2 ] / f );
	}

	inline Vec3 operator + ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.m_v[ 0 ] + v2.m_v[ 0 ], v1.m_v[ 1 ] + v2.m_v[ 1 ], v1.m_v[ 2 ] + v2.m_v[ 2 ] );
	}

	inline Vec3 operator - ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.m_v[ 0 ] - v2.m_v[ 0 ], v1.m_v[ 1 ] - v2.m_v[ 1 ], v1.m_v[ 2 ] - v2.m_v[ 2 ] );
	}

	inline Vec3 operator * ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.m_v[ 0 ] * v2.m_v[ 0 ], v1.m_v[ 1 ] * v2.m_v[ 1 ], v1.m_v[ 2 ] * v2.m_v[ 2 ] );
	}

	inline Vec3 operator / ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.m_v[ 0 ] / v2.m_v[ 0 ], v1.m_v[ 1 ] / v2.m_v[ 1 ], v1.m_v[ 2 ] / v2.m_v[ 2 ] );
	}

	inline float Dot( const Vec3 & v1, const Vec3 & v2 )
	{
		return v1.m_v[ 0 ] * v2.m_v[ 0 ] + v1.m_v[ 1 ] * v2.m_v[ 1 ] + v1.m_v[ 2 ] * v2.m_v[ 2 ];
	}

	inline float Length( const Vec3 & v )
	{
		return sqrtf( Dot( v, v ) );
	}

	inline Vec3 Normalize( const Vec3 & v )
	{
		const float rlen = 1.0f / Length( v );
		return Vec3( v.m_v[ 0 ] * rlen, v.m_v[ 1 ] * rlen, v.m_v[ 2 ] * rlen );
	}

	inline Vec3 Cross( const Vec3 & v1, const Vec3 & v2 )
	{
		Vec3 r;
		r.m_v[ 0 ] = v1.m_v[ 1 ] * v2.m_v[ 2 ] - v1.m_v[ 2 ] * v2.m_v[ 1 ]; 
		r.m_v[ 1 ] = v1.m_v[ 2 ] * v2.m_v[ 0 ] - v1.m_v[ 0 ] * v2.m_v[ 2 ];
		r.m_v[ 2 ] = v1.m_v[ 0 ] * v2.m_v[ 1 ] - v1.m_v[ 1 ] * v2.m_v[ 0 ];

		return r;
	}
}

#endif
