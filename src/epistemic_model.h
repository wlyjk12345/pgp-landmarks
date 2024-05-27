#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <regex>
#include "PDDL_TERNARY.h"
#include "EpistemicQuery.h"
#include "state.h"
#include "instance.h"
#include "parser.h"
#include "coin.h"


class EpistemicModel {
public:

    // Destructor
    ~EpistemicModel() {
        // Clean up dynamically allocated resources
    }

    std::map<std::string, PDDL_TERNARY> epistemicGoalsHandler(std::map<std::string, PDDL_TERNARY> epistemic_goals_list, std::string prefix, vector<State*> &path,  vector<State*> &p_path, StateDescriptor *sd, Instance *ins ) {//std::vector<std::map<std::string, int>> path, std::vector<std::map<std::string, int>> p_path) {
        std::map<std::string, PDDL_TERNARY> perspectives_dict;
        std::map<std::string, EpistemicQuery> eq_dict;
        std::map<std::string, PDDL_TERNARY> result_dict;
	
		
        for (auto const& [epistemic_goal_str, value] : epistemic_goals_list) {
            EpistemicQuery temp_eq = partially_converting_to_eq(epistemic_goal_str);
            if (temp_eq.header_str.empty()) {
                // This is the end of eq
                // No need to generate perspectives
                // Just need to evaluate the result and return value
                // logger.debug("query key: [%s]", temp_eq);

                PDDL_TERNARY result = _evaluateContent(path, temp_eq.q_content, sd, ins);
                result_dict[temp_eq.q_content] = result;
            } else {
                // It means the query is not at the last level yet
                std::string agents_str = temp_eq.agents_str;
                std::string content = temp_eq.q_content;
                std::string key = " " + temp_eq.header_str + " [" + agents_str + "]";

                if (eq_dict.find(key) != eq_dict.end()) {
                    eq_dict[key].content_value[content] = value;
                } else {
                    eq_dict[key] = EpistemicQuery(temp_eq.eq_type, temp_eq.agents_str, content, value);
                }
            }
        }

        for (auto const& [key, item] : eq_dict) {
            vector<State*> new_path;
            EQ_TYPE eq_type = item.eq_type;
            int agt_id = ins->getObjectAddress(item.agents_str);
            if (eq_type == EQ_TYPE::BELIEF) {
                new_path = _generateOnePerspectives(path, agt_id, sd);

            } else if (eq_type == EQ_TYPE::SEEING || eq_type == EQ_TYPE::KNOWLEDGE) {
                new_path = _generateOneObservations(path, agt_id, sd);
            } else {
                assert("wrong eq_type of the epistemic query");
            }

            std::map<std::string, PDDL_TERNARY> local_result_dict = epistemicGoalsHandler(item.content_value, key, new_path, p_path, sd, ins);

            for (auto ptr : new_path) {
                delete ptr;
            }
            // Clear the vector if necessary
            new_path.clear();

            std::map<std::string, PDDL_TERNARY> local_perspectives = local_result_dict;
            for (auto const& [lp_key, lp_value] : local_perspectives) {
                std::string p_key = key + lp_key;
                perspectives_dict[p_key] = lp_value;
            }
			
            for (auto const& [result_key, result_value] : local_result_dict) {
                std::string new_result_key = key + result_key;
                PDDL_TERNARY new_result_value = result_value;
                if (eq_type == EQ_TYPE::SEEING) {    
                    if (!Coin::agentsExists(new_path, agt_id)) {
                        new_result_value = PDDL_TERNARY::UNKNOWN;
                    } else if (result_value == PDDL_TERNARY::UNKNOWN) {
                        new_result_value = PDDL_TERNARY::FALSE;
                    } else {
                        new_result_value = PDDL_TERNARY::TRUE;
                    }
                }
                result_dict[new_result_key] = new_result_value;
            }
        }
        eq_dict.clear();
        perspectives_dict.clear();
        return result_dict;
    }

