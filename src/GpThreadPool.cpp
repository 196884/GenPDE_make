#include "GpThreadPool.h"

void GenPDE::TPWorker::operator()()
{
    std::function< void() > task;
    while( true )
    {
        {
            std::unique_lock< std::mutex > lock( m_threadPool.m_queueMutex );
            while( !m_threadPool.m_terminate && m_threadPool.m_tasks.empty() )
            {
                m_threadPool.m_conditionVar.wait( lock );
            }

            if( m_threadPool.m_terminate )
                return;

            task = m_threadPool.m_tasks.front();
            m_threadPool.m_tasks.pop_front();
        } // Releases the lock

        task();
    }
}

GenPDE::ThreadPool::ThreadPool( size_t nb_threads )
:m_terminate( false )
{
    for( size_t i = 0; i < nb_threads; ++i )
        m_workers.push_back( std::thread( TPWorker( *this ) ) );
}

GenPDE::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock< std::mutex > lock( m_queueMutex );
        m_terminate = true;
    } // Releases the lock

    m_conditionVar.notify_all();

    for( size_t i = 0; i < m_workers.size(); ++i )
        m_workers[i].join();
}
