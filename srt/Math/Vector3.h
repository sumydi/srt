#ifndef SRT_VECTOR3_H
#define SRT_VECTOR3_H

#include <math.h>
#include "Math/MathUtils.h"

namespace srt
{

	class Vec3
	{
	public:
		Vec3( ) : m_v{ 0.0f, 0.0f, 0.0f } {}
		Vec3( const Vec3 & other ) : m_v{ other.X(), other.Y(), other.Z() } { }
		Vec3( const float v ) : m_v{ v, v, v } { }
		Vec3( const float x, const float y, const float z ) : m_v{ x, y, z } { }

		inline Vec3 & operator = ( const Vec3 & other );

		inline Vec3 operator - () const;
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

	private:
		float	m_v[ 3 ];
	};


	inline Vec3 & Vec3::operator = ( const Vec3 & other )
	{
		m_v[ 0 ] = other.m_v[ 0 ];
		m_v[ 1 ] = other.m_v[ 1 ];
		m_v[ 2 ] = other.m_v[ 2 ];

		return *this;
	}

	inline Vec3 Vec3::operator - () const
	{
		return Vec3( -m_v[ 0 ], -m_v[ 1 ], -m_v[ 2 ] );
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
		return Vec3( v.X() + f, v.Y() + f, v.Z() + f );
	}

	inline Vec3 operator + ( const float f, const Vec3 & v )
	{
		return Vec3( v.X() + f, v.Y() + f, v.Z() + f );
	}

	inline Vec3 operator - ( const Vec3 & v, const float f )
	{
		return Vec3( v.X() - f, v.Y() - f, v.Z() - f );
	}

	inline Vec3 operator - ( const float f, const Vec3 & v )
	{
		return Vec3( f - v.X(), f - v.Y(), f - v.Z() );
	}

	inline Vec3 operator * ( const Vec3 & v, const float f )
	{
		return Vec3( v.X() * f, v.Y() * f, v.Z() * f );
	}

	inline Vec3 operator * ( const float f, const Vec3 & v )
	{
		return Vec3( v.X() * f, v.Y() * f, v.Z() * f );
	}

	inline Vec3 operator / ( const Vec3 & v, const float f )
	{
		return Vec3( v.X() / f, v.Y() / f, v.Z() / f );
	}

	inline Vec3 operator + ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.X() + v2.X(), v1.Y() + v2.Y(), v1.Z() + v2.Z() );
	}

	inline Vec3 operator - ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.X() - v2.X(), v1.Y() - v2.Y(), v1.Z() - v2.Z() );
	}

	inline Vec3 operator * ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.X() * v2.X(), v1.Y() * v2.Y(), v1.Z() * v2.Z() );
	}

	inline Vec3 operator / ( const Vec3 & v1, const Vec3 & v2 )
	{
		return Vec3( v1.X() / v2.X(), v1.Y() / v2.Y(), v1.Z() / v2.Z() );
	}

	inline float Dot( const Vec3 & v1, const Vec3 & v2 )
	{
		return v1.X() * v2.X() + v1.Y() * v2.Y() + v1.Z() * v2.Z();
	}

	inline float SquaredLength(const Vec3& v)
	{
		return Dot( v, v );
	}

	inline float Length( const Vec3 & v )
	{
		return sqrtf( Dot( v, v ) );
	}

	inline Vec3 Normalize( const Vec3 & v )
	{
		const float rlen = 1.0f / Length( v );
		return Vec3( v.X() * rlen, v.Y() * rlen, v.Z() * rlen );
	}

	inline Vec3 Cross( const Vec3 & v1, const Vec3 & v2 )
	{
		const float x = v1.Y() * v2.Z() - v1.Z() * v2.Y();
		const float y = v1.Z() * v2.X() - v1.X() * v2.Z();
		const float z = v1.X() * v2.Y() - v1.Y() * v2.X();

		return Vec3( x, y ,z );
	}

	inline Vec3 Clamp( const Vec3 & v, float min, float max )
	{
		return Vec3( Vec3{ Clamp( v.X(), min, max ), Clamp( v.Y(), min, max ), Clamp( v.Z(), min, max ) } );
	}

	inline Vec3 Lerp( const Vec3 & v1, const Vec3 & v2, float t )
	{
		return v1 + t * ( v2 - v1 );
	}

	// ------------------------------------------------------------------------
	//
	//	Computes a reflection vector of v depending on n.
	//	Note: n must be normalized.
	//
	//   v   n   r
	//   \   ^   ^
	//    \  |  /
	//     \ | /
	//		v|/	
	//
	//
	// ------------------------------------------------------------------------
	inline Vec3 Reflect( const Vec3 & v, const Vec3 & n )
	{
		return v - 2.0f * Dot( v, n ) * n;
	}
}

#endif
