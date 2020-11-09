#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *text;

char keywords[][10] = {"key", "Jesus", "faith", "a", "the", "is"};

int read_text_file()
{
	int c;
	FILE *file;
	file = fopen("text.txt", "r");

	if (file)
	{
		int text_size = 0;
		while ((c = getc(file)) != EOF)
			text_size++;

		text = (char *)malloc(text_size * sizeof(char));
		rewind(file);
		for (int i = 0; (c = getc(file)) != EOF; i++)
			text[i] = c;
		fclose(file);

		printf("chars: %d\n", text_size);
		return text_size;
	}

	return -1;
}

void print_results(int *counters, int number_of_keywords)
{
	printf("\033[0;32m");
	printf("\n #####################################");
	printf("\n %-11s%15s%11s", "# ", "Keyword Counter", " #");
	printf("\n #####################################");
	printf("\n # %-22s%11s #", "Keyword", "Occurrences");
	printf("\n #####################################");
	for (int j = 0; j < number_of_keywords; j++)
	{
		printf("\n # %-22s%11d #", keywords[j], counters[j]);
	}
	printf("\n #####################################\n\n");
	printf("\033[0m");
}

int isEndOfWord(char x)
{
	return x == ' ' || x == ',' || x == '.' || x == ';' || x == '!' || x == '?' || x == ')';
}

int isStartOfWord(char x)
{
	return x == ' ' || x == '(';
}

int main(int argc, char *argv[])
{
	int number_of_keywords = (int)sizeof(keywords) / sizeof(keywords[0]);
	int counters[number_of_keywords];

	int text_size = read_text_file();

	for (int i = 0; i < number_of_keywords; i++)
	{
		counters[i] = 0;
	}

	for (int j = 0; j < number_of_keywords; j++)
	{
		int keyword_size = strlen(keywords[j]);
		char *keyword = keywords[j];
		int keyword_cursor = 0;

		for (int i = 0; i < text_size; i++)
		{
			char ch = *(text + i);
			if (*(text + i) == keyword[keyword_cursor])
			{
				if (keyword_cursor == 0 && ((i + keyword_size) > text_size || *(text + i + keyword_size - 1) != keyword[keyword_size - 1]))
					i = i + keyword_size - 1;
				keyword_cursor++;
				if (keyword_cursor == keyword_size && isEndOfWord(*(text + i + 1)) && (i - keyword_size == 0 || isStartOfWord(*(text + i - keyword_size))))
				{
					counters[j]++;
				}
			}
			else if (keyword_cursor)
				keyword_cursor = 0;
		}
	}

	print_results(counters, number_of_keywords);

	return 0;
}
