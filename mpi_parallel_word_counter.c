#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int tag = 1;
char *text;
char keywords[][10] = {
		"a",
		"as",
		"at",
		"be",
		"do",
		"is",
		"in",
		"it",
		"of",
		"he",
		"on",
		"to",
		"but",
		"for",
		"you",
		"not",
		"and",
		"the",
		"was",
		"God",
		"with",
		"this",
		"that",
		"have",
		"LORD",
		"said",
		"faith",
		"Jesus",
		"waters",
		"devils",
		"thyself",
		"neighbour",
};

int read_text_file()
{
	int c;
	FILE *file;
	file = fopen("text.txt", "r");

	if (file)
	{
		int text_size = 0;
		fseek(file, 0L, SEEK_END);
		text_size = ftell(file);

		text = (char *)malloc(text_size * sizeof(char));
		rewind(file);
		fread(text, sizeof(char), text_size, file);
		fclose(file);

		return text_size;
	}

	return -1;
}

void print_results(int *counters, int number_of_keywords)
{
	printf("\033[0;32m");
	printf("\n #####################################");
	printf("\n %-14s%8s%15s", "# ", "Open MPI", " #");
	printf("\n %-11s%15s%11s", "# ", "Keyword Counter", " #");
	printf("\n #####################################");
	printf("\n # %-22s%11s #", "Keyword", "Occurrences");
	printf("\n #####################################");
	for (int j = 0; j < number_of_keywords; j++)
		printf("\n # %-22s%11d #", keywords[j], counters[j]);
	printf("\n #####################################\n\n");
	printf("\033[0m");
}

int isEndOfWord(char x)
{
	return x == ' ' || x == ',' || x == '.' || x == ':' || x == ';' || x == '!' || x == '?' || x == ')' || x == '\'' || x == '\n';
}

int isStartOfWord(char x)
{
	return x == ' ' || x == '\n' || x == '(';
}

int main(int argc, char *argv[])
{
	//Initialize MPI
	MPI_Init(NULL, NULL);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int text_size = read_text_file();
	int number_of_keywords = (int)sizeof(keywords) / sizeof(keywords[0]);
	int number_of_keywords_per_proccess = (number_of_keywords / world_size) + (number_of_keywords % world_size != 0);

	// initialize process counters array
	int counters[number_of_keywords_per_proccess];
	for (int i = 0; i < number_of_keywords_per_proccess; i++)
		counters[i] = 0;

	for (int j = world_rank; j < number_of_keywords; j = j + world_size)
	{
		int keyword_size = strlen(keywords[j]);
		char *keyword = keywords[j];
		int keyword_cursor = 0;

		for (int i = 0; i < text_size; i++)
		{
			if (*(text + i) == keyword[keyword_cursor])
			{
				keyword_cursor++;
				if (keyword_cursor == keyword_size && isEndOfWord(*(text + i + 1)) && (i - keyword_size == 0 || isStartOfWord(*(text + i - keyword_size))))
				{
					counters[j / world_size]++;
				}
			}
			else if (keyword_cursor)
				keyword_cursor = 0;
		}
	}

	MPI_Status status;
	if (world_rank != 0)
	{
		MPI_Send(&counters, number_of_keywords_per_proccess, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Finalize();
	}
	else
	{
		// initialize all counters array
		int all_counters[number_of_keywords];
		for (int i = 0; i < number_of_keywords; i++)
			all_counters[i] = 0;

		// set world_rank 0 counters
		for (int i = 0; i < number_of_keywords; i = world_size + i)
			all_counters[i] = counters[i / world_size];

		// receive other processes counters
		for (int i = 1; i < world_size; i++)
		{
			//create an array to temporarily store other processes counters
			int *temp_array = malloc(number_of_keywords_per_proccess * sizeof(int));
			MPI_Recv(temp_array, number_of_keywords_per_proccess, MPI_INT, i, tag, MPI_COMM_WORLD, &status);

			// map other processes counters into all_counters
			for (int j = i; j < number_of_keywords; j = world_size + j)
				all_counters[j] = temp_array[j / world_size];

			free(temp_array);
		}

		print_results(all_counters, number_of_keywords);
		MPI_Finalize();
	}

	return 0;
}
