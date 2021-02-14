# Andreu Servera Master's Project
# makefile for building ReadFile

all: ReadFile

ReadFile: ReadFile.c utils.o
	#(toni) gcc -o readfile $(mysql_config --cflags) $(mysql_config --libs) ReadFile3.c -lm  -lmysqlclient -lm
	# $@ matches the target, $< matches the first dependent
	gcc utils.o -o $@ $(mysql_config --cflags) $(mysql_config --libs) $< -lm -lmysqlclient -lm

utils.o: utils.c
	gcc -c $<

clean:
	rm ReadFile utils.o




#************************
# makefile structure vvv
#************************

#all: #final output

#target1: #prerequisite1 prerequisiteN
	# command ...
	
#target2 (prerequisite1): #prerequisite1 prerequisiteN
	# command ...

#clear:
	# rm whatever

#************************
# makefile structure ^^^
#************************
