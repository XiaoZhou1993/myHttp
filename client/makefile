CC = gcc

XX = g++

CFLAGS =-W -fPIC -std=c++11

target =./run_client

includes =-I ./ \
		  -I ../ \
		  -I/home/zxy/code/myHttp/client/include

#the a file path
lib_path = -L./  \
		   -L../ \
		   -L/home/zxy/code/myHttp/client/lib

#link lib.a file
lib = -lgtest -lpthread

user_dir = ./ \
		   ./src

files =$(foreach d,$(user_dir),$(wildcard $(d)/*.cpp))

objects =$(patsubst %.cpp,%.o,$(files))

$(target): $(objects)
	$(XX) -o $@ $^ $(lib) $(lib_path)

%.o: %.cpp
	$(XX) $(CFLAGS) $(includes) -c  $^ -g -o $@

.PHONY : clean
clean :
	rm -rf $(target)
	rm -rf $(objects)
