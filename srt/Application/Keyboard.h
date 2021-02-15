#ifndef SRT_KEYBOARD_H
#define SRT_KEYBOARD_H
#include "Base.h"

namespace srt
{
	// ============================================================================
	//
	// Keyboard key code.
	//
	// ============================================================================
	enum class KeyCode : uint32_t
	{
		kEscape = 0,

		kPageUp,
		kPageDown,
		kEnd,
		kHome,

		kLeft,
		kUp,
		kRight,
		kDown,

		kSpace,

		kAdd,
		kSubtract,
		kMultiply,
		kDivide,

		k0,
		k1,
		k2,
		k3,
		k4,
		k5,
		k6,
		k7,
		k8,
		k9,

		kA,
		KB,
		kC,
		kD,
		kE,
		kF,
		kG,
		kH,
		kI,
		kJ,
		kK,
		kL,
		kM,
		kN,
		kO,
		kP,
		kQ,
		kR,
		kS,
		kT,
		kU,
		kV,
		kW,
		kX,
		kY,
		kZ,

		kUnknow

	};

}

#endif
