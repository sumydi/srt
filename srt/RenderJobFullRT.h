#ifndef SRT_RENDERJOB_FULLRT_H
#define SRT_RENDERJOB_FULLRT_H

#include "RenderJob.h"

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
};
}
#endif
