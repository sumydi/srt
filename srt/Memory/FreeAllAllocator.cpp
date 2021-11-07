#include "FreeAllAllocator.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FreeAllAllocator::FreeAllAllocator()
	{
		m_currentPage = CreatePage( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FreeAllAllocator::~FreeAllAllocator()
	{
		for( auto page : m_pages )
		{
			free( page->buffer );
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FreeAllAllocator::Page * FreeAllAllocator::CreatePage()
	{
		Page * page = new Page;
		page->buffer = malloc( kPageSize );
		page->used = 0;
		m_pages.push_back( page );
		return page;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void * FreeAllAllocator::Allocate( size_t size )
	{
		const size_t remainingSize = kPageSize - m_currentPage->used;
		if( size > remainingSize )
		{
			m_currentPage = CreatePage( );
		}
		void * mem = (void *)( (uint8_t *)(m_currentPage->buffer) + m_currentPage->used );
		m_currentPage->used += size;
		return mem;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void FreeAllAllocator::FreeAll( )
	{
		for( auto page : m_pages )
		{
			page->used = 0;
		}
	}
}
