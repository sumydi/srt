#ifndef SRT_RENDERJOB_H
#define SRT_RENDERJOB_H

#include "JobScheduler/JobScheduler.h"
#include "Math/Random.h"

namespace srt
{
	class Image;
	class Scene;
	class Camera;
	class Halton;

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class RenderJob : public Job
	{
	public:
		struct Context
		{
			Context() = default;

			const Image *		image { nullptr };
			const Scene *		scene { nullptr };
			const Camera *		camera { nullptr };
			const Halton *		halton { nullptr };

			uint32_t			x { 0 };
			uint32_t			y { 0 };
			uint32_t			width { 0 };
			uint32_t			height { 0 };
			uint32_t			sampleCount { 1 };
			uint32_t			rayCount { 1 };
		};

		RenderJob() : Job{ } { };
		virtual ~RenderJob() { }

		void SetContext( const Context & context ) { m_context = context; }
	
	protected:
		Context			m_context;
		RandomGenerator	m_rndGenerator;

	private:
		virtual void Execute( ) = 0;

	};
}

#endif
