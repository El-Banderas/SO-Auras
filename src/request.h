
typedef struct Request *Request;


Request createRequest(char *buffer);

int runRequest(Request r);