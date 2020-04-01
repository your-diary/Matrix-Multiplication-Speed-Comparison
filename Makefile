.RECIPEPREFIX := $(.RECIPEPREFIX) 
SHELL := /bin/bash
.DELETE_ON_ERROR :

.PHONY : all

all : calc.out calc_multithreaded.out

%.out : %.cpp
    g++ -O3 -pthread -o $@ $< $(CXXFLAGS)

