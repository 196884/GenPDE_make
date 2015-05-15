#ifndef GP_THREADPOOL_H
#define GP_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace GenPDE
{
    class ThreadPool;

    class TPWorker
    {
    public:
        TPWorker( ThreadPool & thread_pool )
        :m_threadPool( thread_pool )
        {}

        void operator()();

    private:
        ThreadPool& m_threadPool;
    };

    class ThreadPool
    {
    public:
        ThreadPool( size_t nb_threads );

        template < typename TASK >
        void enqueue( TASK task );

        ~ThreadPool();

    private:
        friend class TPWorker;

        // We have to keep the threads around to join them:
        std::vector< std::thread >            m_workers;

        std::deque< std::function< void() > > m_tasks;

        // Synchronization
        std::mutex                            m_queueMutex;
        std::condition_variable               m_conditionVar;
        bool                                  m_terminate; // FIXME: what is this used for? (add comment)
    };
}; // namespace GenPDE

template < typename TASK >
void GenPDE::ThreadPool::enqueue( TASK task )
{
    {
        std::unique_lock< std::mutex > lock( m_queueMutex );
        m_tasks.push_back( std::function< void() >( task ) );
    } // Releases the lock

    m_conditionVar.notify_one();
}

#endif
