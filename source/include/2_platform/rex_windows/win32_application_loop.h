#pragma once

#include "core_application_loop.h"

namespace rex
{
    using RefreshRate = NamedType<int32, struct Rate>;

    //-------------------------------------------------------------------------
    RefreshRate operator"" _hz(long double value)
    {
        return RefreshRate{ gsl::narrow<int32>(value) };
    }

    struct FrameInfo;

    namespace win32
    {
        using UpdateDelegate = std::function<void(const FrameInfo&)>;

        class ApplicationLoop : public CoreApplicationLoop
        {
        public:
            ApplicationLoop(const UpdateDelegate& updateDelegate, const RefreshRate& refreshRate);
            ~ApplicationLoop() override;

            void exec() override;
            void stop() override;

        protected:
            void tick() override;

        private:
            bool m_is_running;
            UpdateDelegate m_update_delegate;
            RefreshRate m_refresh_rate;
        };
    }
}