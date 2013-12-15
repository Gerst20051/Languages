/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef __BLOCKALLOC_H__
#define __BLOCKALLOC_H__

#undef new

#include <vector>
#include <wx/file.h>
#include <wx/string.h>
#include <typeinfo>
#include "globals.h"
#include "prep.h"

namespace BlkAllc
{
    void DebugLog(wxString cn, int blockSize, int poolSize, int max_refs, int total_refs, int ref_count);

    const bool enable_global_debug = false;
    const bool verbose = false;
};


template <class T, unsigned int pool_size, const bool debug>
class BlockAllocator
{
    template <class U>
    union LinkedBlock
    {
        LinkedBlock<U> *next;
        char data[sizeof(U)];
    };

    std::vector<LinkedBlock<T>*> allocBlocks;

    LinkedBlock<T> *first;
    int ref_count;
    int max_refs;
    int total_refs;

    void AllocBlockPushBack()
    {
        LinkedBlock<T> *ptr = new LinkedBlock<T>[pool_size];

        allocBlocks.push_back(ptr);

        for(unsigned int i = 0; i < pool_size - 1; ++i)
            ptr[i].next = &ptr[i+1];

        ptr[pool_size - 1].next = 0;

        first = ptr;
    };


    void PushFront(LinkedBlock<T> *p)
    {
        p->next = first;
        first = p;
    };

public:

    BlockAllocator() : first(0), ref_count(0), max_refs(0), total_refs(0)
    {
	#if defined(__GNUC__)
        assert(__builtin_constant_p(debug));
    #endif
	};

    ~BlockAllocator()
    {
        if(debug)
            BlkAllc::DebugLog(cbC2U(typeid(T).name()), allocBlocks.size(), pool_size, max_refs, total_refs, ref_count);
        else if(BlkAllc::enable_global_debug && (BlkAllc::verbose || ref_count != 0))
            BlkAllc::DebugLog(cbC2U(typeid(T).name()), allocBlocks.size(), pool_size, max_refs, total_refs, ref_count);

        for(unsigned int i = 0; i < allocBlocks.size(); ++i)
            delete[] allocBlocks[i];
    };

    inline void* New()
    {
        if(BlkAllc::enable_global_debug || debug)
        {
            ++ref_count;
            ++total_refs;
            max_refs = ref_count > max_refs ? ref_count : max_refs;
        }

        if(first == 0)
            AllocBlockPushBack();

        void *p = first;
        first = first->next;
        return p;
    };

    inline void Delete(void *ptr)
    {
        if(BlkAllc::enable_global_debug || debug)
            --ref_count;

        PushFront((LinkedBlock<T> *) ptr);
    };
};


template <class T, unsigned int pool_size, const bool debug = 0>
class BlockAllocated
{
    static BlockAllocator<T, pool_size, debug> allocator;

public:

    inline void* operator new(size_t size)
    {
        return allocator.New();
    };

    inline void operator delete(void *ptr)
    {
        if(ptr == 0) // C++ standard requires this
            return;
        allocator.Delete(ptr);
    };
};
template<class T, unsigned int pool_size, const bool debug>
BlockAllocator<T, pool_size, debug> BlockAllocated<T, pool_size, debug>::allocator;

#endif
