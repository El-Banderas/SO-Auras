
typedef struct Request *Request;


Request createRequest(char *buffer, int pidClient);



int runRequest(Request r);

struct Filters;

struct Filters* initFilterStructur();

void addFilter(char *filter, struct Filters *current);


ArrayChar * toString(struct Filters *x);

void sendStatus(struct Filters *all, char * path, int pidClient);