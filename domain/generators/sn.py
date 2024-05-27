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
		str_domain = "SN\n\n"
		str_domain += "STATE DESCRIPTOR:\n"
		
		str_domain += "agent:var_type\n"
		str_domain += "object:var_type\n"
		str_domain += "friended_a(agent):pred_type\n"
		str_domain += "friended_b(agent):pred_type\n"
		str_domain += "nota(agent):pred_type\n"
		str_domain += "post(object,agent):pred_type\n"
		str_domain += "secret(object):pred_type\n"
		
		str_domain += "b:agent\n"
		str_domain += "c:object\n"
		
		str_domain += "\nACTION: a_friends(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( friended_b(x) = 0 )\n"
		str_domain += "( nota(x) = 1 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( friended_a(x) = 1 )\n"
		str_domain += "( friended_b(x) = 1 )\n"
		
		str_domain += "\nACTION: a_unfriends(x:agent)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( friended_b(x) = 1 )\n"
		str_domain += "( nota(x) = 1 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( friended_a(x) = 0 )\n"
		str_domain += "( friended_b(x) = 0 )\n"

		str_domain += "\nACTION: a_post(x:agent,z:object)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( friended_b(x) = 1 )\n"
		str_domain += "( post(z,x) = 0 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( post(z,x) = 1 )\n"


		str_domain += "\nACTION: a_unpost(x:agent,z:object)\n"
		str_domain += "TYPE: memory\n"
		str_domain += "PRECONDITIONS:\n"
		str_domain += "( friended_b(x) = 1 )\n"
		str_domain += "( post(z,x) = 1 )\n"
		str_domain += "EFFECTS:\n"
		str_domain += "( post(z,x) = 0 )\n"
		
		f_domain = open("domain.txt", "w")
		f_domain.write(str_domain)
		f_domain.close()
			
		random.seed(1007)
		
		# INSTANCES
		instance_id = 1
		for i in range(from_nth,to_nth+1):
			# Problem name
			str_problem = "SN-" + str(i) + "\n"
					
			# Objects
			str_problem += "\nOBJECTS:\n"
			str_problem += "a:agent\n"
			str_problem += "b:agent\n"
			str_problem += "c:agent\n"
			str_problem += "d:object\n"
			
			# Initial state
			str_problem += "\nINIT:\n"
			ir = random.randint(0,1)
			str_problem += "( nota(b) = " + str(ir) + " )\n"
			ir = random.randint(0,1)
			str_problem += "( nota(c) = " + str(ir) + " )\n"
			ir = random.randint(0,1)
			str_problem += "( friended_a(b) = " + str(ir) + " )\n"
			str_problem += "( friended_b(b) = " + str(ir) + " )\n"
			ir = random.randint(0,1)
			str_problem += "( friended_a(c) = " + str(ir) + " )\n"
			str_problem += "( friended_b(c) = " + str(ir) + " )\n"
			
			str_problem += "( post(d,b) = 0 )\n"
			str_problem += "( post(d,c) = 0 )\n"
			str_problem += "( secret(d) = 1 )\n"

			# Compute		
				
			# Goal condition
			str_problem += "\nGOAL:\n"
			#str_problem += "( peeking(a) = 0 )\n"
			#str_problem += "( peeking(b) = 0 )\n"
			#str_problem += "( face(c) = 0 )\n"

			# EGoal condition
			str_problem += "\nEGOAL:\n"
			str_problem += "( b [a] ( secret(c) = 1 ) @ 1 )"
			#print( str_problem )
			f_problem=open( out_folder + str( instance_id ) + ".txt","w")
			f_problem.write( str_problem )
			f_problem.close()
			instance_id = instance_id + 1
	
		sys.exit( 0 )
	
if __name__ == "__main__":
	main()
