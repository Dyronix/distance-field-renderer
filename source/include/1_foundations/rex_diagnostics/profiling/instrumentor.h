
//
// Basic instrumentation profiler by Cherno
// Modified by davedb

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::get().beginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::get().endSession();                        // End Session

#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>

namespace rex
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string name;

        FloatingPointMicroseconds start;
        std::chrono::microseconds elapsed_time;

        std::thread::id thread_id;
    };

    struct InstrumentationSession
    {
        std::string name;
    };

    class Instrumentor
    {
    public:
        Instrumentor();

        void enable();
        void disable();
        bool is_enabled() const;

        void begin_session(const std::string& name, const std::string& filepath = "results.json");
        void end_session();

        void write_profile(const ProfileResult& result);

        static Instrumentor& get();

    private:
        void write_header();
        void write_footer();

        void internal_end_session();

        InstrumentationSession* m_current_session;

        std::mutex m_mutex;
        std::ofstream m_output_stream;

        bool m_enabled;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name);
        ~InstrumentationTimer();

        void stop();

    private:
        const char* m_name;
        bool m_stopped;

        std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
    };
}