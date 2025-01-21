#ifndef SRT_FREEALLALLOCATOR_H
#define SRT_FREEALLALLOCATOR_H

#include "Base.h"
#include <vector>

namespace srt
{
	// ============================================================================
	// A free all allocator is a simple & efficient allocator that allocates memory 
	// for objects of any size. The allocated memory will be freed by calling 
	// the FreeAll() function. 
	//
	// Pros:
	//		- efficient and quick allocation
	//		- no memory fragmentation
	// 
	// Cons:
	//		- the dtor of objects are not called when freeing memory
	//
	//
	// ============================================================================
	class FreeAllAllocator
	{
	public:
		FreeAllAllocator( );
		~FreeAllAllocator( );

		template < typename T >
		T * Allocate( )
		{
			void * data = Allocate( sizeof( T ) );
			return new ( data ) T();
		}

		void	FreeAll();

	private:
		static constexpr size_t kPageSize = 1 * 1024 * 1024;

		struct Page
		{
			void *	buffer;
			size_t	used;
		};

		using PageList = std::vector< Page * >;

		PageList	m_pages;
		size_t 		m_currentPage { 0 };

		size_t FindPage( size_t allocSize );
		void * Allocate( size_t size );
		size_t CreatePage( ); 

	};
}

#endif

