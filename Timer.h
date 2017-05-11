#include <chrono>
#include <deque>

//namespace frametimer
//{

template<typename T = float>
class Timer
{
private:
    //int numFrames;
    typename std::result_of<decltype(&std::deque<T>::size)(std::deque<T>)>::type numFrames;
    std::deque<T> frames;
    std::chrono::steady_clock::time_point time;

public:
    Timer(const int numFrames = 1);
    bool setNumFrames(const int numFrames);
    const int getNumFrames() const { return numFrames; }
    bool setTime(const std::chrono::steady_clock::time_point time);
    const std::chrono::steady_clock::time_point getTime() const { return time; }
    void ping();
    T getFrameTime();
    T getFrameTimeAverage();
    T getFPS();
    T getFPSAverage();
};

template<typename T>
Timer<T>::Timer(const int numFrames)
{
    time = std::chrono::steady_clock::now();
    setNumFrames(numFrames);
}

template<typename T>
bool Timer<T>::setNumFrames(const int numFrames)
{
    if(numFrames < 1)
        return false;

    this->numFrames = numFrames;
    for(int n = frames.size() - numFrames; n > 0; --n) // reduce frames to numFrames elements
        frames.pop_back();
    return true;
}

template<typename T>
void Timer<T>::ping()
{
    auto timeOld = time;
    time = std::chrono::steady_clock::now();
    frames.push_front(std::chrono::duration_cast<std::chrono::duration<T>>(time - timeOld).count());
    if(frames.size() > numFrames)
        frames.pop_back();
}

template<typename T>
T Timer<T>::getFrameTime()
{
    return frames.front();
}

template<typename T>
T Timer<T>::getFrameTimeAverage()
{
    if(frames.empty())
        return 0;

    T total = 0;
    for(T time : frames)
        total += time;
    return total / frames.size();
}

template<typename T>
T Timer<T>::getFPS()
{
    return 1 / getFrameTime();
}

template<typename T>
T Timer<T>::getFPSAverage()
{
    return 1 / getFrameTimeAverage();
}

//} // namespace frametimer
