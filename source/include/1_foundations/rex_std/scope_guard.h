#pragma once

namespace rex
{
    template< typename Lambda >
    class ScopeGuard
    {
    public:

        explicit ScopeGuard(Lambda&& rollback);

        template< typename AdquireLambda >
        ScopeGuard(AdquireLambda&& aquireRollback, Lambda&& rollback);
        ~ScopeGuard();

        inline void release();

    private:
        bool m_engaged;
        Lambda m_rollback;
    };

    //-------------------------------------------------------------------------
    template< typename Lambda >
    ScopeGuard<Lambda>::ScopeGuard(Lambda&& rollback)
        : m_engaged(false)
        , m_rollback(std::forward<Lambda>(rollback))
    {}
    //-------------------------------------------------------------------------
    template< typename Lambda >
    template< typename AdquireLambda >
    ScopeGuard<Lambda>::ScopeGuard(AdquireLambda&& aquireRollback, Lambda&& rollback)
        : m_engaged(false)
        , m_rollback(std::forward<Lambda>(rollback))
    {
        std::forward<AdquireLambda>(aquireRollback());
    }
    //-------------------------------------------------------------------------
    template< typename Lambda >
    ScopeGuard<Lambda>::~ScopeGuard()
    {
        if (!m_engaged)
            m_rollback();
    }
    //-------------------------------------------------------------------------
    template< typename Lambda >
    inline void ScopeGuard<Lambda>::release()
    {
        m_engaged = true;
    }

    //-------------------------------------------------------------------------
    template< typename aLambda, typename rLambda>
    const ScopeGuard< rLambda > make_scope_guard(aLambda&& aquireRollback, rLambda&& rollback)
    {
        return ScopeGuard< rLambda >(std::forward<aLambda>(aquireRollback), std::forward<rLambda>(rollback));
    }

    //-------------------------------------------------------------------------
    template<typename rLambda>
    const ScopeGuard< rLambda > make_scope_guard(rLambda&& rollback)
    {
        return ScopeGuard< rLambda >(std::forward<rLambda>(rollback));
    }
}