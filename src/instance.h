#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "common.h"
#include "state.h"
#include "condition.h"

#include <map>
#include <iostream>
#include "PDDL_TERNARY.h"

class Instance{
public:
	explicit Instance( const string &name = "", int instance_id = 0 ){
		_name = name;
		_init = nullptr;
		_instance_id = instance_id;
	}
	
	~Instance() {
    	delete _init;
		// Clearing the goal map	
		e_goal.clear();

		for( auto g : _goal ){
			delete g;
		}
		_goal.clear();

	}

	
	void setName( const string &name = "" ){
		_name = name;
	}
	
	void setInitialState( State *init ){
		_init = init;
		_init->setInstanceID( _instance_id );
	}

		void setGoalCondition( vector< Condition* > &goal ){
		_goal = goal;
	}
	
	void addCondition( Condition* cond ){
		_goal.push_back( cond );
	}
	
 	void setEGoal(const std::map<std::string, PDDL_TERNARY>& newGoal) {
        e_goal = newGoal;
    }

    // Add an individual goal
    void addEGoal(const std::string& condition, PDDL_TERNARY value) {
        e_goal[condition] = value;
    }
	
	string getName() const{
		return _name;
	}
	
	State* getInitialState(){
		return _init;
	}
	
	std::map<std::string, PDDL_TERNARY> getEGoal(){
		return e_goal;
	}

	vector< Condition* > getGoalCondition(){
		return _goal;
	}
	
	Condition* getCondition( int id ){
		return _goal[ id ];
	}

	void addObject( StateDescriptor *sd, const string &name, const string &obj_type ){
	    auto num_of_objects = sd->getStateVarBound( obj_type, _instance_id );
	    assert( obj_to_type.find( name ) == obj_to_type.end() );
	    obj_to_type[ name ] = obj_type;
	    obj_to_address[ name ] = num_of_objects;
	    sd->setStateVarBound( obj_type, _instance_id, num_of_objects+1 );
	}

	string getObjectType( const string &obj_name ) const{
	    auto it = obj_to_type.find( obj_name );
	    assert( it != obj_to_type.end() );
	    return it->second;
	}

	int getObjectAddress( const string &obj_name ) const{
	    auto it = obj_to_address.find( obj_name );
	    assert( it != obj_to_address.end() );
	    return it->second;
	}

	void setInstanceID( int instance_id ){
	    _instance_id = instance_id;
	    if( _init ) _init->setInstanceID( _instance_id );
	}

	int getInstanceID() const {
	    return _instance_id;
	}

	// Full info printing
	string toString() const{
		string ret = "[INSTANCE]: " + _name + "\n";

        ret += "\nOBJECTS:\n";
        for( const auto& it : obj_to_type ){
            auto it2 = obj_to_address.find(it.first);
            ret += (it.first) + ":" + (it.second) +" @" + to_string(it2->second) +"\n";
        }

		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString();

		ret += "\nGOAL:\n";
		for(auto g : _goal)
			ret += g->toString(true);

		ret += "\nEGOAL:\n";
		for (const auto& pair : e_goal) {
        	// Constructing a string representation of the key-value pair
        	ret += pair.first + ": " + PDDL_TERNARY_to_string(pair.second) + "\n";
		}

		return ret;
	}

	// Pretty printing
	string toString( StateDescriptor *sd ){
		string ret = "[INSTANCE]: " + _name + "\n";

		ret += "\nOBJECTS:\n";
		for( const auto& it : obj_to_type ){
		    ret += (it.first) + ":" + (it.second) +" @" + to_string(obj_to_address[ (it.first) ]) +"\n";
		}
		
		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString();

		ret += "\nGOAL:\n";
		for(auto g : _goal)
			ret += g->toString(true);

		ret += "\nEGOAL:\n";
		 for (const auto& pair : e_goal) {
        // Constructing a string representation of the key-value pair
			ret += pair.first + ": " + PDDL_TERNARY_to_string(pair.second) + "\n";
		}

		return ret;;
	}
	
private:
	string _name;
	State *_init;
	std::map<std::string, PDDL_TERNARY> e_goal;
	int _instance_id;
	map< string, string > obj_to_type;
	map< string, int > obj_to_address;
	vector< Condition* > _goal;
};

#endif
