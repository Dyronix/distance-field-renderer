#pragma once

namespace rex
{
    class AspectRatio
    {
    public:
        class Ratio
        {
        public:
            explicit Ratio(float x, float y)
            {
                R_ASSERT(y != 0);

                ratio = x / y;
            }

            float get() const
            {
                return ratio;
            }

        private:
            float ratio;
        };

        AspectRatio(float width, float height);

        void set_width(float width);
        void set_height(float height);

        float get_width() const;
        float get_height() const;

        Ratio get_ratio() const;

    private:
        float m_width;
        float m_height;

        Ratio m_ratio;
    };
}