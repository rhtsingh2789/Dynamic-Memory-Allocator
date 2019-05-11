struct client_registry{
    int fd;
    int num;
    sem_t sem;
    pthread_mutex_t mutex;
    struct client_registry *next;
};