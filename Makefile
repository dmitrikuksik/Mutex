CFLAGS=-Wall
OBJECTS=consumer producer main delete
LIBS=-lpthread

all: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS) $(CFLAGS)     

consumer: ./queue.h ./sem.h $(LIBS)
producer: ./queue.h ./sem.h $(LIBS)
main: ./queue.h ./sem.h $(LIBS)
delete: ./queue.h ./sem.h $(LIBS)
