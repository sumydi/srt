#ifndef SRT_JOBSCHEDULER_H
#define SRT_JOBSCHEDULER_H

#include "Base.h"
#include <thread>
#include <semaphore>
#include <mutex>
#include <vector>

namespace srt
{
	// ============================================================================
	//
	//
	//
	// ============================================================================
	class Job
	{
	public:
		Job() { }
		virtual ~Job( ) { }

		virtual void Execute( ) = 0;
	};

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class JobScheduler
	{
	public:
		JobScheduler( const size_t threadCount );
		~JobScheduler();

	private:
		static void ThreadFunc( JobScheduler * js );
	
		using ThreadContainer = std::vector< std::thread * >;
		ThreadContainer	m_threads;
		size_t			m_threadCount;
	
	};

}

#endif