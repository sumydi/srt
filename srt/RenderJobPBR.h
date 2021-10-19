#ifndef SRT_RENDERJOB_PBR_H
#define SRT_RENDERJOB_PBR_H

#include "RenderJob.h"

namespace srt
{
	class Image;
	class Scene;
	class Camera;

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class RenderJobPBR : public RenderJob
	{
	public:
		RenderJobPBR() : RenderJob{ } { };
		virtual ~RenderJobPBR() { }

	private:
		virtual void Execute() final;
	};
}
#endif
