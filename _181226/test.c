#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* MACRO POSTFIX SETTING SPECIFICATION
 * V_ :: Variable & Specifier generation.
 * M_ :: General macroes.
 * P_ :: Parameter binding.
 * A_ :: Arguments binding. */

/* @macro V_PARSE
 * @brief Parse __MASpecifier__ and __Post__ as __MASpecifier____Post__ name */
#define V_PARSE(__MASpecifier__, __Post__) __MASpecifier__ ## __Post__
/* @macro V_PARSE_LINE
 * @brief Set variable name with postfix, '__LINE__'. */
#define V_PARSE_LINE(__MASpecifier__) V_PARSE(__MASpecifier__, __LINE__)

/* @macro M_LEN
 * @brief Get value of array which is not decayed yet.  */
#define M_LEN(__MAArray__) sizeof(__MAArray__) / sizeof(__MAArray__[0])

/* @macro V_LEN
 * @brief Substitute input variable name with postfix, '_len'.  */
#define V_LEN(__MASpecifier__) V_PARSE(__MASpecifier__, _len)

/* @macro P_ARRAY
 * @brief Insert array as parameter with binding (zipping) */
#define P_ARRAY(__Type__, __MASpecifier__) \
	__Type__* __MASpecifier__, unsigned V_LEN(__MASpecifier__)

/* @macro A_ARRAY
 * @brief Input array as argument. This is substitute of P_ARRAY.  */
#define A_ARRAY(__MAArray__) __MAArray__, M_LEN(__MAArray__)

enum e_errcode
{
	ERR_ArgumentCntSmall,
	ERR_ArgumentNotDigit,
	ERR_OK
};

int sum(P_ARRAY(int, arr))
{
	int i, result;

	for (i = 0, result = 0; i < V_LEN(arr); ++i)
	{
		result += arr[i];
	}

	return result;
}

int subtract(P_ARRAY(int, arr))
{
	int i, result;
	for (i = 0, result = 0; i < V_LEN(arr); ++i)
	{
		result -= arr[i]; 
	}

	return result;
}

typedef int(*cb_intarr_t)(P_ARRAY(int,_));

enum e_errcode CheckArgumentCount(int argc, unsigned count)
{
	if (argc <= count) { return ERR_ArgumentCntSmall; }
	return ERR_OK;
}

/* Check following argv is integer. */
enum e_errcode CheckArgumentsDigit(char** argv, int start, int end)
{
	int i, j;

	for (i = start; i < end; ++i)
	{	
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (j == 0)
			{
				if (isdigit(argv[i][j]) == false 
						&& argv[i][j] != '+' 
						&& argv[i][j] != '-')
				{
					return ERR_ArgumentNotDigit;
				}
			}
			else
			{
				if (isdigit(argv[i][j]) == false)
				{
					return ERR_ArgumentNotDigit;
				}
			}
			++j;
		}
	}

	return ERR_OK;
}

int main(int argc, char** argv)
{
	int i, j;
	int* arr;
	enum e_errcode err;
	cb_intarr_t cb_intarr = NULL;

	/* Exception when argc is only one. (execution program) */
	err = CheckArgumentCount(argc, 1);
	if (err == ERR_ArgumentCntSmall)
	{
		printf("Input one more integer arguments on execution.\n");
		return 0;
	}

	err = CheckArgumentsDigit(argv, 1, argc);
	if (err == ERR_ArgumentNotDigit)
	{
		printf("Argument is not a digit. Failed to execute.\n");
		return 0;
	}

	/* Create dynamic array for storing values from argv. */
	arr = (int*)malloc(sizeof(int) * (argc - 1));
	for (i = 1; i < argc; ++i) { arr[i-1] = atoi(argv[i]); }

	cb_intarr = sum;
	printf("Sum is %d\n", cb_intarr(arr, (argc - 1)));
	cb_intarr = subtract;
	printf("Subtract is %d\n", cb_intarr(arr, (argc - 1)));

	free(arr);
	return 0;
}
