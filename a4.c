#define  _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<assert.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define exit(N) {fflush(stdout); fflush(stderr); _exit(N); }
static int get_port();
static const char error400[] = "400 Bad Request";
static const char ok200[] = "200 OK";
static const char error404[] = "404 Not Found";
static char reader[3000];
static char clientHeader[1025];
static int headerSize;
static int bodySize;
static char body[1025];
static int posted = 0;
static int readingHeader(int clientsocket );
int readingRequestLine(char *requestLine, int clientsocket);
static int allStats[5];
static fd_set readset, readyset, writeset, writeReadySet;

typedef struct dataNode data;

struct dataNode{
    char *dataChunk;
    int chunkLength;
    data  *next;
    data *prev;

};
typedef struct {
    int clientfd;
    char selfReader[3000];
    data * processDatas;
    data * end;
    
} clientData;
clientData cliendfds[12];

void clear(clientData * clientInfo, int index) {
    clientInfo[index].clientfd = -1;
    memset(cliendfds[index].selfReader, 0, sizeof(cliendfds[index].selfReader));
    clientInfo[index].processDatas = NULL;
    clientInfo[index].end = NULL;
    
}

void freeEnd(clientData * cliendfd ) {
    data* temp = cliendfd -> end;
    cliendfd -> end = cliendfd-> end -> prev;
    free(temp ->dataChunk);
    free(temp);
}
void createDataChunck(clientData * cliendfd, char *buffer) {
    data * newNode = malloc(sizeof(data));
    newNode ->chunkLength = strlen(buffer);
    char * bufferStore = malloc(1025 * sizeof(char));
    memcpy(bufferStore, buffer, strlen(buffer));
    newNode ->dataChunk = bufferStore;
    newNode ->prev = NULL;
    newNode ->next = cliendfd ->processDatas;
    if (cliendfd ->processDatas == NULL){
        cliendfd -> end = newNode;
        cliendfd -> processDatas = newNode;
    } else {
        cliendfd -> processDatas -> prev = newNode;
        cliendfd -> processDatas = newNode;
    }

}
int main(int argc, char** argv) {

    int port = get_port();

    printf("Using port %d\n", port);
    printf("PID: %d\n", getpid());
    // Make server available on port

    // Process client requests
    int mysocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myAddressInfo;

    inet_pton(AF_INET, "127.0.0.1", &(myAddressInfo.sin_addr));

    myAddressInfo.sin_family = AF_INET;
    myAddressInfo.sin_port = htons(port);
    int optval = 1;
    setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int bindStatus = bind(mysocket, (struct sockaddr *)&myAddressInfo, sizeof(myAddressInfo));
    if (bindStatus != 0) {
        perror("BIND Error: ");
    }

    int listenStatus = listen(mysocket, 20);
    if (listenStatus != 0) {
        perror("LISTEN Error: ");
    }

    struct sockaddr_in clientAddr;
    socklen_t sockSize = sizeof(clientAddr);
    memset(allStats, 0, sizeof(allStats));
    
    for (int i = 0; i < 12; i++) {
        cliendfds[i].processDatas = NULL;
        clear(cliendfds, i);
    }
    FD_ZERO(&readset);
    FD_ZERO(&readyset);
    FD_ZERO(&writeset);
    FD_ZERO(&writeReadySet);
    FD_SET(mysocket, &readset);
    int maxsock = mysocket;
    while (1) {
        readyset = readset;
        writeReadySet = writeset;
        select(maxsock+1, &readyset,&writeReadySet,NULL,NULL);
        if (FD_ISSET(mysocket, &readyset)) {
            int clientsocket = accept(mysocket, (struct sockaddr *)&clientAddr,&sockSize );
            FD_SET(clientsocket, &readset);
            if (clientsocket == -1) {
                perror("Accept Error: ");
            }

            for (int i = 0; i < 12; i++) {
                if (cliendfds[i].clientfd < 0) {
                    cliendfds[i].clientfd = clientsocket;
                    if (maxsock < clientsocket) {
                        maxsock = clientsocket;
                    }
                    break;
                }
            }
        }
        
        for (int i = 0; i < 12;i++) {
            if (cliendfds[i].clientfd > 0 && FD_ISSET(cliendfds[i].clientfd, &readyset)) {
        
                int status = recv(cliendfds[i].clientfd, cliendfds[i].selfReader, 3000, 0);
                if (status == -1) {
                    perror("client Buffer Error: ");
                }
                memcpy(reader, cliendfds[i].selfReader, sizeof(cliendfds[i].selfReader));
                readingHeader(cliendfds[i].clientfd);
                FD_SET(cliendfds[i].clientfd,&writeset);
                FD_CLR(cliendfds[i].clientfd,&readset);
            }
        }

        for (int i = 0; i < 12; i++) {
            if (cliendfds[i].clientfd > 0 && FD_ISSET(cliendfds[i].clientfd, &writeReadySet)) {
                if (cliendfds[i].end != NULL) {
                    char *temp = (cliendfds[i].end) ->dataChunk;
                    int length = (cliendfds[i].end) ->chunkLength;
                    int sendStat = send(cliendfds[i].clientfd, temp, length,0);
                    freeEnd(&(cliendfds[i]));
                    
                } else {
                    close(cliendfds[i].clientfd);
                    FD_CLR(cliendfds[i].clientfd, &writeset);
                    clear(cliendfds,i);
                }
            }
        }
        

        memset(reader, 0, sizeof(reader));  
        memset(clientHeader, 0, sizeof(clientHeader));  
        
    }
    close(mysocket);
    return 0;
}

