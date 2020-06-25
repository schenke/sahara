all:
	gcc -std=c++11 -O3 src/Main.cpp -o Sahara.exe $(shell gsl-config --cflags) -I/usr/local/include $(shell gsl-config --libs) -lm  -lstdc++ 
#gcc -std=c++11 -O3 src/Main.cpp -o Sahara.exe -I/usr/local/boost_1_73_0/include/ -I/usr/local/include -L/usr/local/boost_1_73_0/lib/ -L/usr/local/include -lm -lgsl -lstdc++ -lboost_filesystem -lboost_program_options -lboost_system -lboost_regex
