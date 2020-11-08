#include <stdio.h>
#include <stdlib.h> 
 

char keywords[][10] = {"nome","meu", "test"};
char text[] = "Meu nome e Juliana, o nome do meu namorado e Gabriel e o nome do meu pai e Hiltonom";  

int main(int argc, char *argv[])
{
	
	int number_of_keywords = (int)sizeof(keywords)/sizeof(keywords[0]); 
	for (int j = 0; j< number_of_keywords; j++){ 
		//int keyword_size = (int)sizeof(keyword)/sizeof(keyword[0])-1; 
		printf("keyword %s\n", keywords[j]);
		printf("length %d\n",(int)sizeof(keywords[j]));
		
	} 
	
	
	int keyword_cursor = 0;
	int counter = 0;
	 
	int keyword_size = 4; //(int)sizeof(keyword)/sizeof(keyword[0])-1;
	
	
	for (int i = 0; text[i]; i++)
	{
		if(text[i] == keywords[keyword_cursor])
		{ 
			if(keyword_cursor == 0 && (!text[i + keyword_size - 1] || text[i + keyword_size - 1] != keywords[keyword_size - 1]))
				i = i + keyword_size - 1;
			keyword_cursor++;
			if(keyword_cursor == keyword_size)
				counter++;
		}
		else if(keyword_cursor)
			keyword_cursor = 0;		
		
	} 
	
	printf("Number of occurences: %d\n",counter);
	return 0;
}
