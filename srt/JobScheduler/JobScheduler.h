#ifndef SRT_JOBSCHEDULER_H
#define SRT_JOBSCHEDULER_H

#include "Base.h"
#include <thread>
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

		void PushJob();

	private:
		static void ThreadFunc( JobScheduler * js );
	
		using ThreadContainer = std::vector< std::thread * >;
		ThreadContainer	m_threads;
		size_t			m_threadCount;

		Semaphore		m_sem{ 0 };


		volatile bool	m_exit { false };

	
	};

}

#endif