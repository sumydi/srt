#ifndef SRT_INPUTDEVICE_H
#define SRT_INPUTDEVICE_H
#include "Base.h"

namespace srt
{
	// ============================================================================
	//
	// Keyboard & mouse key code.
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

		kF1,
		kF2,
		kF3,
		kF4,
		kF5,
		kF6,
		kF7,
		kF8,
		kF9,
		kF10,

		kShift,
		kControl,

		kMouseLeftButton,
		kMouseMiddleButton,
		kMouseRightButton,


		kUnknow,
		kCount = kUnknow
	};

	// ============================================================================
	//
	// Key state.
	//
	// ============================================================================
	struct KeyState
	{
		union
		{
			uint32_t	raw { 0 };
			struct
			{
				uint32_t	pressed : 1;		// if set to 1, the key is pressed
				uint32_t	justPressed : 1;	// if set to 1, this is the first time the key is pressed, else the key is hold
			};
		};
	};


	// ============================================================================
	//
	// Mouse position.
	//
	// ============================================================================
	struct MousePos
	{
		int		x { 0 };
		int		y { 0 };
	};

}

#endif
