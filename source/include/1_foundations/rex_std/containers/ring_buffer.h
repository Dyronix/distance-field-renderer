#pragma once

namespace rex
{
    template <class T, size_t capacity>
    class ringbuffer
    {
    public:
        const T& push_back(const T& item);

        const T& front() const;
        const T& back() const;

        T* data();
        const T* data() const;

        size_t head_index() const;
        size_t tail_index() const;

        size_t size() const;

        bool is_empty() const;

    private:
        size_t wrap(size_t value, size_t max, size_t min = 0) const;

        T m_buffer[capacity];

        size_t m_head = 0;
        size_t m_tail = 0;

        size_t m_current_size = 0;
    };

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    const T& rex::ringbuffer<T, capacity>::push_back(const T& item)
    {
        m_buffer[m_tail] = item;

        m_tail = wrap(m_tail + (size_t)1, capacity);

        if (m_current_size < capacity)
        {
            ++m_current_size;
        }
        else
        {
            m_head = wrap(m_head + (size_t)1, capacity);
        }

        return item;
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    const T& rex::ringbuffer<T, capacity>::front() const
    {
        if (is_empty())
        {
            throw std::runtime_error("ring buffer is empty, nothing to was found at the head of the buffer.");
        }

        return m_buffer[m_head];
    }
    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    const T& rex::ringbuffer<T, capacity>::back() const
    {
        if (is_empty())
        {
            throw std::runtime_error("ring buffer is empty, nothing was found at the tail of the buffer.");
        }

        return m_buffer[m_tail];
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    T* rex::ringbuffer<T, capacity>::data()
    {
        return &m_buffer[0];
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    const T* rex::ringbuffer<T, capacity>::data() const
    {
        return &m_buffer[0];
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    size_t rex::ringbuffer<T, capacity>::head_index() const
    {
        return m_head;
    }
    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    size_t rex::ringbuffer<T, capacity>::tail_index() const
    {
        return m_tail;
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    size_t rex::ringbuffer<T, capacity>::size() const
    {
        return m_current_size;
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    bool rex::ringbuffer<T, capacity>::is_empty() const
    {
        return m_head == m_tail;
    }

    //-------------------------------------------------------------------------
    template <class T, size_t capacity>
    size_t rex::ringbuffer<T, capacity>::wrap(size_t value, size_t max, size_t min) const
    {
        return value >= max ? min : value;
    }
}