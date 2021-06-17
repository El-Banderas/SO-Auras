#include "basicOperations.h"


struct Filters;

void initFilterStructur();

void addFilter(char *filter);

ArrayChar *toString();

ArrayChar *getFiltersNames();

int filtersMissing(ArrayChar *filters);