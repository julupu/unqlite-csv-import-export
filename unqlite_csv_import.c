#include "unqlite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELENGTH 4096

void usage(char* s){
	printf("%s database list\n", s);
}

void handle_db_error(unqlite* db, int rc){
	const char *error_buf;
	int len;
	unqlite_config(db, UNQLITE_CONFIG_ERR_LOG, &error_buf, &len);
	if( len > 0 ){
		puts(error_buf);
	}
	if( rc != UNQLITE_BUSY && rc != UNQLITE_NOTIMPLEMENTED ){
		unqlite_rollback(db);
	}
}


int main(int argc, char **argv){

	int rc;
	FILE* list;
	unqlite *db;
	char line_buf[LINELENGTH];

	if(argc < 3){
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = unqlite_open(&db, argv[1], UNQLITE_OPEN_CREATE);
	if( rc != UNQLITE_OK ){
		printf("Couldn't open database\n");
		exit(EXIT_FAILURE);
	}

	list = fopen(argv[2], "r");
	line_buf[LINELENGTH-2]='\n';
	while(fgets(line_buf, LINELENGTH, list) != NULL){
		if(line_buf[LINELENGTH-1] == '\0' && line_buf[LINELENGTH - 2] != '\n'){
			printf("Couldn't parse line, too long\n");
			char c;
			while((c = fgetc(list)) != '\n' && c != EOF){
				continue;
			}
			line_buf[LINELENGTH-2]='\n';
			continue;
		}

		char *key = strtok(line_buf, ";");
		printf("Key: %s\t", key);

		char* value;
		value = strtok(NULL, ";");
		int len;
		len = strlen(value);
		if(value[len-1] = '\n'){
			value[len-1] = '\0';
			len -= 1; 
		}
		printf("Value: %s\n", value);
		rc = unqlite_kv_store(db, key, strlen(key), value, len);
		if( rc != UNQLITE_OK ){
			handle_db_error(db, rc);
		}
	}

	unqlite_close(db);
	exit(EXIT_SUCCESS);

}
