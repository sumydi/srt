#ifndef SRT_RENDERJOB_SIMPLE_H
#define SRT_RENDERJOB_SIMPLE_H

#include "RenderJob.h"

namespace srt
{

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class RenderJobSimple : public RenderJob
	{
	public:
		RenderJobSimple() : RenderJob{ } { };
		virtual ~RenderJobSimple() { }

	private:
		virtual void Execute() final;
	};
}
#endif
