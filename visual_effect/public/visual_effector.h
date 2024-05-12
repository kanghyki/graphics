#ifndef INCLUDED_VISUAL_EFFECTOR_H
#define INCLUDED_VISUAL_EFFECTOR_H

class VisualEffector
{
  public:
    static VisualEffector *GetInstance();
    ~VisualEffector();

    void Render();

  private:
    VisualEffector();
    VisualEffector(const VisualEffector &);
    VisualEffector &operator=(const VisualEffector &);

    static VisualEffector *instance_;
};

#endif
