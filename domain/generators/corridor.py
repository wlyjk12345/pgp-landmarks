#!/usr/bin/env python3
# coding: utf-8

import sys
import numpy as np
import random

def main():
	try:
		from_nth = int( sys.argv[1] )
		to_nth = int( sys.argv[2] )
		out_folder = sys.argv[3]
	except:
		print("Usage: ")
		print(sys.argv[ 0 ] + " <from_n> <to_n> <outfolder>")
		print(sys.argv[ 0 ] + " 2 6 tmp/")
		sys.exit(-1)
		
	# DOMAIN
	str_domain = "CORRIDOR\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	
	str_domain += "agents:var_type\n"
	str_domain += "objects:var_type\n"
	str_domain += "agent_at(agents):pred_type\n"
	str_domain += "secret_at(objects):pred_type\n"
	str_domain += "sensed(objects):pred_type\n"
	str_domain += "secret(objects):pred_type\n"
	str_domain += "a:agents\n"
	str_domain += "b:agents\n"
	str_domain += "s:objects\n"
	
		
	str_domain += "\nACTION: vector-right(x:agents)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( agent_at(x) + 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( agent_at(x) += 1 )\n"
	
	str_domain += "\nACTION: vector-left(x:agents)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( agent_at(x) - 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( agent_at(x) -= 1 )\n"
	
	str_domain += "\nACTION: sense(x:agents,y:objects)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( secret_at(y) = agent_at(x) )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( sensed(y) = 1 )\n"
	
	str_domain += "\nACTION: shout(x:agents,y:objects)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( sensed(y) = 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( shared(y) = agent_at(x) )\n"
	str_domain += "( secret(y) = 1 )\n"	
	
	str_domain += "\nACTION: shout_lie(x:agents,y:objects)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( sensed(y) = 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( shared(y) = agent_at(x) )\n"
	str_domain += "( secret(y) = -1 )\n"
	

	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
		

	random.seed(1007)
	
	# INSTANCES
	for i in range(from_nth,to_nth+1):
		# Problem name
		str_problem = "CORRIDOR-" + str(i) + "\n"
		
		# Compute (pre)
		vi = random.randint(0,3)
		vgi = random.randint(0,3)
		i = random.randint(0,3)
		j = random.randint(0,3)
		while vgi == vi:
			vgi = random.randint(0, 3)
			
		# Objects
		str_problem += "\nOBJECTS:\n"
		str_problem += "a:agents\n"
		str_problem += "b:agents\n"		
		str_problem += "s:objects\n"
		
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( agent_at(a) = " + str(vi) + " )\n"
		str_problem += "( agent_at(b) = " + str(vgi) + " )\n"
		str_problem += "( secret_at(s) = " + str(vgi) + " )\n"		
		str_problem += "( sensed(s) = " + str(i) + " )\n"	
		str_problem += "( secret(s) = " + str(j) + " )\n"	
		str_problem += "( shared(s) = " + str(vgi) + " )\n"		
		
		# Compute		
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		
		str_problem += "\nEGOAL:\n"
		str_problem += "( b [a] b [b] ( secret(s) = 1 ) @ 1 )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( i+1-from_nth ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
	
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
