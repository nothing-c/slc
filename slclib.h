#include <stdio.h>
#include <stdlib.h>
/* For isdigit() and isalpha() */
#include <ctype.h>

/* These stop the compiler from throwing a fit */
void expression (void);
void match (char x);
int slcisdigit (char x);
int slcisalpha (char x);

/* Struct to hold variable info (offset + name) */
typedef struct {
  char name;
  int offset;
} var;

/*
  The limit is arbitrary, and smaller than it really needs to be (stack size is
  ~1MiB on modern systems, apparently)
*/
#define VARLISTSIZE 64
/* List of variable names */
var VARIABLES[VARLISTSIZE];
int VARINDEX = 0;
/* Max size of a variable (for allocation) */
int VARSIZE = 72;

/* FLAGS */

/* If > 0, then the number being processed is negative */
int NEGFLAG;

/* ERRORS */

/* Print error message and exit */
void error (char * str) {
  fprintf(stderr, "\a Error: %s expected\n", str);
  exit(1);
}

/* INPUT GETTING */

/* Lookahead character */
char LOOK;

/* Look ahead one character */
void updatechar (void) {
  LOOK = getchar();
}

/* Get first character from input stream */
void init (void) {
  updatechar();
}

/*
  Get char from input stream
  This way, we can handle the error checking here instead of
  having to check for errors every time we want to get an alpha char
  from the stream
*/
char getname (void) {
  char ret;
  if (slcisalpha(LOOK) < 0) {
    error("Name");
  } else {
    ret = LOOK;
    updatechar();
    return ret;
  }
}

/*
  Get digit from stream
  Same reason for existence as getname()
*/
char * getnum (void) {
  char * ret;
  int iterate = 1;
  /* Do negatives */
  if (LOOK == '-') {
    updatechar();
    NEGFLAG = 1;
    ret = getnum();
    return ret;
  }
  /* Allow for nesting expressions */
  if (LOOK == '(') {
    match('(');
    expression();
    match(')');
  }
  /* Substitute varname for location in memory */
  if (slcisalpha(LOOK) == 0) {
    for (int i = 0; i < VARLISTSIZE; i++) {
      if (LOOK == VARIABLES[i].name) {
	ret = malloc(20);
	sprintf(ret, "-0x%x(%%rbp)", VARIABLES[i].offset);
	/*
	  Missing this gave me such a headache, but it's what's missing
	  to do proper variable substitution
	*/
	updatechar();
	return ret;
      }
    }
  }
  if (isdigit(LOOK) < 0) {
    error("Integer");
  } else {
    /* Maxint is 10 chars long */
    ret = malloc(12);
    *ret = '$';
    while (slcisdigit(LOOK) == 0) {
      *(ret + iterate) = LOOK;
      updatechar();
      iterate++;
    }
    *(ret + iterate) = '\0';
    return ret;
  }
}
      
/* INPUT CHECKING */

/* Match input characters */
void match (char x) {
  if (LOOK == x) {
    updatechar();
  } else {
    /* Abort if the characters don't match */
    error(&x);
  }
}

/* Check if there's an operator */
int isop (char x) {
  if ((x == '+') || (x == '-') || (x == '*') || (x == '/')) {
    return 0;
  } else {
    return -1;
  }
}

int slcisdigit (char x) {
  if ((x >= '0') && (x <= '9')) {
    return 0;
  } else {
    return -1;
  }
}

int slcisalpha (char x) {
  if ((toupper(x) >= 'A') && (toupper(x) <= 'Z')) {
    return 0;
  } else {
    return -1;
  }
}

/* CODE GENERATION */

/* Emit a line of asm */
void emitln (char * str) {
  printf("\t%s\n", str);
}

/* Put a number into a register */
void initreg (char *  x) {
  if (*(x + 1) == '\0') {
    /* Skip, and just let it pop out if we're recursing */
  } else { 
    char * output = malloc(13);
    sprintf(output, "mov %s, %%rax", x);
    emitln(output);
    free(output);
    /* Check if we're negative. If so, negate */
    if (NEGFLAG > 0) {
      NEGFLAG = 0;
      emitln("neg %rax");
    }
  }
}

