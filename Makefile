all:
	gcc -std=c++11 -O3 src/Main.cpp -o Sahara.exe -I/usr/local/include -L/usr/local/include -lm -lgsl -lstdc++ 
#gcc -std=c++11 -O3 src/Main.cpp -o Sahara.exe -I/usr/local/boost_1_73_0/include/ -I/usr/local/include -L/usr/local/boost_1_73_0/lib/ -L/usr/local/include -lm -lgsl -lstdc++ -lboost_filesystem -lboost_program_options -lboost_system -lboost_regex
