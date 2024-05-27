#include <vector>
#include <map>
#include "PDDL_TERNARY.h" // Include the header file for PDDL_TERNARY
#include "state.h"

class Coin {
public:
    static bool agentsExists(const std::vector<State*>& path, int agt_id) {
            const State* state = path.back(); // Use .back() to access the last element of the vector
            const std::vector<std::map<std::vector<int>, int>>& typed_registers = state->getTypedRegisters();

            std::vector<int> obj_idxs; // Correct way to declare a vector
            obj_idxs.push_back(agt_id); // Append agt_id to obj_idxs

            for (const auto& i : typed_registers) {
                if (i.count(obj_idxs) != 0) // Check if agt_id exists in each map i
                    return true;
            }

            return false;
        }

    static PDDL_TERNARY checkVisibility(const std::vector<std::map<std::vector<int>, int>>& typed_registers, int agt_id, int pred_type, const std::vector<int>& obj_idxs, StateDescriptor* sd) {
        try {
            std::string peek = "peeking";
            int pred_peek_idx = sd->getPredicateIDX(peek);
            std::vector<int> obj_idxs; // Correct way to declare a vector

            obj_idxs.push_back(agt_id); // Append agt_id to obj_idxs

            // agents are able to see each other
            if (pred_peek_idx == pred_type) {
                return PDDL_TERNARY::TRUE;
            } else {
                // Check if agt_id exists in the typed_registers for the specified pred_type
                auto it = typed_registers[pred_peek_idx].find(obj_idxs);
                if (typed_registers.size() > static_cast<size_t>(pred_peek_idx) && it != typed_registers[pred_peek_idx].end() && it->second == 1) {
                    //todo
                    return PDDL_TERNARY::TRUE;
                } else {
                    return PDDL_TERNARY::FALSE;
                }
            }
        } catch (const std::out_of_range& e) {
            return PDDL_TERNARY::UNKNOWN;
        }
    }
};
