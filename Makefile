filter:
	clang++ -std=c++14 -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter filter.cpp helpers.cpp
