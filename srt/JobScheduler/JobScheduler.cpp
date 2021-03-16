#include "JobScheduler.h"
#include <assert.h>

namespace srt
{
	// ============================================================================
	//
	// ============================================================================

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Semaphore::Acquire( )
	{
		std::unique_lock < std::mutex > lock( m_mutex );
		while( m_count == 0)
		{
			m_cv.wait( lock );
		}
		m_count--;	
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Semaphore::Release( )
	{
		std::unique_lock < std::mutex > lock( m_mutex );
		m_count++;
		m_cv.notify_one();
	}

	// ============================================================================
	//
	// ============================================================================

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::ThreadFunc( JobScheduler * js, size_t threadIdx )
	{
		while( js->m_exit==false )
		{
			// The semaphore is used to tell us that there's something to do
			js->m_sem.Acquire();

			// Process job if any
			Job * job = js->PopJob( );
			if( job )
			{
				job->Execute( );
				js->SignalJobDone( threadIdx );
			}
		}

		int goodBye = 0;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	JobScheduler::JobScheduler( const size_t threadCount )
	{
		for( size_t threadIdx = 0; threadIdx < threadCount; ++threadIdx )
		{
			m_threads.push_back( new std::thread( ThreadFunc, this, threadIdx ) );
		}

	#if defined( SRT_JOBSCHEDULER_STATS )
		m_threadStats.resize( threadCount );
	#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	JobScheduler::~JobScheduler( )
	{
		m_exit = true;

		for( auto it : m_threads )
		{
			m_sem.Release();
		}

		for( auto it : m_threads )
		{
			it->join( );
			delete it;
		}

	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::WaitForJobs( )
	{
		while( m_jobsToExecuteCount.load() > 0 )
		{
		
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::PushJob( Job * job )
	{
		{
			std::unique_lock < std::mutex > lock( m_jobsMutex );
			m_jobs.push_back( job );
		}
		m_jobsToExecuteCount.fetch_add( 1 );

		// Signal scheduler's threads that there is a job to process
		m_sem.Release();
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Job * JobScheduler::PopJob( )
	{
		Job * job = nullptr;

		std::unique_lock < std::mutex > lock( m_jobsMutex );
		if( !m_jobs.empty() )
		{
			job = m_jobs.back();
			m_jobs.pop_back();
		}
		return job;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::SignalJobDone( size_t threadIdx )
	{
		int32_t jobCount = m_jobsToExecuteCount.fetch_sub( 1 );
		assert( jobCount > 0 );

	#if defined( SRT_JOBSCHEDULER_STATS )
		m_threadStats[ threadIdx ].jobProcessed++;
	#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::ResetThreadStat( )
	{
	#if defined( SRT_JOBSCHEDULER_STATS )
		for( auto & threadStat : m_threadStats )
		{
			threadStat.jobProcessed = 0;
		}
	#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	const JobScheduler::ThreadStat & JobScheduler::GetThreadStat( size_t threadIdx ) const
	{
	#if defined( SRT_JOBSCHEDULER_STATS )
		assert( threadIdx < m_threadStats.size( ) );
		return m_threadStats[ threadIdx ];
	#else
		static ThreadStat dummyThreadStat;
		return dummyThreadStat;
	#endif
	}
}
