# Direttive del makefile che non sono associate ad alcun nome di file
.PHONY: all math ped

all:
	@ gcc main.c
	@ ./a.out
math:
	@ gcc main.c -lm
	@ ./a.out
ped:
	@ gcc main.c -ansi -Wall -W -pedantic -std=c89
	@ ./a.out
debug:
	@ gcc main.c -g
	@ gdb --args ./a.out
liste:
	@ gcc lista.c mioprog.c -o out
	@ ./out
