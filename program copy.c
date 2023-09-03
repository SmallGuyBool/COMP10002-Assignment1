/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Julian Lewis 1444859
   Dated:     30/08/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */

/* Crude fileno fix to allow compilation on Windows */
#define FILENO_STDIN 0
#define FILENO_STDOUT 1 


/* Placeholder typedef for skeleton code
*/

typedef struct {
    int length;
    int digits[INTSIZE];
} longint_t;

#define LONG_ZERO 0
#define LONG_ONE  1


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
//int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void zero_vars(longint_t vars[]);
longint_t parse_num(char *rhs);
int get_longint_length(char *num);

int max_2_ints(int num1, int num2);
void do_product(longint_t *var1, longint_t *var2);
void print_register_info(longint_t *var1);
void digit_shift(longint_t *var1, int shift_width);


/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {

    /* Read up to 1000 digit char array */
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];

    /* Initialise vars as a 0 longint array and prompt user for input */
	zero_vars(vars);
	print_prompt();

    /* Read input from user into line array */
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
        /* Prompt user for input again */
		print_prompt();
	}

    /* All done! finish up :) */
	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(FILENO_STDIN)) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(FILENO_STDIN) && isatty(FILENO_STDOUT)) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(FILENO_STDIN) && !isatty(FILENO_STDOUT)) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(FILENO_STDIN) || isatty(FILENO_STDOUT)) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(FILENO_STDOUT)) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(FILENO_STDIN) || !isatty(FILENO_STDOUT)) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value;

	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, &second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (optype == ASSIGN) {
		do_assign(vars+varnum, &second_value);
	} else if (optype == PLUS) {
		do_plus(vars+varnum, &second_value);

	} else if (optype == MULT) {
        do_product(vars+varnum, &second_value);
    } else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
			longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		*second_value = parse_num(rhsarg);
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
	int i;
    /* Initialise a zero longint_t */
	longint_t zero;
    for(i=0; i<INTSIZE; i++) {
        zero.digits[i] = 0;
    }
    zero.length = 0;

    /* Iterate and assign all registers to longint_t zero */
	for (i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
longint_t
parse_num(char *rhs) {
	int i, rhs_len = strlen(rhs), first_non_zero = 0, leading_zero_count = 0;;
	int parsed_digit;
    longint_t parsed_num;

    

    /* Read number backwards into parsed_num digits buffer */
    for(i=0; i<rhs_len; i++) {
        
        /* Offset characters by the code point of '0' */
        
		parsed_digit = rhs[rhs_len - 1 - i] - CH_ZERO;
		/* Check if the digit being read in is non-zero */
		if(rhs[i] != '0') {
			first_non_zero = 1;
		}
		/* Overwrite leading zeros */
		if(!first_non_zero) {
			leading_zero_count++;
		}
		
		parsed_num.digits[i] = (int)parsed_digit;
    }
	parsed_num.length = rhs_len - leading_zero_count;
	//printf("Read: %s, parsed to: %d\n", rhs, parsed_num.length);
    return parsed_num;
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {
	printf("register %c: ", varnum+CH_A);
    int i;
    for(i=var->length-1; i>=0; i--) {
        printf("%d", var->digits[i]);
    }
    printf("\n");
	
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t *var1, longint_t *var2) {
	
	*var1 = *var2;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t *var1, longint_t *var2) {

	/*printf("\n\nAddition:\n");
	print_register_info(var1);
	print_register_info(var2);
	printf("End addition\n\n");*/

    int i, len_increase = 0, var1_len = var1->length, 
		var2_len = var2->length, carry = 0;

    int longest_len = max_2_ints(var1_len, var2_len);

    /* Check for overflows before we modify the underlying values */
    if(var1->digits[INTSIZE - 1] + var2->digits[INTSIZE - 1] >= INT_TEN) {
        print_error("longint_t overflow has occurred.");
        exit(EXIT_FAILURE);
    }

	/* Iterate through all digits involved in the sum */
    for(i=0; i < longest_len + carry; i++) {

		/* If the current digit is in-bounds for both numbers, sum them 
		and update corresponding var1 digit */
        if(i < var2_len && i < var1_len) {
        	var1->digits[i] += var2->digits[i];
		} else if (i >= var1_len) {
			/* var1 is out of bounds, so just assign it the digit of var2 */
			var1->digits[i] = var2->digits[i];
		}
        /* Add previous carry to the current digit and reset carry */
        var1->digits[i] += carry;
        carry = 0;

        /* Handle digit carries */
		if(var1->digits[i] >= INT_TEN) {
			
            /* Check if carry occurs in final digit */
            if(i == longest_len - 1) {
                len_increase = 1;
            }
			carry = 1;
			var1->digits[i] -= INT_TEN;
		}

    }

    /* Update the length of var1 */
	var1->length = longest_len + len_increase;
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/* Return the largest of 2 input integers
*/
int
max_2_ints(int num1, int num2) {
	int max_int = (num1 > num2) ?  num1 : num2;
	return max_int;
}

void
print_register_info(longint_t *var1) {
	int i = 0, var_len = var1->length;
	printf("Length: %d, Value: ", var_len);
	for(i=var_len - 1; i>=0; i--) {
		printf("%d", var1->digits[i]);
	}
	printf("\n");
}

/* Shift a longint_t register over by 1 digit and increment
   the length by one. This is the same effect as multiplying by
   10.
*/
void
digit_shift(longint_t *var1, int shift_width) {
	int i, var1_len = var1->length, starting_pos = (var1_len + shift_width - 1);

	for(i=starting_pos; i>=0; i--) {
		if(i-shift_width >= 0) {
			var1->digits[i] = var1->digits[i-shift_width];
		} else {
			var1->digits[i] = 0;
		}
	}

	var1->length += shift_width;
}

void
do_product(longint_t *var1, longint_t *var2) {
    int i, j, curr_digit_product, var1_len = var1->length, 
        var2_len = var2->length, carry = 0;

    /* Initialise empty longint_t structs to store intermediate products */
    longint_t curr_total_product, final_product;

	/* Initialise a zero longint_t */
	longint_t zero;
    for(i=0; i<INTSIZE; i++) {
        zero.digits[i] = 0;
    }
    zero.length = 0;

	do_assign(&curr_total_product, &zero);
	do_assign(&final_product, &zero);

	//print_register_info(&final_product);
	//print_register_info(&curr_total_product);

	for(int k = 0; k < 3; k++) {
		printf("%d",var1->digits[k]);
	}
	printf("\n");
    /* Iterate through all digits of the second number */
	for(i=0; i<var2_len; i++) {
        /* Iterate through all digits of the first number */
		for(j=0; j<var1_len + carry; j++) {
			printf("Multiplying:   %dx%d ", var2->digits[i], var1->digits[j]);
            /* Calculate the product of the current digit combination */
            curr_digit_product = var2->digits[i] * var1->digits[j];
            /* Account for any previous digit carries */
            curr_digit_product += carry;
            carry = 0;

            /* Check for current digit carries */
            if(curr_digit_product >= INT_TEN) {
                curr_total_product.digits[j] = curr_digit_product % INT_TEN;
				printf("assigning product digit: %d\n", curr_digit_product % INT_TEN);
                carry = curr_digit_product / INT_TEN;
            } else {
                /* No carry necessary, assign digit regularly */
                curr_total_product.digits[j] = curr_digit_product;
				printf("assigning product digit: %d\n", curr_digit_product);
            }
            //printf("Product digit [%d]  is  %d, carry?: %s\n", j, curr_total_product.digits[j], (carry ? "yes" : "no"));
        }
		
		curr_total_product.length = j;
		//print_register_info(&curr_total_product);
        /* Current intermediate product is complete, add to total product */
        if(i == 0) {
            do_assign(&final_product, &curr_total_product);
			//printf("Initial assignment\n");
        } else {
			/* Shift the product over to the correct base 10 digit */
			
			digit_shift(&curr_total_product, i);
			/* Add the intermediate product on to the final product */
            do_plus(&final_product, &curr_total_product);
			//printf("Sum\n");

        }
		//printf("%d\n", i);
	}
	/* Multiplication complete, assign value to var1 */
    do_assign(var1, &final_product);
}


/*
Algorithms are fun!!!
*/