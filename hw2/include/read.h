extern long gedcom_lines;
extern long current_lineno;
extern char *current_gedcom;

struct node *read_gedcom(FILE *f, struct node *prev, int level);

