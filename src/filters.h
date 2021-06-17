#include "basicOperations.h"


struct Filters;

void initFilterStructur();

void addFilter(char *filter);

ArrayChar *toString();

ArrayChar *getFiltersNames();

int filtersMissing(ArrayChar *filters);

ArrayChar *getFiltersNames();

char *toLook(char * toLook);
int changeFilter(ArrayChar* listUsedFilters, int change);


struct Filters* duplicateFilters();
void setFilters(struct Filters* toReplace);