    PDDL_TERNARY _evaluateContent(std::vector<State*>& path, std::string temp_eq, StateDescriptor* sd, Instance *ins){
        State* state = path.back(); // Accessing the last state in the path

        // Parse temp_eq string to extract predicate type and value
        std::istringstream iss(temp_eq);
        std::string word;
        iss >> word; // Read the first word, expecting "("

        if (word != "(") {
            std::cerr << "Error: Expected '(' but found '" << word << "'" << std::endl;
            return PDDL_TERNARY::UNKNOWN;
        }

        // Read grounded predicate
        iss >> word;
        size_t pos = word.find('(');

        if (pos == std::string::npos) {
            std::cerr << "Error: Invalid predicate format" << std::endl;
            return PDDL_TERNARY::UNKNOWN;
        }

        std::string pred_type = word.substr(0, pos);
        // Get object names
        std::vector<std::string> obj_names = Parser::split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
        std::vector<int> obj_idxs(obj_names.size());

		 for (int oname_idx = 0; oname_idx < static_cast<int>(obj_names.size()); oname_idx++) {
			obj_idxs[oname_idx] = ins->getObjectAddress(obj_names[oname_idx]);
		}


        // Read "="
        iss >> word;
        if (word != "=") {
            std::cerr << "Error: Expected '=' but found '" << word << "'" << std::endl;
            return PDDL_TERNARY::UNKNOWN;
        }

        // Read value
        int value;
        iss >> value;
		

        // Get state value for the given predicate type and object indexes
        int state_value = state->getRegister(sd, pred_type, obj_idxs);
        //getRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &var_obj_idx )

        // Check if state_value matches the provided value
        if (state_value == -1) {         // when get -1, not find in state
            return PDDL_TERNARY::UNKNOWN;
        } else if (state_value == value) {
            return PDDL_TERNARY::TRUE;
        } else {
            return PDDL_TERNARY::FALSE;
        }
    }

	
	std::vector<State*> _generateOnePerspectives(std::vector<State*>& path, const int& agt_id, StateDescriptor* sd) {
    State* state_template = path[0];

    std::vector<State*> new_path;
    std::vector<std::vector<std::map<std::vector<int>, int>>> observation_list;
    
    for (size_t i = 0; i < path.size(); i++) {
        observation_list.push_back(_getOneObservation(path[i], agt_id, sd));
    }
	
    
    for (size_t i = 0; i < path.size(); i++) {
        State* new_state = _generateOnePerspective(*state_template, std::vector<std::vector<std::map<std::vector<int>, int>>>(observation_list.begin(), observation_list.begin() + i + 1), sd);
        new_path.push_back(new_state);
    }
    
    return new_path;
}



	std::vector<State*> _generateOneObservations(const vector<State*>& path, const int& agt_id, StateDescriptor* sd) {
    std::vector<State*> new_path;
    for (size_t i = 0; i < path.size(); i++) {
        State* new_state = new State();
        
        std::vector<std::map<std::vector<int>, int>> observation = _getOneObservation(path[i], agt_id, sd);
        new_state->setRegister(sd, observation);
        new_path.push_back(new_state);
    }
    return new_path;
}



    State* _generateOnePerspective(const State& state_template, const std::vector<std::vector<std::map<std::vector<int>, int>>>& observation_list, StateDescriptor* sd) {
        State* new_state = new State(sd);         // need change defination
        const vector< map< vector<int>, int > >  typed_registers = state_template.getTypedRegisters();

	vector<string> PredicateTypes = sd->getPredicateTypes();

        for (int pred_type = 0; pred_type < static_cast<int>(typed_registers.size()); ++pred_type) {
            string pred_type_string = PredicateTypes[pred_type];
            const std::map<std::vector<int>, int>& content = typed_registers[pred_type];
            const int pred_idx = sd->getPredicateIDX( pred_type_string );

            for (auto& [obj_idxs, e] : content) {
                //std::cout << "\t find history value for [" << pred_type_string << "],[" << e << "]" << std::endl;
                int ts_index = identifyLastSeenTimestamp(observation_list, pred_idx, obj_idxs);
                //std::cout << "\t last seen timestamp index: [" << ts_index << "]" << std::endl;
                int value = identifyMemorizedValue(observation_list, ts_index, pred_idx, obj_idxs);
                //std::cout << "\t [" << pred_type << "]'s value is: [" << value << "]" << std::endl;
                new_state->setRegister(sd, pred_type, obj_idxs, value);
            }
        }
        
        return new_state;
    }

