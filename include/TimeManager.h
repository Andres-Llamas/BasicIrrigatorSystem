#ifndef TimeManager_h
#define TimeManager_h

class TimeManager
{
private:
    /* data */
public:
    char *day;
    int hours;
    int minutes;
    int seconds;
    void Initialization();
    void UpdateTime();
    void ShowDateAndTime();
    TimeManager(long utcOffsetInSeconds);
};
#endif