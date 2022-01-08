COMPILER=clang++

CXXFLAGS = -Werror -Wextra -Wall -std=c++98

HEADERS = map.hpp \
          vector.hpp \
          stack.hpp \
          check_utils.hpp \
          ft_pair/pair.hpp \
          ft_meta_cpp/enable_if \
          ft_meta_cpp/enable_if.hpp \
          ft_meta_cpp/iterator_traits.hpp \
          ft_meta_cpp/lexicographical_compare.hpp \
          ft_meta_cpp/reverse_iterator.hpp \
          ft_meta_cpp/type_traits.hpp

all: ft_test stl_test

clear:
	rm -rf stl_test ft_test

fclear: clear
	rm -rf stl_main.o ft_main.o

re: fclear all

.PHONY: all clear fclear re

stl_main.o: main.cpp $(HEADERS)
	$(COMPILER) -c main.cpp $(CXXFLAGS) -D__TEST__STD__ -o stl_main.o

ft_main.o: main.cpp $(HEADERS)
	$(COMPILER) -c main.cpp $(CXXFLAGS) -o stl_main.o

ft_test: ft_main.o
	$(COMPILER) ft_main.o -o ft_test

stl_test: stl_main.o
	$(COMPILER) stl_main.o -o stl_test

