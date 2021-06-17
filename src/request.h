
typedef struct Request *Request;


Request createRequest(char *buffer, int pidClient);

int runRequest(Request r);

void addFilter(char *filter);

void sendStatus(char *path, int pidClient);