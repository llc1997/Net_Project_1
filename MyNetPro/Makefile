OBJ += main.o
OBJ += arp_pthread.o
OBJ += arp_link.o
OBJ += ip_pthread.o
OBJ += ip_link.o
OBJ += key_pthread.o
OBJ += get_interface.o
OBJ += main_child.o
<<<<<<< HEAD
OBJ += route_link.o
=======
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab

FLAGS = -Wall
CC = gcc

router:$(OBJ)  
	$(CC) $(OBJ) -o $@ $(FLAGS) -lpthread
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)
.PHONY:clean
clean:
	rm router *.o -rfv	
