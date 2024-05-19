#ifndef INCLUDED_TIME_MANAGER_H
#define INCLUDED_TIME_MANAGER_H

class TimeManager
{
  public:
    static TimeManager *GetInstance();
    ~TimeManager();

    void Reset();
    void Start();
    void Stop();
    void Tick();

    float delta_time() const;
    float total_time() const;

  private:
    TimeManager();
    TimeManager(const TimeManager &c);
    TimeManager &operator=(const TimeManager &c);

    static TimeManager *instance_;
    float base_time_;
    float curr_time_;
    float prev_time_;
    float delta_time_;
    float paused_time_;
    float stop_time_;
    bool is_stopped_;
};

#endif
