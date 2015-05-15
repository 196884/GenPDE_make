#ifndef GP_PARALLELIZER_H
#define GP_PARALLELIZER_H

#include "GpThreadPool.h"

namespace GenPDE
{
    class Parallelizer;

    class Parallelizer
    {
    public:
        template < typename TASK >
        class Task
        {
        public:
            Task( TASK task, Parallelizer & parallelizer )
            : m_task( task )
            , m_parallelizer( parallelizer )
            {}

            void operator()()
            {
                m_task();
                m_parallelizer.notifyDone();
            }

        private:
            std::function< void() > m_task;
            Parallelizer &          m_parallelizer;
        };

    public:
        Parallelizer( ThreadPool & thread_pool )
        : m_threadPool( thread_pool )
        , m_nbTasks( 0 )
        , m_nbTasksDone( 0 )
        {}

        // called from a single thread before calling 'run'
        template < typename TASK >
        void addTask( TASK task )
        {
            m_tasks.push_back( Task<TASK>( task, *this ) );
            ++m_nbTasks;
        }

        // used by tasks to notify they're done...
        void notifyDone()
        {
            bool done = false;
            {
                std::unique_lock< std::mutex > lock( m_mutex );
                ++m_nbTasksDone;
                done = m_nbTasksDone == m_nbTasks;
            } // Releases the lock

            if( done )
                m_conditionVar.notify_one();
        }

        // Will launch the computation and block until all are complete
        void run()
        {
            while( !m_tasks.empty() )
            {
                m_threadPool.enqueue( m_tasks.front() );
                m_tasks.pop_front();
            }

            std::unique_lock< std::mutex > lock( m_mutex );
            while( m_nbTasksDone != m_nbTasks )
            {
                m_conditionVar.wait( lock );
            }
            return;
        }

    private:
        ThreadPool &                          m_threadPool;
        std::deque< std::function< void() > > m_tasks;
        std::mutex                            m_mutex;
        std::condition_variable               m_conditionVar;
        unsigned int                          m_nbTasks;
        unsigned int                          m_nbTasksDone;
    }; // class Parallelizer
} // namespace GenPDE

#endif // GP_PARALLELIZER_H
