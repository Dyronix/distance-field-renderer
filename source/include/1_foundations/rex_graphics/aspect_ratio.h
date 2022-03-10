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

        void setWidth(float width);
        void setHeight(float height);

        float getWidth() const;
        float getHeight() const;

        Ratio getRatio() const;

    private:
        float m_width;
        float m_height;

        Ratio m_ratio;
    };
}