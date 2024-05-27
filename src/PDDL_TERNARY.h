#ifndef PDDL_TERNARY_H
#define PDDL_TERNARY_H

#include <iostream>

enum class PDDL_TERNARY {
    TRUE = 1,
    UNKNOWN = 0,
    FALSE = -1
};

bool operator<(PDDL_TERNARY left, PDDL_TERNARY right) {
    return static_cast<int>(left) < static_cast<int>(right);
}

PDDL_TERNARY convertBooltoPDDL_TERNARY(bool value) {
    return value ? PDDL_TERNARY::TRUE : PDDL_TERNARY::FALSE;
}

PDDL_TERNARY convertIntToPDDL_TERNARY(int value) {
    if (value == 1)
        return PDDL_TERNARY::TRUE;
    else if (value == 0)
        return PDDL_TERNARY::UNKNOWN;
    else
        return PDDL_TERNARY::FALSE;
}

std::string PDDL_TERNARY_to_string(PDDL_TERNARY value) {
    switch(value) {
        case PDDL_TERNARY::TRUE: return "TRUE";
        case PDDL_TERNARY::UNKNOWN: return "UNKNOWN";
        case PDDL_TERNARY::FALSE: return "FALSE";
        default: return "Invalid";
    }
}
#endif // PDDL_TERNARY_H
