#ifndef SRT_VECTOR2_H
#define SRT_VECTOR2_H

#include <math.h>
#include "Math/MathUtils.h"

namespace srt
{

	class Vec2
	{
	public:
		Vec2( ) : m_v{ 0.0f, 0.0f } {}
		Vec2( const Vec2 & other ) : m_v{ other.X(), other.Y() } { }
		Vec2( const float v ) : m_v{ v, v } { }
		Vec2( const float x, const float y ) : m_v{ x, y } { }

		inline Vec2 & operator = ( const Vec2 & other );

		inline Vec2 operator - () const;
		inline Vec2 & operator += ( const float f );
		inline Vec2 & operator -= ( const float f );
		inline Vec2 & operator *= ( const float f );
		inline Vec2 & operator /= ( const float f );

		inline Vec2 & operator += ( const Vec2 & other );
		inline Vec2 & operator -= ( const Vec2 & other );
		inline Vec2 & operator *= ( const Vec2 & other );
		inline Vec2 & operator /= ( const Vec2 & other );

		float X() const { return m_v[ 0 ]; }
		float Y() const { return m_v[ 1 ]; }

	private:
		float	m_v[ 2 ];
	};


	inline Vec2 & Vec2::operator = ( const Vec2 & other )
	{
		m_v[ 0 ] = other.m_v[ 0 ];
		m_v[ 1 ] = other.m_v[ 1 ];

		return *this;
	}

	inline Vec2 Vec2::operator - () const
	{
		return Vec2( -m_v[ 0 ], -m_v[ 1 ] );
	}

	inline Vec2 & Vec2::operator += ( const float f )
	{
		m_v[ 0 ] += f;
		m_v[ 1 ] += f;

		return *this;
	}

	inline Vec2 & Vec2::operator -= ( const float f )
	{
		m_v[ 0 ] -= f;
		m_v[ 1 ] -= f;

		return *this;
	}

	inline Vec2 & Vec2::operator *= ( const float f )
	{
		m_v[ 0 ] *= f;
		m_v[ 1 ] *= f;

		return *this;
	}

	inline Vec2 & Vec2::operator /= ( const float f )
	{
		m_v[ 0 ] /= f;
		m_v[ 1 ] /= f;

		return *this;
	}

	inline Vec2 & Vec2::operator += ( const Vec2 & other )
	{
		m_v[ 0 ] += other.m_v[ 0 ];
		m_v[ 1 ] += other.m_v[ 1 ];

		return *this;
	}

	inline Vec2 & Vec2::operator -= ( const Vec2 & other )
	{
		m_v[ 0 ] -= other.m_v[ 0 ];
		m_v[ 1 ] -= other.m_v[ 1 ];

		return *this;
	}

	inline Vec2 & Vec2::operator *= ( const Vec2 & other )
	{
		m_v[ 0 ] *= other.m_v[ 0 ];
		m_v[ 1 ] *= other.m_v[ 1 ];

		return *this;
	}

	inline Vec2 & Vec2::operator /= ( const Vec2 & other )
	{
		m_v[ 0 ] /= other.m_v[ 0 ];
		m_v[ 1 ] /= other.m_v[ 1 ];

		return *this;
	}

	inline Vec2 operator + ( const Vec2 & v, const float f )
	{
		return Vec2( v.X() + f, v.Y() + f );
	}

	inline Vec2 operator + ( const float f, const Vec2 & v )
	{
		return Vec2( v.X() + f, v.Y() + f );
	}

	inline Vec2 operator - ( const Vec2 & v, const float f )
	{
		return Vec2( v.X() - f, v.Y() - f );
	}

	inline Vec2 operator - ( const float f, const Vec2 & v )
	{
		return Vec2( f - v.X(), f - v.Y() );
	}

	inline Vec2 operator * ( const Vec2 & v, const float f )
	{
		return Vec2( v.X() * f, v.Y() * f );
	}

	inline Vec2 operator * ( const float f, const Vec2 & v )
	{
		return Vec2( v.X() * f, v.Y() * f );
	}

	inline Vec2 operator / ( const Vec2 & v, const float f )
	{
		return Vec2( v.X() / f, v.Y() / f );
	}

	inline Vec2 operator + ( const Vec2 & v1, const Vec2 & v2 )
	{
		return Vec2( v1.X() + v2.X(), v1.Y() + v2.Y() );
	}

	inline Vec2 operator - ( const Vec2 & v1, const Vec2 & v2 )
	{
		return Vec2( v1.X() - v2.X(), v1.Y() - v2.Y() );
	}

	inline Vec2 operator * ( const Vec2 & v1, const Vec2 & v2 )
	{
		return Vec2( v1.X() * v2.X(), v1.Y() * v2.Y() );
	}

	inline Vec2 operator / ( const Vec2 & v1, const Vec2 & v2 )
	{
		return Vec2( v1.X() / v2.X(), v1.Y() / v2.Y() );
	}

	inline float Dot( const Vec2 & v1, const Vec2 & v2 )
	{
		return v1.X() * v2.X() + v1.Y() * v2.Y();
	}

	inline float Length( const Vec2 & v )
	{
		return sqrtf( Dot( v, v ) );
	}

	inline Vec2 Normalize( const Vec2 & v )
	{
		const float rlen = 1.0f / Length( v );
		return Vec2( v.X() * rlen, v.Y() * rlen );
	}

	inline Vec2 Clamp( const Vec2 & v, float min, float max )
	{
		return Vec2( Vec2{ Clamp( v.X(), min, max ), Clamp( v.Y(), min, max ) } );
	}

	inline Vec2 Lerp( const Vec2 & v1, const Vec2 & v2, float t )
	{
		return v1 + t * ( v2 - v1 );
	}
}

#endif
