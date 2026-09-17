#pragma once
#include "operation.hpp"

#include <math/math.hpp>

struct Task
{
    int firstNumber;
    int secondNumber;
    Operation operation;
    int result;
    math::Status status;
};
