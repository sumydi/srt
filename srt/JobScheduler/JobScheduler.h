#ifndef SRT_JOBSCHEDULER_H
#define SRT_JOBSCHEDULER_H

#include "Base.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace srt
{
	#define SRT_JOBSCHEDULER_STATS

	// ============================================================================
	//
	// Semaphore implementation. C++20 where are you?
	//
	// ============================================================================
	class Semaphore
	{
	public:
		Semaphore( int32_t count ) : m_count { count } { }

		void Acquire( );
		void Release( ); 

	private:
		std::mutex					m_mutex;
		std::condition_variable		m_cv;
		int32_t						m_count;
	};
	
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

		void	PushJob( Job * job );
		void	WaitForJobs( );

		struct ThreadStat
		{
			uint32_t	jobProcessed { 0 };
		};

		size_t				GetThreadCount( ) const { return m_threads.size(); }
		void				ResetThreadStat( );
		const ThreadStat &	GetThreadStat( size_t threadIdx ) const;

	private:
		static void ThreadFunc( JobScheduler * js, size_t threadIdx );
		Job *		PopJob( );
		void		SignalJobDone( size_t threadIdx );

		// JobScheduler threads	
		using ThreadContainer = std::vector< std::thread * >;
		ThreadContainer				m_threads;
		Semaphore					m_sem{ 0 };					// Used to tell to job threads there's something to do

#if defined( SRT_JOBSCHEDULER_STATS )
		using ThreadStatsContainer = std::vector< ThreadStat >;
		ThreadStatsContainer		m_threadStats;
#endif

		// Jobs list
		using JobContainer = std::vector< Job * >;
		JobContainer				m_jobs;
		std::mutex					m_jobsMutex;
		std::atomic< int32_t >		m_jobsToExecuteCount { 0 };	// Number of Jobs to be executed (pending or currently executing)

		volatile bool	m_exit { false };
	
	};

}

#endif