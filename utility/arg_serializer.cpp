/*
 * This file contains C-style serializers and deserializers (that rely on C++ classes)
 * for argv arrays. The functions contained in this file are not thread safe
 * @Author: D. Alvarez 
 */
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

/*
 * This function takes argc and argv, and returns a serialized version of 
 * argv to be reconstructed later. The serialized array is a bytecopiable
 * array that contains the following sequence:
 * argv[0],argv[1],....
 *
 * Note that if any of the argv values contains "," it will cause problems.
 *
 * This probably can be solved more elegantly using C++ classes, but since
 * this is are simple char arrays we can keep it simple. Also my C++-fu is
 * weak, and there is a significant number of things that could have been
 * done better.
 */
char * arg_serializer(int argc, const char **argv){

		char *arg_ser;
		int arg_ser_len;
		int aux_i = 0;

		arg_ser_len=0;
				
		aux_i=strlen(argv[0]);

		arg_ser_len+=aux_i; /* The lenght of the value */

		/* Compute required lenght. Each value has two components: size and argument */
		for(int i = 1; i<argc; i++){
				arg_ser_len+=1; /* The comma to separate values */

				aux_i=strlen(argv[i]);

				arg_ser_len+=aux_i; /* The lenght of the value */
		}

		/* Allocate the new serialized array */
		arg_ser = (char *)malloc(sizeof(char)*arg_ser_len+1);
		if(arg_ser == NULL){
				std::cerr << "FATAL ERROR: Cannot allocate " << sizeof(char)*arg_ser_len << " bytes for the serialization of argv. Exiting....\n";
				exit(-1);
		}

		/* Construct serialized array */
		aux_i=0;

		strcpy((char *)arg_ser+aux_i,argv[0]); /* The value */
		aux_i+=strlen(argv[0]);

		for(int i = 1; i<argc; i++){
				strcpy((char *)arg_ser+aux_i,","); /* The comma to separate values */
				aux_i++;
			
				strcpy((char *)arg_ser+aux_i,argv[i]); /* The value */
				aux_i+=strlen(argv[i]);
		}

		return arg_ser;

}


/*
 * This function takes a serialized version of argv and returns argc and argv.
 *
 * Note that if any of the argv values contains "," it will cause problems.
 *
 * This probably can be solved more elegantly using C++ classes, but since
 * this is are simple char arrays we can keep it simple. Also my C++-fu is
 * weak, and there is a significant number of things that could have been
 * done better.
 *
 * Also, to make it completely C, maybe we can change the code to use strtok
 */
void arg_deserializer(const char *arg_ser, int *argc, char ***argv){

		int n_arg = 0;
		int i = 0;
		std::string aux_s = arg_ser;
		std::istringstream buf(aux_s);

		/* Iterate over arguments to calculate its number */
		for(std::string token; getline(buf, token, ','); ){
				i++;
		}
	
		*argc = i;

		/* Allocate space for the argument pointers */
		*argv=(char **)malloc(sizeof(char *)*i);
		if(*argv == NULL){
				std::cerr << "FATAL ERROR: Cannot allocate " << sizeof(char *)*i << " bytes for the deserialization of argv. Exiting....\n";
				exit(-1);
		}
	
		/* Iterate over arguments to deserialize */
		i = 0;
		buf.str(aux_s);
		buf.clear();
		for(std::string token; getline(buf, token, ','); ){
				/* Allocate space for new argument */
				(*argv)[i]=(char *)malloc(sizeof(char)*token.size());
				if((*argv)[i] == NULL){
						std::cerr << "FATAL ERROR: Cannot allocate " << sizeof(char)*token.size() << " bytes for the deserialization of argv. Exiting....\n";
						exit(-1);
				}
				std::strcpy((*argv)[i],token.c_str());
				i++;
		}

		printf("%s\n",(*argv)[i-1]);

		return;

}
