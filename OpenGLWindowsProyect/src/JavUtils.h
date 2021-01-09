
#pragma once

#include <iostream>

namespace JUtils {
    void printError(const char* error);
    
    inline float minf(float val1,float val2){
        return (val1 < val2) ? val1:val2;
    }
    
    inline float maxf(float val1, float val2){
        return (val1 > val2) ? val1:val2;
    }
    
    // Returns the clamped value of the parameter value, DOES NOT CHANGE VALUE
    inline float clampF(float min, float max, float value){
        return minf(maxf(value,min),max);
    }
}

