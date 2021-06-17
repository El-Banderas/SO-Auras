
typedef struct Request *Request;

//Send message to client.
void sendMessage(char *path, int pidClient, char * message);

Request createRequest(char *buffer, int pidClient, char * path);

int runRequest(Request r, char * path);

void addFilter(char *filter);

void sendStatus(char *path, int pidClient);

pid_t getPid(Request r);