//
//  Time.h
//  OpenGLProject
//
//  Created by Patxi Arin Reiriz on 4/5/20.
//  Copyright © 2020 Javier Arin Reiriz. All rights reserved.
//


// This will be a class to handle time, drawing times and ellapsed times to control frame speed, physics, etc...
#ifndef Time_h
#define Time_h

#include <chrono>

class TimeManager{
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
    float deltaTime;
public:
    TimeManager(){deltaTime = 0.0f;};
    void StartPoint(){
        start = std::chrono::high_resolution_clock::now();
    }
    void FinishPoint(){
        end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float,std::milli>(end-start).count();
    }
    float getDeltaTime(){
        return deltaTime;
    }
};

#endif /* Time_h */
