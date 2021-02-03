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
		explicit Vec3( const float x, const float y, const float z ) : m_v{ x, y, z } {}

		inline Vec3 & operator = ( const Vec3 & other );

		inline Vec3 operator - ();
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

	inline Vec3 Vec3::operator - ()
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

	inline Vec3 operator - ( const Vec3 & v, const float f )
	{
		return Vec3( v.X() - f, v.Y() - f, v.Z() - f );
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
}

#endif
