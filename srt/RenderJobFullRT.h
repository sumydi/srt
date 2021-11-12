#ifndef SRT_RENDERJOB_FULLRT_H
#define SRT_RENDERJOB_FULLRT_H

#include "RenderJob.h"
#include "Math/Random.h"

namespace srt
{

class Ray;

// ============================================================================
//
//
//
// ============================================================================
class RenderJobFullRT : public RenderJob
{
public:
	RenderJobFullRT() : RenderJob{ } { };
	virtual ~RenderJobFullRT() { }

private:
	virtual void	Execute() final;
	Vec3			ComputeColor( const Ray & ray, uint32_t rayIdx );

	RandomGenerator	m_rndGenerator;
};
}
#endif
