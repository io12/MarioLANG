#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [file]\n", argv[0]);
		exit(1);
	}
	FILE* fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "File %s not found\n", argv[1]);
		exit(1);
	}

	long lines = 1;
	long cols = 1;
	bool colsf = true;
	char c;
	for (;;) {
		if (colsf)
			++cols;
		if ((c = fgetc(fp)) == '\n') {
			++lines;
			colsf = false;
		}
		if (c == EOF)
			break;
	}
	rewind(fp);
	char** map = malloc(lines);
	for (int i = 0; i < lines; ++i) {
		map[i] = malloc(cols);
		fgets(map[i], cols, fp);
	}

	bool right = true;
	bool standing = false;
	bool walking = true;
	bool skip = false;
	bool bracket_skip = false;
	uint8_t* m = malloc(30000);
	uint16_t mp = 0;
	memset(m, 0, 30000);

	for (int i = 0, j = 0;;) {
		skip = false;
		if (!bracket_skip) {
			switch (map[j][i]) {
				case '=': standing = true; --j; break;
				case '|': walking = false; right ? --i : ++i; break;
				case '#': standing = true; walking = false; 
					  {
						  int e;
						  bool success = false;
						  for (e = j; e < lines; ++e)
							  if (map[e][i] == '"') {
								  j = e - 1;
								  success = true;
								  break;
							  }
						  if (success)
							  break;
						  else
							  for (e = j; e >= 0; --e)
								  if (map[e][i] == '"') {
									  j = e - 1;
									  success = true;
									  break;
								  }
						  if (success)
							  break;
						  else
							  exit(1);
					  }
				case '"': standing = true; walking = true; --j; break;
				case ')': mp == 30000 ? mp = 0 : ++mp; break;
				case '(': mp == 0 ? mp = 30000 : --mp; break;
				case '+': ++m[mp]; break;
				case '-': --m[mp]; break;
				case '.': putchar(m[mp]); break;
				case ':': printf(" %d ", (int) m[mp]); break;
				case ',': m[mp] = getchar(); break;
				case ';':
					  {
						  char* intstring;
						  getdelim(&intstring, 0, ' ', stdin);
						  sscanf(intstring, "%d", (int*) &m[mp]);
					  }
					  break;
				case '>': skip = true; walking = true; right = true; ++i; break;
				case '<': skip = true; walking = true; right = false; --i; break;
				case '^': walking = false; --j; break;
				case '!': walking = false; break;
				case '[': bracket_skip = true; break;
				case '@': walking = true; --right; break;
			}
		}
		else
			bracket_skip = false;
		if (!skip) {
			if (j + 1 >= lines || (map[j + 1][i] != '"' && map[j + 1][i] != '=' && map[j + 1][i] != '^'))
				standing = false;
			else
				standing = true;
			if (!standing)
				++j;
			else if (right && walking)
				++i;
			else if (!right && walking)
				--i;
		}
		if (j >= lines || i >= cols) {
			putchar('\n');
			exit(0);
		}
	}
	return 0;
}
