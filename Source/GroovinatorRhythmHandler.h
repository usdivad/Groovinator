//
//  GroovinatorRhythmHandler.h
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#ifndef GroovinatorRhythmHandler_h
#define GroovinatorRhythmHandler_h

#include <stdio.h>
#include <vector>

class GroovinatorRhythmHandler
{
public:
    GroovinatorRhythmHandler();
    GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps);
    GroovinatorRhythmHandler(int originalNumSteps, std::vector<int> originalRhythm,
                             int targetNumSteps, std::vector<int> targetRhythm);
    ~GroovinatorRhythmHandler();
    
    // Getters
    int getOriginalNumSteps();
    int getTargetNumSteps();
    std::vector<int> getOriginalRhythm();
    std::vector<int> getTargetRhythm();
    std::string getOriginalRhythmStr();
    std::string getTargetRhythmStr();
    
    // Setters
    void setOriginalNumSteps(int v);
    void setTargetNumSteps(int v);
    void setOriginalRhythm(std::vector<int> r);
    void setTargetRhythm(std::vector<int> r);
    
    // Static methods
    static std::vector<int> generateEuclideanRhythm(int numPulses, int numSteps);
    static std::string rhythmToString(std::vector<int> rhythm);

private:
    int _originalNumSteps;
    std::vector<int> _originalRhythm;
    int _targetNumSteps;
    std::vector<int> _targetRhythm;
};

#endif /* GroovinatorRhythmHandler_h */
