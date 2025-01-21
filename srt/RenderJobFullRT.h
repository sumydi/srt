#ifndef SRT_RENDERJOB_PATHTRACING_H
#define SRT_RENDERJOB_PATHTRACING_H

#include "RenderJob.h"

namespace srt
{

class Ray;
struct SceneTraceResult;

// ============================================================================
//
//
//
// ============================================================================
class RenderJobPathTracing : public RenderJob
{
public:
	RenderJobPathTracing() : RenderJob{ } { };
	virtual ~RenderJobPathTracing() { }

private:
	virtual void	Execute() final;
	Vec3			ComputeColor( const Ray & ray, uint32_t rayIdx );
	bool			Scatter( const Ray & ray, const SceneTraceResult & traceResult, Vec3 & scattered, Vec3 & attenuation );
};
}
#endif
