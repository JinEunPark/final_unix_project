Subject_Management : delete_todo.o make_directory.o make_todo.o read_from_txt.o upload_subject.o upload_to_txt.o view_file.o main.o client.o server.o
	gcc -o Subject_Management delete_todo.o make_directory.o make_todo.o read_from_txt.o upload_subject.o upload_to_txt.o view_file.o main.o client.o server.o

delete_todo.o : delete_todo.c
	gcc -c -o delete_todo.o delete_todo.c

make_directory.o : make_directory.c
	gcc -c -o make_directory.o make_directory.c

make_todo.o : make_todo.c
	gcc -c -o make_todo.o make_todo.c

read_from_txt.o : read_from_txt.c
	gcc -c -o read_from_txt.o read_from_txt.c

upload_subject.o : upload_subject.c
	gcc -c -o upload_subject.o upload_subject.c

upload_to_txt.o : upload_to_txt.c
	gcc -c -o upload_to_txt.o upload_to_txt.c

view_file.o : view_file.c
	gcc -c -o view_file.o view_file.c
client.o : client.c
	gcc -c -o client.o client.c
server.o : server.c
	gcc -c -o server.o server.c

clean:
	rm *.o Subject_Management