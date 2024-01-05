#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define N		(10)
#define BUFSIZE (10)


void error(const char *msg) {
    printf("%s", msg);
}



char *parse_int(char* c, int *value) {
	
	*value = 0;
	int stack[BUFSIZ];
	int num = 0;
	//stack = malloc(10*sizeof(int));

	while(*c && !isdigit(*c)) {
		c++;
	}

	if(!*c){
		return NULL;
	}

	while(*c ){ //&& isdigit(*c)
		
		//printf("hej");
		if (isdigit(*c)){
			*value = 10*(*value) + *c++ - '0';

		}
		else if (*c== '%' || *c == '/' || *c == '*' || *c == '+' || *c == '-') {
			*c++;
		}
		else {
			//printf("%d\n", *value);
			stack[num] = *value;


			//printf("%d\n", stack[num]);
			//printf("%d\n", num);
			*c++;
			*value=0;
			num++;
		}
			
		//*value = 10*(*value) + *c++ - '0';
	}

	return c;
}



int sum(char *line){
	int value, sum;

	sum = 0;



	while((line = parse_int(line, &value)) != NULL){
		//Spara alla tal i vektor'
		//spara alla operationer i vektor
		sum += value;
	}

	return sum;
}


int main(int argc, char *argv[])
{
	int c;

	FILE *input;
	FILE *output;


	//char buffer[BUFSIZ];
	char* buffer;
	buffer = calloc(BUFSIZ, sizeof(char));

	if(argc < 2){
		error(("Could not open file %s\n", argv[1]));
		return 1;
	}
		
		//error("You must give me a filename!\n");
		

	if((input = fopen(argv[1], "r")) == NULL){
		error(("Could not open file %s\n", argv[1]));
		return 1;
	}
		


	sprintf(buffer, "%s.out", argv[1]);

	if((output = fopen(argv[2], "w")) == NULL){
		error(("Could not open file %s\n", argv[2]));
		return 1;
	}	
	
	
	
	int line_nbr = 0;
	while (fgets(buffer, BUFSIZ, input)) {
		line_nbr++;
		fprintf(output, "%d: %d\n", line_nbr, sum(buffer));
		//printf("loop");
		
		//printf(buffer[line_nbr]);
	}
	
	//printf("Slut\n");

	fclose(input);
	fclose(output);


	//Loop
	/*
	while(c = getchar()){
		printf("%c\n",c);

		//if(isdigit(c)) 
			//Lägg ihop tal ex: 100


		

		if(c == 97) //a
			break;
	}*/
	//c = getchar();
	//printf("%c\n",c);


	return 0;
}
