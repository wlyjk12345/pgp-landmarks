#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <map>
#include "PDDL_TERNARY.h"

enum class EQ_TYPE {
    KNOWLEDGE,
    SEEING,
    BELIEF
};

class EpistemicQuery {
public:
    std::string q_content;
    EQ_TYPE eq_type;
    std::string header_str;
    std::string agents_str;
    std::map<std::string, PDDL_TERNARY> content_value;
    static std::unordered_map<char, EQ_TYPE> mapping;

    // Existing constructors...
    EpistemicQuery() = default; // Add default constructor

    EpistemicQuery(char query_key, std::string agents_str, std::string content) {
        auto it = mapping.find(query_key);
        if (it != mapping.end()) {
            this->eq_type = it->second;
            this->header_str = std::string(1, query_key); // Convert char to string
            this->agents_str = agents_str;
            this->q_content = content;
        } else {
            // Default values in case key is not found
            this->q_content = content;
        }
    }

    EpistemicQuery(std::string content) {
            this->q_content = content;
    }

    EpistemicQuery(EQ_TYPE eq_type, std::string agents_str, std::string content, PDDL_TERNARY value) {
            this->eq_type = eq_type;
            this->agents_str = agents_str;
            this->content_value[content] = value;
    }
	
	 // Add a toString() function
    std::string toString() const {
        std::string str;
        str += "Header: " + header_str + ", ";
        str += "Agents: " + agents_str + ", ";
        str += "Content: " + q_content;
        return str;
    }
};

std::unordered_map<char, EQ_TYPE> EpistemicQuery::mapping = {
    {'k', EQ_TYPE::KNOWLEDGE},
    {'s', EQ_TYPE::SEEING},
    {'b', EQ_TYPE::BELIEF}
    // Add other mappings here
};
