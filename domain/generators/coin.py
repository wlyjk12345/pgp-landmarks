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
		str_domain = "COIN\n\n"
		str_domain += "STATE DESCRIPTOR:\n"
		
		str_domain += "agent:var_type\n"
		str_domain += "object:var_type\n"
		str_domain += "peeking(agent):pred_type\n"
		str_domain += "face(object):pred_type\n"
		str_domain += "a:agent\n"
		str_domain += "b:agent\n"
		str_domain += "c:object\n"
		
		str_domain += "\nACTION: single_peek(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( peeking(x) = 0 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( peeking(x) = 1 )\n"
		
		str_domain += "\nACTION: return(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( peeking(x) = 1 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( peeking(x) = 0 )\n"

		str_domain += "\nACTION: turn_coin_true(y:object)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( face(y) = 0 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( face(y) = 1 )\n"


		str_domain += "\nACTION: turn_coin_false(y:object)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( face(y) = 1 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( face(y) = 0 )\n"

		f_domain = open("domain.txt", "w")
		f_domain.write(str_domain)
		f_domain.close()
			
		random.seed(1007)
		
		# INSTANCES
		instance_id = 1
		for i in range(from_nth,to_nth+1):
			# Problem name
			str_problem = "COIN-" + str(i) + "\n"
					
			# Objects
			str_problem += "\nOBJECTS:\n"
			str_problem += "a:agent\n"
			str_problem += "b:agent\n"
			str_problem += "c:object\n"
			
			# Initial state
			str_problem += "\nINIT:\n"
			ir = random.randint(0,1)
			str_problem += "( face(c) = " + str(ir) + " )\n"
			
			str_problem += "( peeking(a) = 0 )\n"
			str_problem += "( peeking(b) = 0 )\n"

			# Compute		
				
			# Goal condition
			str_problem += "\nGOAL:\n"
			#str_problem += "( peeking(a) = 0 )\n"
			#str_problem += "( peeking(b) = 0 )\n"
			#str_problem += "( face(c) = 0 )\n"

			# EGoal condition
			str_problem += "\nEGOAL:\n"
			str_problem += "( b [a] ( face(c) = 1 ) @ 1 )"

			#print( str_problem )
			f_problem=open( out_folder + str( instance_id ) + ".txt","w")
			f_problem.write( str_problem )
			f_problem.close()
			instance_id = instance_id + 1
	
		sys.exit( 0 )
	
if __name__ == "__main__":
	main()
