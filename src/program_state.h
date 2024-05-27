#ifndef __PROGRAM_STATE_H__
#define __PROGRAM_STATE_H__

#include "common.h"
#include "state.h"
#include <iostream> 
#include <vector>

class ProgramState{
public:
	ProgramState(){
		_state = nullptr;
		_line = 0;

		_timestamp = 0; //
	}
	
	~ProgramState(){
		delete _state;
        // Delete each pointer in the vector
        for (auto ptr : _stateHistory) {
            delete ptr;
        }

        // Clear the vector if necessary
        _stateHistory.clear();
	}
	
	void setState( State *s ){
		_state = s;

		_stateHistory.push_back(s->copy()); // 将新状态推入历史记录
    	_timestamp += 1; // 更新当前时间戳;
	}
	
	void setLine( int l ){
		_line = l;
	}

	void setTimestamp(int timestamp){
    _timestamp = timestamp;
	}
	
	State* getState() const{
		return _state;
	}
	
	int getLine() const{
		return _line;
	}

	State* getStateAtTimestamp(int timestamp){
    if (timestamp >= 0 && static_cast<size_t>(timestamp) < _stateHistory.size()) {
        return _stateHistory[timestamp];
    }
    return nullptr; // 如果时间戳超出范围，返回nullptr
	}

	vector<State*>  getStateHistory(){
        return _stateHistory;
	}

	vector< int > asVector() const{
        vector< int > res( 1, _line );
        auto vars = _state->getStateVars();
        for( auto v : vars ){
            res.insert( res.end(), v.begin(), v.end() );
        }
        return res;
	}
	
	string toString() const{
		string ret = "[PROGRAM STATE]:\nLINE:" + to_string( _line ) + "\n";
		if( _state )
			ret += _state->toString();
		return ret;
	}

	string toString( StateDescriptor *sd ) const{
        string ret = "[PROGRAM STATE]:\nLINE:" + to_string( _line ) + "\n";
        if( _state )
            ret += _state->toString(sd);
        return ret;
	}


	vector<State*> _stateHistory; 
	int _timestamp; 
	
private:
	State *_state;
	int _line;
	//
	
    
};

#endif
