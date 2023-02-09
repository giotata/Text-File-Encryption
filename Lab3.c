#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

unsigned long file_size(char *file_name){
	FILE *ptr;
	ptr = fopen(file_name, "rb");
	
	if (ptr == NULL){
		perror("FILE DOES NOT EXIST");
		exit(1);
	}

	fseek(ptr, 0, SEEK_END);
	unsigned long size = ftell(ptr);
	fclose(ptr);	
	
	return size;
}

void make_rand_key(unsigned long length, char *key){
	srand(time(NULL));
	for(int i = 0; i < length; i++){
		int random = rand()%255;
		key[i] = (char)random;
	}	
}

char* read_file(unsigned long *size, char *file_name){
	FILE *ptr;
	ptr = fopen(file_name, "rb");
	
	if (ptr == NULL){
		perror("FILE DOES NOT EXIST");
		exit(1);
	}

	char *buffer = malloc(sizeof(char)*(*size+1));
	fread(buffer, *size+1, 1, ptr);

	fclose(ptr);
	return buffer;
}

int file_write(unsigned long *size, char *output, char *file_name){
	FILE *new;
	new = fopen(file_name, "wb");
	char *buffer = malloc(sizeof(char)*(*size+1));
	fwrite(output, *size, 1, new);
	fclose(new);
	free(buffer);

	return 0;
}

void encrypt(char *clear, char *key, char *cipher){
	unsigned long size = file_size(clear);
	char* buffer = read_file(&size, clear);		
	char* ky = malloc(sizeof(char)*(size+1));
	make_rand_key(size, ky);
	file_write(&size, ky, key);	

	for(int i=0; i<size; i++){
		char xor = buffer[i]^ky[i];	
		buffer[i] = xor;
	}
		
	file_write(&size, buffer, cipher);
	free(buffer);
	free(ky);
}

void decrypt(char *clear, char *key, char *cipher){
	unsigned long ci_size = file_size(cipher);
	char* ci_buffer = read_file(&ci_size, cipher);
	
	unsigned long ky_size = file_size(key);
	char* ky_buffer = read_file(&ky_size, key);

	for(int i=0; i<ci_size; i++){
		ci_buffer[i] = ci_buffer[i]^ky_buffer[i];
	}
	
	file_write(&ci_size, ci_buffer, clear);
	free(ci_buffer);
	free(ky_buffer);
}	

int main(){	
	int done = 0;
	int i;
	char clear[30];
	char key[30];
	char cipher[30];
	while(!done){
		printf("Encrypt: 1\nDecrypt: 2\nExit: 3\n");
		scanf("%d", &i);
		switch(i){
			case 1:
				printf("ENTER CLEAR TEXT FILE NAME\n");
				scanf("%s",clear);
				printf("ENTER KEY FILE NAME\n");
				scanf("%s",key);	
				printf("ENTER CIPHER FILE NAME\n");
				scanf("%s",cipher);
	
				encrypt(clear, key, cipher);
				printf("ENCRYPTION SUCCESS\n");
				break;
			case 2:
				printf("ENTER CLEAR TEXT FILE NAME\n");
				scanf("%s",clear);
				printf("ENTER KEY FILE NAME\n");
				scanf("%s",key);	
				printf("ENTER CIPHER FILE NAME\n");
				scanf("%s",cipher);

				decrypt(clear, key, cipher);
				printf("DECRYPTION SUCCESS\n");
				break;
			case 3:
				printf("EXITING\n");
				done = 1;
				break;
			default:
				printf("ENTER A VALID OPTION\n");
				break;
		}
	}

	return 0;
}
