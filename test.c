#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common/common.h"
#include "lex/pl0_lex.h"

int l_num = 1;
int char_count = 0;

void show_help(const char * arg0)
{
	printf("Usage: %s <src>\n\n", arg0);
}

//int main(int argc, char * argv[])
int main()
{
	/* Parse arguments */
	/*if (argc != 2)
	{
		show_help(argv[0]);
		exit(-1);
	}*/
	char filename[100];
    scanf("%s",filename);
	fin = fopen(filename, "rb");
	/*if (!fin)
	{
		printf("Fatal: Cannot open file for reading: %s\n", argv[1]);
		exit(-1);
	}*/
    if (!fin)
    {
        printf("Fatal: Cannot open file for reading:\n");
        exit(-1);
    }

	/* Start */
	printf("=== This is the program for PL/0 testing ===\n");
	
	/* Setup */
	compiler = PL0Compiler_create();

	/* Lexical Analysis */
	printf("--- Lexical Analysis testing ---\n");
	PL0Lex * lex = compiler->lex;
	while (PL0Lex_get_token(lex))
	{
		assert(lex->last_token_type != TOKEN_NULL);
		assert(lex->last_token_type != TOKEN_RESWORDS);
		assert(lex->last_token_type != TOKEN_SYMBOL);
		if (lex->last_token_type == TOKEN_IDENTIFIER)
		{
			printf("Identifier: %s\n", lex->last_id);
		}
		else if (lex->last_token_type == TOKEN_NUMBER)
		{
			printf("Number: %d\n", lex->last_num);
		}
		else if (lex->last_token_type > TOKEN_RESWORDS)
		{
			printf("Reserved word: %s\n", TOKEN_RESERVED_WORDS[lex->last_token_type - TOKEN_RESWORDS - 1]);
		}
		else
		{
			printf("Symbol: %s\n", TOKEN_SYMBOLS[lex->last_token_type - TOKEN_SYMBOL - 1]);
		}
		printf("The line number:%d, the begin position:%d, the end position:%d\n", lex->line_num, lex->line_begin, lex->line_end);

	}

	printf("--- Lexical Analysis testing end. ---\n");

	/* Cleanup */
	fclose(fin);

	/* Finish */
	printf("=== Normal end for testing. ===\n");
	return 0;
}
