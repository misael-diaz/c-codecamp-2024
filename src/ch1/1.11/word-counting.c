#include <stdio.h>

// partial solution of problem 1.11 - word counting
// words are preceded and followed by spaces, start with a letter, and may contain digits
// the first and last words in the stream are handled specially
// have not tested this code thoroughly so there may be some inputs that cause it to fail

#define FALSE 0
#define TRUE 1

int main ()
{
	int c = EOF;
	int num_chars = 0;			// number of characters in the stream
	int num_words = 0;			// number of valid words
	int num_read_chars = 0;			// number of read characters in a word
	int first_char_flag = TRUE;		// first character in the stream flag
	int letter_start_flag = FALSE;		// word starts with a letter flag
	int whitespace_char_flag = FALSE;	// whitespace character flag
	int valid_chars_flag = FALSE;		// valid characters in word flag
	FILE *file = fopen("data.txt", "r");
	if (!file) {
		printf("IOERR\n");
		return 0;
	}
	do {
		c = fgetc(file);
		if (('A'<=c && 'Z'>=c) || ('a'<=c && 'z'>=c) || ('0'<=c && '9'>=c)) {

			// handles a valid word at the beginning of the stream
			if (TRUE == first_char_flag) {
				if ((('A'<=c && 'Z'>=c) || ('a'<=c && 'z'>=c))) {
					num_read_chars = 0;
					letter_start_flag = TRUE;
				}
				first_char_flag = FALSE;
				continue;
			}

			// detects possible word at intermediate locations in the stream
			if (TRUE == whitespace_char_flag) {
				// this is our way of asserting that the flag is FALSE
				if (TRUE == letter_start_flag) {
					// happens if we forget to disable flag
					printf("first-char-flag-enabled-error\n");
					goto err;
				}
				// intermediate words are preceeded by whitespace
				if ((('A'<=c && 'Z'>=c) || ('a'<=c && 'z'>=c))) {
					num_read_chars = 0;
					letter_start_flag = TRUE;
				}
				whitespace_char_flag = FALSE;
				continue;
			}

			// this is partly how we determine if we have a valid word:
			// if this is true that means that we might be reading a valid
			// word (at least the current char is valid) so we set the flag
			// to true to indicate that so far the word is valid (that is,
			// it has more than one character and all the characters are
			// valid)
			if (TRUE == letter_start_flag) {
				if (0 == num_read_chars) {
				       if (FALSE == valid_chars_flag) {
						valid_chars_flag = TRUE;
				       }
				}
				++num_read_chars;
			}

			whitespace_char_flag = FALSE;

		} else {

			if (TRUE == first_char_flag) {
				first_char_flag = FALSE;
			}

			if (' ' == c || '\t' == 'c' || '\n' == c) {

				// we have just read a word
				if (TRUE == letter_start_flag) {
					if (TRUE == valid_chars_flag) {
						// increment counter if the word in valid
						++num_words;
						valid_chars_flag = FALSE;
					}
					letter_start_flag = FALSE;
				}

				whitespace_char_flag = TRUE;

			} else {
				// we have a character that does not conform a valid word
				// so we disable the flag, note that the flag won't be
				// set again until we actually hit a new word
				if (TRUE == letter_start_flag) {
					++num_read_chars;
					valid_chars_flag = FALSE;
				}

				whitespace_char_flag = FALSE;
			}
		}
		++num_chars;
	} while (EOF != c);

	// handles last word, we might have hit EOF while reading a valid word
	if ((TRUE == letter_start_flag) && (TRUE == valid_chars_flag)) {
		++num_words;
		letter_start_flag = FALSE;
		valid_chars_flag = FALSE;
	}

	printf("number of words: %d\n", num_words);
	fclose(file);
	return 0;
err:
	fclose(file);
	return 0;
}
