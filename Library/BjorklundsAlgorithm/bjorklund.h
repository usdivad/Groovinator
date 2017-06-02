// Implementation of Bjorklund's algorithm by Sergio Castro
// https://bitbucket.org/sjcastroe/bjorklunds-algorithm

#ifndef BJORKLUND_H
#define BJORKLUND_H

#include <string>

/**
bjorklund evenly distributes a number of beats within a set number of total steps (thereby creating a rhythm)
Ex. if beats = 4 and steps = 8, "11110000" ==> "10101010"
@param beats is the number of "active" steps (a single beat is represented by a "1")
@param steps is the total number of units (total number of "1"s and "0"s)
*/
namespace BjorklundsAlgorithm
{
    std::string bjorklund(int beats, int steps);
}

#endif
