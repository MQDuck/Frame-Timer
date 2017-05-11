#include <chrono>
//#include <deque>

namespace frametimer
{

template<typename T = float>
class Timer
{
private:
    typedef std::chrono::steady_clock steady_clock;

    T* frames;
    int numFrames, numFramesActual, framesIndex;
    steady_clock::time_point time;
    T frameTimeTotal;

public:
    Timer(const int numFrames = 1);
    ~Timer() { delete[] frames; }
    bool setNumFrames(const int numFrames);
    const int getNumFrames() const { return numFrames; }
    bool setTime(const steady_clock::time_point& time = steady_clock::now());
    const steady_clock::time_point getTime() const { return time; }
    void ping();
    const T getFrameTime() const { return frames[framesIndex]; }
    const T getFrameTimeAverage() const;
    const T getFPS() const;
    const T getFPSAverage() const;
};

template<typename T>
Timer<T>::Timer(const int numFrames)
{
    this->numFrames = 1;
    setNumFrames(numFrames);
    time = steady_clock::now();
}

template<typename T>
bool Timer<T>::setNumFrames(const int numFrames)
{
    if(numFrames < 1)
        return false;

    this->numFrames = numFrames;
    if(frames)
        delete[] frames;
    frames = new T[numFrames]();
    framesIndex = 0;
    frameTimeTotal = 0;
    numFramesActual = 0;
    return true;
}

template<typename T>
bool Timer<T>::setTime(const steady_clock::time_point& time)
{
    this->time = time;
    return true;
}

template<typename T>
void Timer<T>::ping()
{
    steady_clock::time_point timeOld = time;
    time = steady_clock::now();
    T frameTime = std::chrono::duration_cast<std::chrono::duration<T>>(time - timeOld).count();
    frameTimeTotal -= frames[framesIndex];
    frameTimeTotal += frameTime;
    frames[framesIndex] = frameTime;
    framesIndex = (framesIndex + 1) % numFrames;
    if(numFramesActual < numFrames)
        ++numFramesActual;
}

template<typename T>
const T Timer<T>::getFrameTimeAverage() const
{
    if(numFramesActual == 0)
        return 0;
    return frameTimeTotal / numFramesActual;
}

template<typename T>
const T Timer<T>::getFPS() const
{
    return 1 / getFrameTime();
}

template<typename T>
const T Timer<T>::getFPSAverage() const
{
    return numFramesActual / frameTimeTotal;
}

/*template<typename T = float>
class Timer
{
private:
    typedef std::chrono::steady_clock steady_clock;

    typename std::result_of<decltype(&std::deque<T>::size)(std::deque<T>)>::type numFrames;
    std::deque<T> frames;
    steady_clock::time_point time;
    T frameTimeTotal;

public:
    Timer(const int numFrames = 1);
    bool setNumFrames(const int numFrames);
    const int getNumFrames() const { return numFrames; }
    bool setTime(const steady_clock::time_point& time = steady_clock::now());
    const steady_clock::time_point getTime() const { return time; }
    void ping();
    const T getFrameTime() const { return frames.front(); }
    const T getFrameTimeAverage() const;
    const T getFPS() const;
    const T getFPSAverage() const;
};

template<typename T>
Timer<T>::Timer(const int numFrames)
{
    this->numFrames = 1;
    setNumFrames(numFrames);
    frameTimeTotal = 0;
    time = steady_clock::now();
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
bool Timer<T>::setTime(const steady_clock::time_point& time)
{
    this->time = time;
    return true;
}

template<typename T>
void Timer<T>::ping()
{
    steady_clock::time_point timeOld = time;
    time = steady_clock::now();
    T frameTime = std::chrono::duration_cast<std::chrono::duration<T>>(time - timeOld).count();
    frames.push_front(frameTime);
    frameTimeTotal += frameTime;
    if(frames.size() > numFrames)
    {
        frameTimeTotal -= frames.back();
        frames.pop_back();
    }
}

template<typename T>
const T Timer<T>::getFrameTimeAverage() const
{
    if(frames.empty())
        return 0;
    return frameTimeTotal / frames.size();
}

template<typename T>
const T Timer<T>::getFPS() const
{
    return 1 / getFrameTime();
}

template<typename T>
const T Timer<T>::getFPSAverage() const
{
    return frames.size() / frameTimeTotal;
}*/

} // namespace frametimer
