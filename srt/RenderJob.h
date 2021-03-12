#ifndef SRT_RENDERJOB_H
#define SRT_RENDERJOB_H

#include "JobScheduler/JobScheduler.h"

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
	class RenderJob final : public Job
	{
	public:
		struct Context
		{
			Context() = default;
			Context( const Image * image, const Scene * scene, const Camera * camera )
			: image{image }
			, scene{ scene }
			, camera{ camera }
			{

			}

			const Image *		image;
			const Scene *		scene;
			const Camera *		camera;

			uint32_t			x { 0 };
			uint32_t			y { 0 };
			uint32_t			width { 0 };
			uint32_t			height { 0 };
		};

		RenderJob() : Job{ } { };
		~RenderJob() { }

		void SetContext( const Context & context ) { m_context = context; }
	
	private:
		void Execute( ) final;

		Context		m_context;

	};
}

#endif
