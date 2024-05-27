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
		str_domain = "BBL\n\n"
		str_domain += "STATE DESCRIPTOR:\n"
		
		str_domain += "agent:var_type\n"
		
		str_domain += "dir(agent):pred_type\n"
		str_domain += "x(agent):pred_type\n"
		str_domain += "y(agent):pred_type\n"
		str_domain += "v(agent):pred_type\n"
		
		str_domain += "a:agent\n"
		str_domain += "b:agent\n"
		str_domain += "c:agent\n"
		
		str_domain += "\nACTION: turn_clockwise(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( dir(x) += 1 )\n"
		
		str_domain += "\nACTION: turn_counter_clockwise(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( dir(x) -= 1 )\n"
		

		f_domain = open("domain.txt", "w")
		f_domain.write(str_domain)
		f_domain.close()
			
		random.seed(1007)
		
		# INSTANCES
		instance_id = 1
		for i in range(from_nth,to_nth+1):
			# Problem name
			str_problem = "BBL-" + str(i) + "\n"
					
			# Objects
			str_problem += "\nOBJECTS:\n"
			str_problem += "a:agent\n"
			str_problem += "b:agent\n"
			str_problem += "c:agent\n"
			
			# Initial state
			str_problem += "\nINIT:\n"
			ir = random.randint(0,4)
			
			str_problem += "( dir(a) = 0 )\n"
			str_problem += "( dir(b) = 0 )\n"
			str_problem += "( x(a) = 3 )\n"
			str_problem += "( x(b) = 2 )\n"
			str_problem += "( x(c) = 1 )\n"
			str_problem += "( y(a) = 3 )\n"
			str_problem += "( y(b) = 2 )\n"
			str_problem += "( y(c) = 1 )\n"
			str_problem += "( v(c) = 1 )\n"
			# Compute		
				
			# Goal condition
			str_problem += "\nGOAL:\n"
			#str_problem += "( peeking(a) = 0 )\n"
			#str_problem += "( peeking(b) = 0 )\n"
			#str_problem += "( face(c) = 0 )\n"

			# EGoal condition
			str_problem += "\nEGOAL:\n"
			str_problem += "( b [a] ( v(c) = 1 ) @ 1 )"                  
			#print( str_problem )
			f_problem=open( out_folder + str( instance_id ) + ".txt","w")
			f_problem.write( str_problem )
			f_problem.close()
			instance_id = instance_id + 1
	
		sys.exit( 0 )
	
if __name__ == "__main__":
	main()
