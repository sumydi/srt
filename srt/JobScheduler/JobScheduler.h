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

	private:
		static void ThreadFunc( JobScheduler * js );
		Job *		PopJob( );
		void		SignalJobDone( );

		// JobScheduler threads	
		using ThreadContainer = std::vector< std::thread * >;
		ThreadContainer				m_threads;
		size_t						m_threadCount;
		Semaphore					m_sem{ 0 };			// used to tell threads thre's something to do

		// Jobs list
		using JobContainer = std::vector< Job * >;
		JobContainer				m_jobs;
		std::mutex					m_jobsMutex;
		std::atomic< int32_t >		m_jobsToExecuteCount { 0 };	// Number of Jobs to be executed (pending or currently executing)

		volatile bool	m_exit { false };
	
	};

}

#endif