#include <stdio.h>

int has_vowel(char* word)
{
	while(*word != '\0')
	{
		if(*word == 'a' || *word == 'e' || *word == 'i' || *word == 'o'
				|| *word == 'u')
			return 1;
	
		word++;
	}

	return 0;
}

int main()
{
	char word[100];
	FILE* fp = fopen("dict8.txt", "r");

	while(!feof(fp))
	{
		fscanf(fp, "%s\n", word);

		
		if(has_vowel(word))
			printf("%s\n", word);
	}


	fclose(fp);
	return 0;
}