static int get_port() {
    int fd = open("port.txt", O_RDONLY);
    if (fd < 0) {
        perror("Could not open port");
        exit(1);
    }

    char buffer[32];
    int r = read(fd, buffer, sizeof(buffer));
    if (r < 0) {
        perror("Could not read port");
        exit(1);
    }

    return atoi(buffer);
}

static int readingHeader(int clientsocket) {

    int len = 1024;
    memcpy(clientHeader, reader, len);
    clientHeader[1024] = '\0';
    char * endHeader = strstr(clientHeader, "\r\n\r\n");
    if (endHeader != NULL) {
        endHeader = '\0';
        len = strlen(clientHeader);
    }

    headerSize = len;
    char tempHeader[headerSize+1];

    memcpy(tempHeader, clientHeader, headerSize+1);
    char *requestLine = strtok(tempHeader, "\r\n");

    int requestStat = 0;
    if (requestLine != NULL) {
        requestStat = readingRequestLine(requestLine, clientsocket);
    }
    if(!requestStat){
        allStats[0]--;
    }
    allStats[0]++;
    
    
    
    //printf("%s size: %d", clientHeader, headerSize);
    return 1;
}
int sendData(char *buffer,  int clientsocket) {
    int len = strlen(buffer);
    for (int i = 0; i < 12; i++) {
        if (cliendfds[i].clientfd == clientsocket) {

            createDataChunck(&(cliendfds[i]), buffer);
            break;
        }
        
    }
    

    return len;
}
void ping(char *version, int clientsocket) {
    char head[50];
    snprintf(head, 50, "%s %s\r\n%s\r\n\r\n",version, ok200, "Content-Length: 4");
    allStats[1] += sendData(head, clientsocket);
    char *theBody ="pong";
    allStats[2] += sendData(theBody, clientsocket);
    
}

void echo(char *version, int clientsocket) {
    
    char tempHeader[headerSize+1];
    memcpy(tempHeader, clientHeader, headerSize+1);
    
    char * headDes = strstr(tempHeader,"\r\n") + 2;
    char * end= strstr(tempHeader, "\r\n\r\n");
    if (end != NULL){
        *end='\0';
    } 
    

    int desLen = strlen(headDes);

    char  head[50];
    snprintf(head, (50), "%s %s\r\n%s%d\r\n\r\n",version, ok200,"Content-Length: ",desLen);
    allStats[1] += sendData(head, clientsocket);
    allStats[2] += sendData(headDes,clientsocket);
}

