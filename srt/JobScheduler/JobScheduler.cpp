#include "JobScheduler.h"

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void JobScheduler::ThreadFunc( JobScheduler * js )
	{
		
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	JobScheduler::JobScheduler( const size_t threadCount )
	: m_threadCount{ threadCount }
	{
		for( size_t threadIdx = 0; threadIdx < m_threadCount; ++threadIdx )
		{
			m_threads.push_back( new std::thread( ThreadFunc, this ) );
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	JobScheduler::~JobScheduler( )
	{
		for( auto it : m_threads )
		{
			it->join( );
			delete it;
		}
	}
}
