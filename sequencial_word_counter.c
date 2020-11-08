#include <stdio.h>
#include <string.h>


char keywords[][10] = {"to","for", "fly","night"};
char text[] = "\
Blackbird singing in the dead of night \
Take these broken wings and learn to fly \
All your life \
You were only waiting for this moment to arise \
Blackbird singing in the dead of night \
Take these sunken eyes and learn to see \
All your life \
You were only waiting for this moment to be free \
Blackbird fly, blackbird fly \
Into the light of a dark black night \
Blackbird fly, blackbird fly \
Into the light of a dark black night \
Blackbird singing in the dead of night \
Take these broken wings and learn to fly \
All your life \
You were only waiting for this moment to arise \
You were only waiting for this moment to arise \
You were only waiting for this moment to arise";  

int main(int argc, char *argv[])
{
	 
	int number_of_keywords = (int)sizeof(keywords)/sizeof(keywords[0]); 
	int counters[number_of_keywords];
	
	for (int j = 0; j< number_of_keywords; j++){
		int keyword_size = strlen(keywords[j]); 
		char* keyword = keywords[j];
		int keyword_cursor = 0; 
		
		for (int i = 0; text[i]; i++)
		{
			if(text[i] == keyword[keyword_cursor])
			{ 
				if(keyword_cursor == 0 && (!text[i + keyword_size - 1] || text[i + keyword_size - 1] != keyword[keyword_size - 1]))
					i = i + keyword_size - 1;
				keyword_cursor++;
				if(keyword_cursor == keyword_size)
					counters[j]++;
			}
			else if(keyword_cursor)
				keyword_cursor = 0;		
		} 
	
	}
	for (int j = 0; j< number_of_keywords; j++){
		printf("%s: %d\n",keywords[j],counters[j]);
	}
	
	return 0;
}