void postWrite (char* version, int clientsocket) {
    memset(body, 0, sizeof(body));
    posted = 1;
    char tempHeader[3000];
    memcpy(tempHeader, clientHeader, headerSize);
    char* bodyLenStr = strstr(tempHeader, "Content-Length: ") + 16;
    char * numEnd = strstr(bodyLenStr, "\r\n");
    numEnd[0] = '\0';
    bodySize = atoi(bodyLenStr);
    if (bodySize > 1024) {
        bodySize = 1024;
    }
    memset(tempHeader, 0, sizeof(reader));
    memcpy(tempHeader, reader, 3000);

    for (int i =0; i < 3000; i++) {
        if (strncmp(&(tempHeader[i]), "\r\n\r\n", 4) == 0) {
            numEnd = (&tempHeader[i]) + 4;
            break;
        }
    }

    memcpy(body, numEnd, bodySize);


    char header[50];
    char * content = "Content-Length: ";
    snprintf(header, 50, "%s %s\r\n%s%d\r\n\r\n",version, ok200, content,bodySize);
    allStats[1] += sendData(header, clientsocket);
    allStats[2] += sendData(body, clientsocket);
    

}

void readBody(char * version, int clientsocket) {
    char header[50];
    char *theBody;
    char * content = "Content-Length: ";
    int sendSize;
    if (!posted) {
        snprintf(header, 50, "%s %s\r\n%s7\r\n\r\n",version, ok200, content);
        theBody = "<empty>";

    } else {
        snprintf(header, 50, "%s %s\r\n%s%d\r\n\r\n",version, ok200, content,bodySize);
        theBody = body;
    }

    allStats[1] += sendData(header, clientsocket);
    allStats[2] += sendData(theBody,clientsocket);
}
void other(char * version, int clientsocket, char * url) {
    struct stat fileStat;
    char * fileName = &url[1];
    int fd = open(fileName, O_RDONLY, 0);
    if (fd < 0) {
        char buffer[30];
        int errorBytes = snprintf(buffer, 30, "%s %s", version, error404);
        allStats[3]++;
        allStats[4] += sendData(buffer, clientsocket);
        allStats[0]--;
        return;
    }

    fstat(fd, &fileStat);
    char buffer[1025];
    memset(buffer,0, sizeof(buffer));
    allStats[1] += snprintf(buffer, 1024, "%s %s\r\nContent-Length: %ld\r\n\r\n", version, ok200, fileStat.st_size);
    sendData(buffer, clientsocket);

    while(read(fd, buffer, 1024) != 0) {
        buffer[1024] = '\0';
        allStats[2] += sendData(buffer, clientsocket);
        memset(buffer,0,sizeof(buffer));
    }
    close(fd);

}

void getStats(char * version, int clientsocket) {
    char header[50];
    char theBody[100];
    int theBodySize = 
    snprintf(theBody, 100, 
    "Requests: %d\nHeader bytes: %d\nBody bytes: %d\nErrors: %d\nError bytes: %d", 
    allStats[0], allStats[1], allStats[2], allStats[3], allStats[4]);

    int headerSize = snprintf(header, 50,"%s %s\r\nContent-Length: %d\r\n\r\n",version, ok200, theBodySize);

    allStats[1] += sendData(header, clientsocket);
    allStats[2] += sendData(theBody, clientsocket);

}
int readingRequestLine(char *requestLine, int clientsocket) {
    
    char * method = strtok(requestLine, " ");
    char * request = strtok(NULL, " ");
    char * version = strtok(NULL, " ");
    if (version == NULL) {
        version = "HTTP/1.1";
    }

    if (strcmp(method, "GET")== 0 && strlen(method) == 3) {
        if (strcmp(request, "/ping") == 0) {
            ping(version, clientsocket);
        } else if (strcmp(request, "/echo") == 0) {
            echo(version, clientsocket);
        } else if (strcmp(request,"/read") == 0) {
            readBody(version, clientsocket);
        } else if (strcmp(request,"/stats") == 0) {
            getStats(version, clientsocket);
        } else {
            other(version, clientsocket, request);
        }
    } else if (strcmp(method, "POST") == 0 && strlen(method) == 4) {
        if (strcmp(request, "/write") == 0) {
            postWrite(version, clientsocket);
        } else {
            allStats[3]++;
            char buffer[30];
            int errorBytes = snprintf(buffer, 30, "%s %s", version, error400);
            allStats[4] += sendData(buffer, clientsocket);
            return 0;
        }
    } else {
        allStats[3]++;
        char buffer[30];
        int errorBytes = snprintf(buffer, 30, "%s %s", version, error400);
        allStats[4] += sendData(buffer, clientsocket);
        return 0;
    }
    return 1;

}