#pragma once

namespace rex
{
    template <typename T, typename TParameter>
    class NamedType
    {
        static_assert(std::is_fundamental<T>::value, "Class can only be used with primitive types");

    public:
        NamedType() noexcept;
        NamedType(const T& edge) noexcept;

        explicit NamedType(const NamedType<T, TParameter>& other) noexcept;
        explicit NamedType(NamedType<T, TParameter>&& other) noexcept;

        NamedType<T, TParameter>& operator=(const NamedType<T, TParameter>& other) noexcept;
        NamedType<T, TParameter>& operator=(NamedType<T, TParameter>&& other) noexcept;

        operator T() const noexcept;

        T& get();
        const T& get() const;

    private:
        T m_value;
    };

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    rex::NamedType<T, TParameter>::NamedType() noexcept : m_value(T())
    {
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    rex::NamedType<T, TParameter>::NamedType(const NamedType<T, TParameter>& other) noexcept : m_value(other.m_value)
    {
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    rex::NamedType<T, TParameter>::NamedType(NamedType<T, TParameter>&& other) noexcept : m_value(std::exchange(other.m_value, T()))
    {
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    rex::NamedType<T, TParameter>::NamedType(const T& edge) noexcept : m_value(edge)
    {
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    NamedType<T, TParameter>& rex::NamedType<T, TParameter>::operator=(const NamedType<T, TParameter>& other) noexcept
    {
        m_value = other.m_value;
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    NamedType<T, TParameter>& rex::NamedType<T, TParameter>::operator=(NamedType&& other) noexcept
    {
        m_value = std::exchange(other.m_value, 0.0f);
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    rex::NamedType<T, TParameter>::operator T() const noexcept
    {
        return m_value;
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    T& rex::NamedType<T, TParameter>::get()
    {
        return m_value;
    }

    //-------------------------------------------------------------------------
    template <typename T, typename TParameter>
    const T& rex::NamedType<T, TParameter>::get() const
    {
        return m_value;
    }
}