all: adaemon.c
	g++ -c -I/usr/local/mysql/include/mysql -I/usr/include/mysql adaemon.c
	g++ -o adaemon adaemon.o -L/usr/local/mysql/lib/mysql -L/usr/lib/mysql -lmysqlclient
