#ifndef SRT_RENDERJOB_PBR_H
#define SRT_RENDERJOB_PBR_H

#include "RenderJob.h"

namespace srt
{
	class Ray;

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
		virtual void	Execute() final;
		Vec3			ComputeColor( const Ray & ray, uint32_t rayIdx );
	};
}
#endif
