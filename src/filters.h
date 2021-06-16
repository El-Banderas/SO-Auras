

struct Filters;

struct Filters* initFilterStructur() ;
void addFilter(char *filter, struct Filters *current) ;
ArrayChar * toString(struct Filters *x);
ArrayChar* getFiltersNames(struct Filters *x);