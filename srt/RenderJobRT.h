#ifndef SRT_RENDERJOB_RAYTRACING_H
#define SRT_RENDERJOB_RAYTRACING_H

#include "RenderJob.h"

namespace srt
{
	class Ray;

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class RenderJobRayTracing : public RenderJob
	{
	public:
		RenderJobRayTracing() : RenderJob{ } { };
		virtual ~RenderJobRayTracing() { }

	private:
		virtual void	Execute() final;
		Vec3			ComputeColor( const Ray & ray, uint32_t rayIdx );
	};
}
#endif
