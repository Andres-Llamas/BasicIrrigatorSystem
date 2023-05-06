#ifndef TimeManager_h
#define TimeManager_h
#include "CustomStructs.h"

class TimeManager
{
private:

public:
    clockTime currentTime;
    void Initialization();
    void UpdateTime();
    /// @brief Displays an specific date and time
    /// @param timeToShow
    static void ShowDateAndTime(clockTime timeToShow);

    /// @brief Displays the current date and time obtained from ntpc
    void ShowDateAndTime();
    TimeManager(long utcOffsetInSeconds);
};
#endif