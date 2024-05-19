#include "time_manager.h"
#include "opengl_device.h"

float GetTime()
{
    return glfwGetTime();
}

TimeManager *TimeManager::instance_ = nullptr;

TimeManager *TimeManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new TimeManager();
    }
    return instance_;
}

TimeManager ::TimeManager()
    : base_time_(0.0f), curr_time_(0.0f), prev_time_(0.0f), delta_time_(0.0f), paused_time_(0.0f), stop_time_(0.0f),
      is_stopped_(false)
{
}

TimeManager ::~TimeManager()
{
}

void TimeManager::Reset()
{
    float curr_time = GetTime();
    base_time_ = curr_time;
    prev_time_ = curr_time;
    stop_time_ = 0.0f;
    is_stopped_ = false;
}

void TimeManager::Start()
{
    float start_time = GetTime();
    if (is_stopped_)
    {
        paused_time_ += (start_time - stop_time_);
        prev_time_ = start_time;
        stop_time_ = 0.0f;
        is_stopped_ = false;
    }
}

void TimeManager::Stop()
{
    stop_time_ = GetTime();
    is_stopped_ = true;
}

void TimeManager::Tick()
{
    if (is_stopped_)
    {
        delta_time_ = 0.0f;
        return;
    }
    curr_time_ = GetTime();
    delta_time_ = curr_time_ - prev_time_;
    prev_time_ = curr_time_;
    if (delta_time_ < 0.0f)
    {
        delta_time_ = 0.0f;
    }
}

float TimeManager::delta_time() const
{
    return delta_time_;
}

float TimeManager::total_time() const
{
    if (is_stopped_)
    {
        return (stop_time_ - paused_time_) - base_time_;
    }
    else
    {
        return (curr_time_ - paused_time_) - base_time_;
    }
}
