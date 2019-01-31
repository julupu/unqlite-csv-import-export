#include "unqlite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char* s){
	printf("%s database\n", s);
}

int print_callback(const void *data, unsigned int len, void *user_data /*unused */){
	printf("%.*s", len, data);
}

int main(int argc, char **argv){

	int rc;
	unqlite *db;
	unqlite_kv_cursor *cursor;
	unqlite_int64 data_len;


	if(argc < 2){
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = unqlite_open(&db, argv[1], UNQLITE_OPEN_READONLY | UNQLITE_OPEN_MMAP);
	if( rc != UNQLITE_OK ){
		printf("Couldn't open database\n");
		exit(EXIT_FAILURE);
	}

	rc = unqlite_kv_cursor_init(db, &cursor);
	if( rc != UNQLITE_OK ){
		printf("Cursor error\n");
		exit(EXIT_FAILURE);
	}
	rc = unqlite_kv_cursor_first_entry(cursor);
	if( rc != UNQLITE_OK ){
		printf("Cursor error\n");
		exit(EXIT_FAILURE);
	}

	while( unqlite_kv_cursor_valid_entry(cursor) ){
		unqlite_kv_cursor_key_callback(cursor, print_callback, 0);
		printf(";");
		unqlite_kv_cursor_data_callback(cursor, print_callback, 0);
		printf("\n");
		unqlite_kv_cursor_next_entry(cursor);
	}
	unqlite_kv_cursor_release(db, cursor);
	unqlite_close(db);

	exit(EXIT_SUCCESS);

}
