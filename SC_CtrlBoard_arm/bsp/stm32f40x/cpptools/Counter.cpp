#include "Counter.h"

void Counter::Increase()
{
    if (++present_counter > limited_counter)
    {
        m_result = true;
        present_counter = limited_counter;
    }
}

void Counter::Decrease()
{
    if (--present_counter < 0)
    {
        present_counter = 0;
        m_result = false;
    }
}

void Counter::Reset()
{
    present_counter = 0;
    m_result = false;
}

