CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp
PROG = abc

OPENCV = `pkg-config opencv --cflags --libs`
MYSQL = `mysql_config --include --libs`
MYLASP = -lmysqlclient
MYLIBS = $(MYSQL) $(MYLASP)
LASPICAM = -lraspicam -lraspicam_cv
LIBS = $(OPENCV) $(LASPICAM)
LDLIBS = -lwiringPi

.PHONY: all clean

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(LDLIBS) $(MYLIBS)

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG) 
