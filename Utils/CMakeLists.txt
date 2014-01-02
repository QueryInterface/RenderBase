cmake_minimum_required (VERSION 2.8.11)

include_directories(../Includes)

add_library(Utils
    Utils.h
    Utils.cpp
	ErrorHandler.h
	ErrorHandler.cpp
	MathBasics.h)