    int identifyMemorizedValue(const std::vector<std::vector<std::map<std::vector<int>, int>>>& observation_list, const int ts_index, const int& pred_idx, const std::vector<int>& var_obj_idx) {
        int ts_index_temp = ts_index;
        
        if (ts_index_temp < 0) return -1;
        
        while (ts_index_temp >= 0) {
            const  std::vector<std::map<std::vector<int>, int>>& temp_observation = observation_list[ts_index_temp];
            
            if (temp_observation[pred_idx].find(var_obj_idx) == temp_observation[pred_idx].end() || temp_observation[pred_idx].count(var_obj_idx) == 0 ) {
                ts_index_temp -= 1;
            } else {
                return temp_observation[pred_idx].at(var_obj_idx);
            }
        }
        
        ts_index_temp = ts_index + 1;
        
        while (ts_index_temp < static_cast<int>(observation_list.size())) {
            const  std::vector<std::map<std::vector<int>, int>>& temp_observation = observation_list[ts_index_temp];
            
            if (temp_observation[pred_idx].find(var_obj_idx) == temp_observation[pred_idx].end() || temp_observation[pred_idx].count(var_obj_idx) == 0) {
                ts_index_temp += 1;
            } else {
                return temp_observation[pred_idx].at(var_obj_idx);
            }
        }
        
        return -1;
    }

    int identifyLastSeenTimestamp(const std::vector<std::vector<std::map<std::vector<int>, int>>>& observation_list, const int& pred_idx, const std::vector<int>& var_obj_idx) {
        int ts_index_temp = observation_list.size() - 1;
        
        while (ts_index_temp >= 0) {
            if (observation_list[ts_index_temp][pred_idx].find(var_obj_idx) != observation_list[ts_index_temp][pred_idx].end()) {
                return ts_index_temp;
            } else {
                ts_index_temp -= 1;
            }
        }
        return -1;
    }

    std::vector<std::map<std::vector<int>, int>> _getOneObservation(const State* state, const int& agt_id, StateDescriptor* sd) {

        const std::vector<std::map<std::vector<int>, int>>& typed_registers = state->getTypedRegisters();
        std::vector<std::map<std::vector<int>, int>> new_typed_registers;

        vector<string> PredicateTypes = sd->getPredicateTypes();
        for (int pred_type = 0; pred_type < static_cast<int>(typed_registers.size()); ++pred_type) {
            const std::map<std::vector<int>, int>& content = typed_registers[pred_type];

            string pred_type_string = PredicateTypes[pred_type];
            const int pred_idx = sd->getPredicateIDX( pred_type_string );
			

            std::map<std::vector<int>, int> new_map;

            for (const auto& [obj_idxs, value] : content) {
                if (Coin::checkVisibility(typed_registers, agt_id, pred_idx, obj_idxs, sd) == PDDL_TERNARY::TRUE) {
                    new_map[obj_idxs] = value;
                }
            }

            new_typed_registers.push_back(new_map);
        }

        return new_typed_registers;
    }


