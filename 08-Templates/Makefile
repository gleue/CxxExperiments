CXX := $(shell brew --prefix gcc)/bin/g++-14 -std=c++20
RM := rm -f

templates: template.h template.cpp main.cpp
	$(CXX) $^ -o $@

clean:
	$(RM) templates
