//
// Created by 20550 on 2023/3/25.
//

#ifndef MINISTL_ALLOC_HH
#define MINISTL_ALLOC_HH

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <cstring>

namespace stl
{
    template<typename T, typename Alloc>
    class simple_alloc
    {
    public:
        static T *allocate(size_t n)
        {
            return n ? (T *) Alloc::allocate(n * sizeof(T)) : nullptr;
        }

        static T *allocate()
        {
            return (T *) Alloc::allocate(sizeof(T));
        }

        static void deallocate(T *p, size_t n)
        {
            Alloc::deallocate(p, n * sizeof(T));
        }

        static void deallocate(T *p)
        {
            Alloc::deallocate(p, sizeof(T));
        }
    };


    /*
     * 第一级配置器
     * */
    class Malloc_alloc
    {
    private:
        static void *oom_malloc(size_t);

        static void *oom_realloc(void *, size_t);

        static void (*malloc_alloc_oom_handler)();

    public:
        static void *allocate(size_t n)
        {
            void *result;
            return (result = malloc(n)) ? result : oom_malloc(n);
        }

        static void deallocate(void *p, size_t n)
        {
            (void) n;

            free(p);
        }

        static void *reallocate(void *p, size_t old_sz, size_t new_sz)
        {
            (void) old_sz;
            void *result;
            return (result = realloc(p, new_sz)) ? result : oom_realloc(p, new_sz);
        }

        static auto set_malloc_handler(void (*f)()) -> void (*)()
        {
            void (*old)() = malloc_alloc_oom_handler;
            malloc_alloc_oom_handler = f;
            return old;
        }
    };

    void (*Malloc_alloc::malloc_alloc_oom_handler)(){};

    void *Malloc_alloc::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void *result;

        while (true)
        {
            my_malloc_handler = malloc_alloc_oom_handler;
            if (!my_malloc_handler)
                throw std::bad_alloc();
            my_malloc_handler();
            result = malloc(n);
            if (result)
                return result;
        }
    }

    void *Malloc_alloc::oom_realloc(void *p, size_t n)
    {
        void (*my_malloc_handler)();
        void *result;

        while (true)
        {
            my_malloc_handler = malloc_alloc_oom_handler;
            if (!my_malloc_handler)
                throw std::bad_alloc();
            my_malloc_handler();
            result = realloc(p, n);
            if (result)
                return result;
        }
    }


    /*
    * 第二级配置器
    * */
    enum
    {
        ALIGN = 8
    };
    enum
    {
        MAX_BYTES = 128
    };
    enum
    {
        NFREELISTS = MAX_BYTES / ALIGN
    };

    class Default_alloc
    {
    private:
        static size_t ROUND_UP(size_t bytes)
        {
            return (bytes + ALIGN - 1) & (~(ALIGN - 1));
        }

        struct obj
        {
            obj *free_list_link;
        };

        static obj *volatile free_list[NFREELISTS];

        static size_t FREELIST_INDEX(size_t bytes)
        {
            return (bytes + ALIGN - 1) / ALIGN - 1;
        }

        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, int &nobjs);

        static char *start_free;
        static char *end_free;
        static size_t heap_size;

    public:
        static void *allocate(size_t n);
        static void deallocate(void *p, size_t n);
        static void *reallocate(void *p, size_t old_sz, size_t new_sz);
    };

    char *Default_alloc::start_free{};
    char *Default_alloc::end_free{};
    size_t Default_alloc::heap_size{};
    Default_alloc::obj * volatile Default_alloc::free_list[NFREELISTS]{};

    void *Default_alloc::allocate(size_t n)
    {
        // 大于128字节，从第一级配置器中分配
        if (n > MAX_BYTES)
            return Malloc_alloc::allocate(n);
        else
        {
            obj * result;

            obj * volatile * my_free_list = free_list + FREELIST_INDEX(n);
            result = *my_free_list;

            if (!result)
                return refill(ROUND_UP(n));
            *my_free_list = result->free_list_link;
            return result;
        }
    }

    void Default_alloc::deallocate(void *p, size_t n)
    {
        // 大于128字节，从第一级配置器中分配
        if (n > MAX_BYTES)
            return Malloc_alloc::deallocate(p, n);
        else
        {
            obj *q = reinterpret_cast<obj *>(p);
            obj * volatile * my_free_list = free_list + FREELIST_INDEX(n);
            q->free_list_link = *my_free_list;
            *my_free_list = q;
        }
    }

    void *Default_alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
    {
        char * new_p;

        if (new_sz > MAX_BYTES)
            new_p = (char *)Malloc_alloc::allocate(new_sz);
        else
        {
            if (old_sz <= MAX_BYTES && ROUND_UP(old_sz) == ROUND_UP(new_sz))
                return p;
            else
                new_p = reinterpret_cast<char *>(allocate(new_sz));
        }
        memmove(new_p, p, old_sz);

        if (old_sz > MAX_BYTES)
            Malloc_alloc::deallocate(p, old_sz);
        else
            deallocate(p, old_sz);
        return new_p;
    }

    void * Default_alloc::refill(size_t n)
    {
        int nobjs = 20;
        char * chunk = chunk_alloc(n, nobjs);

        if (nobjs == 1)
            return chunk;
        else
        {
            obj * volatile * my_free_list = free_list + FREELIST_INDEX(n);
            void * result = chunk;

            obj * current_obj = reinterpret_cast<obj *>(chunk + n);
            *my_free_list = current_obj;

            for (int i = 2; i < nobjs; ++i)
            {
                current_obj->free_list_link = reinterpret_cast<obj *>(chunk + i * n);
                current_obj = current_obj->free_list_link;
            }
            current_obj->free_list_link = nullptr;

            return result;
        }
    }

    char * Default_alloc::chunk_alloc(size_t size, int &nobjs)
    {
        size_t total_bytes = size * nobjs;
        size_t free_bytes = end_free - start_free;

        if (total_bytes <= free_bytes) // 可以满足分配要求
        {
            char * chunk = start_free;
            start_free += total_bytes;
            return chunk;
        }
        else if (free_bytes >= size) // 至少可以分配一个
        {
            nobjs = static_cast<int>(free_bytes / size);
            total_bytes = nobjs * size;
            char * chunk = start_free;
            start_free += total_bytes;
            return chunk;
        }
        else
        {
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);

            // 将剩余的可利用的内存分配给相应的free list
            if (free_bytes > 0)
            {
                obj * volatile * my_free_list = free_list + FREELIST_INDEX(free_bytes);
                obj * head = reinterpret_cast<obj *>(start_free);
                head->free_list_link = *my_free_list;
                *my_free_list = head;
            }

            start_free = (char *)malloc(bytes_to_get);

            if (!start_free)
            {
                // 暂时利用较大的free list中的区块
                for (size_t i = FREELIST_INDEX(size); i < NFREELISTS; ++i)
                {
                    if (free_list[i])
                    {
                        obj * chunk = free_list[i];
                        free_list[i] = chunk->free_list_link;
                        start_free = reinterpret_cast<char *>(chunk);
                        end_free = reinterpret_cast<char *>(chunk) + (i + 1) * 8;

                        return chunk_alloc(size, nobjs);
                    }
                }

                end_free = nullptr;
                start_free = (char *)Malloc_alloc::allocate(bytes_to_get);
            }

            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }

    using alloc = Default_alloc;
}

#endif //MINISTL_ALLOC_HH
