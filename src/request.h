
typedef struct Request *Request;


Request createRequest(char *buffer);

int runRequest(Request r);

struct Filters;

struct Filters* initFilterStructur();

void addFilter(char *filter, struct Filters *current);

void toString(struct Filters *x);