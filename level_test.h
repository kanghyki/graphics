#ifndef INCLUDED_CREATE_TEST_LEVEL_H
#define INCLUDED_CREATE_TEST_LEVEL_H

class CreateTestLevel
{
  public:
    CreateTestLevel();
    ~CreateTestLevel();

    void Create();

  private:
    CreateTestLevel(const CreateTestLevel &);
    CreateTestLevel &operator=(const CreateTestLevel &);
};

#endif
