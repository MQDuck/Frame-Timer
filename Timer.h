/*
    Copyright 2017 Jeffrey Thomas Piercy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FRAMETIMER_TIMER_H
#define FRAMETIMER_TIMER_H

#include <chrono>
#include <memory>

namespace frametimer
{

template<typename T = float>
class Timer
{
private:
    typedef std::chrono::steady_clock steady_clock;

    std::unique_ptr<T[]> frames;
    int numFrames, numFramesActual, framesIndex;
    steady_clock::time_point time;
    T frameTimeTotal;

public:
    Timer(const int numFrames = 1);
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
Timer<T>::Timer(const int numFrames) : frames()
{
    if(numFrames < 1)
        throw std::runtime_error("Parameter numFrames to Timer<>::Timer(int numFrames) must be >= 1");
    
    setNumFrames(numFrames);
    time = steady_clock::now();
}

template<typename T>
bool Timer<T>::setNumFrames(const int numFrames)
{
    if(numFrames < 1)
        return false;

    this->numFrames = numFrames;
    frames.reset(new T[numFrames]);
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

} // namespace frametimer

#endif // FRAMETIMER_TIMER_H