cmake_minimum_required(VERSION 3.22)
project(final_unix_project C)

set(CMAKE_C_STANDARD 99)

add_executable(final_unix_project main.c delete_todo.c delete_todo.h
        make_directory.c make_directory.h make_todo.c make_todo.h read_from_txt.c
        read_from_txt.h todo_structure.h upload_subject.c upload_subject.h upload_to_txt.c
        upload_to_txt.h view_file.c view_file.h
        server.c server.h
        client.c client.h)