    std::map<std::string, vector<State*>> epistemicPathsHandler(std::map<std::string, PDDL_TERNARY> epistemic_goals_list, std::string prefix, vector<State*> &path,  vector<State*> &p_path, StateDescriptor *sd, Instance *ins ) {//std::vector<std::map<std::string, int>> path, std::vector<std::map<std::string, int>> p_path) {
        std::map<std::string, PDDL_TERNARY> perspectives_dict;
        std::map<std::string, EpistemicQuery> eq_dict;
        std::map<std::string, PDDL_TERNARY> result_dict;
        std::map<std::string, vector<State*>> result_path;


        for (auto const& [epistemic_goal_str, value] : epistemic_goals_list) {
            EpistemicQuery temp_eq = partially_converting_to_eq(epistemic_goal_str);
            if (temp_eq.header_str.empty()) {
                // This is the end of eq
                // No need to generate perspectives
                // Just need to evaluate the result and return value
                // logger.debug("query key: [%s]", temp_eq);

                //PDDL_TERNARY result = _evaluateContent(path, temp_eq.q_content, sd, ins);
                //result_dict[temp_eq.q_content] = result;
                result_path[""] = path;
                return result_path;
            } else {
                // It means the query is not at the last level yet
                std::string agents_str = temp_eq.agents_str;
                std::string content = temp_eq.q_content;
                std::string key = " " + temp_eq.header_str + " [" + agents_str + "]";

                if (eq_dict.find(key) != eq_dict.end()) {
                    eq_dict[key].content_value[content] = value;
                } else {
                    eq_dict[key] = EpistemicQuery(temp_eq.eq_type, temp_eq.agents_str, content, value);
                }
            }
        }

        for (auto const& [key, item] : eq_dict) {
            vector<State*> new_path;
            EQ_TYPE eq_type = item.eq_type;
            int agt_id = ins->getObjectAddress(item.agents_str);
            if (eq_type == EQ_TYPE::BELIEF) {
                new_path = _generateOnePerspectives(path, agt_id, sd);

            } else if (eq_type == EQ_TYPE::SEEING || eq_type == EQ_TYPE::KNOWLEDGE) {
                new_path = _generateOneObservations(path, agt_id, sd);
            } else {
                assert("wrong eq_type of the epistemic query");
            }
            std::map<std::string, vector<State*>> local_result_path_dict = epistemicPathsHandler(item.content_value, key, new_path, p_path, sd, ins);
            std::map<std::string, vector<State*>> local_perspectives = local_result_path_dict;
            for (auto const& [lp_key, lp_value] : local_result_path_dict) {
                std::string p_key = key + lp_key;
                local_perspectives[p_key] = lp_value;
            }
            result_path = local_perspectives;
        }
        perspectives_dict.clear();
        result_dict.clear();
        eq_dict.clear();

        return result_path;
    }



    vector<int> asVector(const std::map<std::string, std::vector<State*>>& e_outcome, ProgramState *ps) {
        vector< int > res( 1, ps->getLine() );

        for (const auto& pair : e_outcome) {
            const State* state = pair.second.back();
            auto vars = state->getStateVars();
            for( auto v : vars ){
                res.insert( res.end(), v.begin(), v.end() );
            }
        }
        return res;
    }

    static void freeEStates( vector< ProgramState* > &vps ){
        for( auto ps : vps ){
            delete ps;
        }
    }


	
	static EpistemicQuery partially_converting_to_eq(std::string eq_str) {
    std::regex pattern("[ksb] \\[[0-9a-z_,]*\\] ");
    std::smatch match;
    if (!std::regex_search(eq_str, match, pattern)) {
        // Log debug message
        return EpistemicQuery(eq_str);
		cout<<"EMeq_str"<<eq_str<<endl;
    } else {
        std::string header_str, agents, content;
        std::istringstream iss(eq_str);
        iss >> header_str >> agents;
        std::getline(iss, content);

        // Convert header_str to char
        char query_key = header_str[0];
		// Remove spaces from agents string
        agents.erase(std::remove(agents.begin(), agents.end(), '['), agents.end());
		agents.erase(std::remove(agents.begin(), agents.end(), ']'), agents.end());
		
		//cout<<"EMheader_str:"<<header_str<<", agents:"<<agents<<", content:"<<content<<endl;
        return EpistemicQuery(query_key, agents, content);
    }
}

	
	
};
