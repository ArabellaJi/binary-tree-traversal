# Makefile for CPS 222 Project 3

# Google Test must be cloned and built.  If it's not in the same parent
# directory (..), change GTEST_ROOT to the actual location.
GTEST_ROOT = ../googletest/googletest
GTEST_MAIN = $(GTEST_ROOT)/make/libgtest_main.a

# Compiler flags: extra warnings (-Wall), debugging (-g), and Google Test
# Use C 2011 (required by Google Test, and a good idea anyway)
CXXFLAGS = -std=c++11

# -Wall and -Wextra requests more warnings -- fix them for clean code
# -g compiles for debugging
CPPFLAGS = -I $(GTEST_ROOT)/include -Wall -Wextra -g

# Default target
all:	project3_main test

# Build and run the unit tests
test:	project3_test
	./$<

project3_test:	project3_test.o project3.o threaded_bst_basic.o $(GTEST_MAIN)
	g++ -o $@ $^ -pthread

# Build program for manual testing
project3_main: threaded_bst_main.o project3.o threaded_bst_basic.o
	g++ -o $@ $^

# Dependencies
project3.o:	threaded_bst.h
threaded_bst_basic.o:	threaded_bst.h
threaded_bst_main.o:	threaded_bst.h
