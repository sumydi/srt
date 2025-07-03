#include "FreeAllAllocator.h"
#include <assert.h>

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
	size_t FreeAllAllocator::CreatePage()
	{
		Page * page = new Page;
		page->buffer = malloc( kPageSize );
		page->used = 0;
		m_pages.push_back( page );
		return m_pages.size() - 1;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	size_t FreeAllAllocator::FindPage( size_t allocSize )
	{
		const size_t pageCount = m_pages.size();
		size_t pageIdx = m_currentPage;

		while( pageIdx < pageCount )
		{
			const Page * page = m_pages[ pageIdx ];
			if( allocSize <= (kPageSize - page->used) )
			{
				break;
			}
			++pageIdx;
		}

		return pageIdx;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void * FreeAllAllocator::Allocate( size_t size )
	{
		m_currentPage = FindPage( size );
		if( m_currentPage==m_pages.size() )
		{
			m_currentPage = CreatePage();
		}
		Page * page = m_pages[ m_currentPage ];
		void * mem = (void *)( (uint8_t *)(page->buffer) + page->used );
		page->used += size;
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
		m_currentPage = 0;
	}
}
