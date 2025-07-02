#include "ToneMapping.h"

namespace srt
{
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Vec3 ACESFilm( const Vec3 & v )
    {
        constexpr float a = 2.51f;
        constexpr float b = 0.03f;
        constexpr float c = 2.43f;
        constexpr float d = 0.59f;
        constexpr float e = 0.14f;
        return Clamp( ( v * ( a * v + b) ) / ( v * ( c * v + d ) + e ), 0.0f, 1.0f );
    }
}
