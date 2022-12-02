#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define BUFSIZE 100
#define MAXWORDS 100

struct tnode {      /* the tree node */
    char *word;         /* points to the text */
    int count;          /* number of occurences */
    struct tnode *left; /* left child */
    struct tnode *right; /* right child */
};

struct tnode *addtree(struct tnode*, char*);
void treeprint(struct tnode*);
int getword(char*, int);

struct tnode *nodes[MAXWORDS] = {NULL};
int get_nodes(struct tnode*);

// now is the time for all good men to come to the aid of the party

/* word frequency count */
int main(void) {
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            root = addtree(root, word);
        }
    }
    
    int i = get_nodes(root);
    for (--i; i >= 0; i--) {
        printf("%4d %s\n", nodes[i]->count, nodes[i]->word);
    }
    return 0;
}

void iterate(struct tnode *r, int freq, int *max_freq, int *n);

int get_nodes(struct tnode *r) {
    int frequency = 1, max_frequency = 1, n = 0;

    // Iterate recursively through the tree and fill nodes[] with nodes in increasing order of count
    for ( ; frequency <= max_frequency; frequency++) {
        iterate(r, frequency, &max_frequency, &n);
    }

    return n;
}

void iterate(struct tnode *r, int freq, int *max_freq, int *n) {
    if (r != NULL) {
        iterate(r->left, freq, max_freq, n);
        if (r->count == freq) {
            int i;
            for (i = 0; nodes[i] != NULL; i++);
            nodes[i] = r;
            *n+=1;
        }
        if (*max_freq < r->count) {
            *max_freq = r->count;
        }
        iterate(r->right, freq, max_freq, n);
    }
}

struct tnode *talloc(void);
char *strdup2(char* s);

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) {            /* a new word has arrived */
        p = talloc();           /* make a new node */
        p->word = strdup2(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;     /* repeated word */
    }
    else if (cond < 0) {    /* less than into left subtree */
        p->left = addtree(p->left, w);
    }
    else {
        p->right = addtree(p->right, w);
    }
    return p;
}

/* talloc: make a tnode */
struct tnode *talloc(void) {
    return malloc(sizeof(struct tnode));
}

char *strdup2(char *s) {     /* make a duplicate */
    char *p;

    p = (char*) malloc(strlen(s) + 1);  /* +1 for '\0' */
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%4d %s", p->count, p->word);
        treeprint(p->right);
    }
}

/* getword: get next word or character from input */
int getword(char* word, int lim) {
    int c, getch(void);
    void ungetch(int);
    char* w = word;

    while (isspace(c = getch()));
    if (c != EOF)
        *w++ = c;
    if (c == '/' || c == '*') {
        c = getch();
        *w++ = c;
    }
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++) {
        if (!isalnum(*w = getch()) && *w != '_') {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

char buf[BUFSIZE];  /* buffer for ungetch */
int bufp = 0;       /* next free position in buf */

int getch(void){ /* get a (possibly pushed back) character */    
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c){    /* push character back on input*/
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else 
        buf[bufp++] = c;
}