#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "buffer.h"
#include "pos_sockets/char_buffer.h"
#include "pos_sockets/active_socket.h"
#include "pos_sockets/passive_socket.h"

typedef struct point {
    double x;
    double y;
} POINT;

POINT generate_point(void) {
    double x = 2 * (rand() / (double) RAND_MAX) - 1;
    double y = 2 * (rand() / (double) RAND_MAX) - 1;
    POINT point = {x, y};
    return point;
}


typedef struct pi_estimation {
    long long total_count;
    long long inside_count;
} PI_ESTIMATION_DATA;

void pi_estimation_add_point(POINT data, struct pi_estimation *output_data) {
    ++output_data->total_count;
    if (data.x * data.x + data.y * data.y <= 1) {
        ++output_data->inside_count;
    }
    printf("Odhad pi: %f\n", 4 * (double) output_data->inside_count / (double) output_data->total_count);
}

_Bool pi_estimation_try_deserialize(struct pi_estimation *pi_estimation, struct char_buffer *buf) {
    if (sscanf(buf->data, "%lld;%lld;", &pi_estimation->total_count, &pi_estimation->inside_count) == 2) {
        return true;
    }
    return false;
}


GENERATE_BUFFER(struct point, point)


typedef struct thread_data {
    long long replications_count;
    struct buffer_point buf;
    pthread_mutex_t mutex;
    pthread_cond_t is_full;
    pthread_cond_t is_empty;

    short port;
    ACTIVE_SOCKET *my_socket;
} THREAD_DATA;

void thread_data_init(struct thread_data *data, long long replications_count, int buffer_capacity,
                      short port, ACTIVE_SOCKET *my_socket) {
    data->replications_count = replications_count;
    buffer_point_init(&data->buf, buffer_capacity);
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->is_full, NULL);
    pthread_cond_init(&data->is_empty, NULL);

    data->port = port;
    data->my_socket = my_socket;
}

void thread_data_destroy(struct thread_data *data) {
    data->replications_count = 0;
    buffer_point_destroy(&data->buf);
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->is_full);
    pthread_cond_destroy(&data->is_empty);

    data->port = 0;
    data->my_socket = NULL;
}

void *process_client_data(void *thread_data) {
    struct thread_data *data = (struct thread_data *) thread_data;
    PASSIVE_SOCKET sock;
    passive_socket_init(&sock);
    passive_socket_start_listening(&sock, data->port);
    passive_socket_wait_for_client(&sock, data->my_socket);
    //keby chcem viac klientov, tak by som dal viac krat wait_for_client/ whíle is listeing
    //a vytvoril by som viac threadov
    passive_socket_stop_listening(&sock);
    passive_socket_destroy(&sock);

    active_socket_start_reading(data->my_socket);

    return NULL;
}

void *produce(void *thread_data) {
    struct thread_data *data = (struct thread_data *) thread_data;

    for (long long i = 1; i <= data->replications_count; ++i) {
        POINT item = generate_point();

        pthread_mutex_lock(&data->mutex);
        while (!buffer_point_try_push(&data->buf, item)) {
            pthread_cond_wait(&data->is_empty, &data->mutex);
        }
        pthread_cond_signal(&data->is_full);
        pthread_mutex_unlock(&data->mutex);
    }
    return NULL;
}

_Bool try_get_client_pi_estimation(struct active_socket *my_sock, struct pi_estimation *client_pi_estimaton) {
    CHAR_BUFFER buf;
    char_buffer_init(&buf);
    _Bool result = false;
    if (active_socket_try_get_read_data(my_sock, &buf)) {
        if (!pi_estimation_try_deserialize(client_pi_estimaton, &buf)) {
            if (active_socket_is_end_message(my_sock, &buf)) {
                active_socket_stop_reading(my_sock);
            }
        } else {
            result = true;
        }
    }
    char_buffer_destroy(&buf);

    return result;
}

void *consume(void *thread_data) {
    struct thread_data *data = (struct thread_data *) thread_data;

    struct pi_estimation pi_estimaton = {0, 0};
    struct pi_estimation client_pi_estimaton = {0, 0};
    for (long long i = 1; i <= data->replications_count; ++i) {
        POINT item;

        pthread_mutex_lock(&data->mutex);
        while (!buffer_point_try_pop(&data->buf, &item)) {
            pthread_cond_wait(&data->is_full, &data->mutex);
        }
        pthread_cond_signal(&data->is_empty);
        pthread_mutex_unlock(&data->mutex);

        printf("%ld: ", i);
        ++pi_estimaton.total_count;
        if (item.x * item.x + item.y * item.y <= 1) {
            ++pi_estimaton.inside_count;
        }
        printf("Odhad pi: %lf\n", 4 * (double) pi_estimaton.inside_count / (double) pi_estimaton.total_count);

        if (data->my_socket != NULL) {
            try_get_client_pi_estimation(data->my_socket, &client_pi_estimaton);
            printf("%ld: ", i);
            printf("Odhad pi s vyuzitim dat od klienta: %lf\n",
                   4 * (double) (pi_estimaton.inside_count + client_pi_estimaton.inside_count) /
                   (double) (pi_estimaton.total_count + client_pi_estimaton.total_count));
        }
    }
    if (data->my_socket != NULL) {
        while (active_socket_is_reading(data->my_socket)) {
            if (try_get_client_pi_estimation(data->my_socket, &client_pi_estimaton)) {
                printf("Odhad pi s vyuzitim dat od klienta: %lf\n",
                       4 * (double) (pi_estimaton.inside_count + client_pi_estimaton.inside_count) /
                       (double) (pi_estimaton.total_count + client_pi_estimaton.total_count));
            }
        }
    }

    return NULL;
}

//! moje funkcie
void saveFile(const char *str) {
    printf("Saving file...\n");
    printf("%s", str);
    char temp[1024 * 2];
    strcpy(temp, str);

    char *filename = strtok(temp, ";");
    if (filename != NULL) {
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            for (int i = strlen(filename) + 1; str[i] != '\0'; i++) {
                if (str[i] == ';') {
                    fputc('\n', file);
                } else {
                    fputc(str[i], file);
                }
            }
            fclose(file);
        }
    }
    printf("File saved: %s\n", filename);
}

char *downloadFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file != NULL) {
        char line[256];
        char *result = malloc(1);
        result[0] = '\0';

        result = realloc(result, strlen(result) + strlen(fileName) + 2);
        strcat(result, fileName);
        strcat(result, ";");

        while (fgets(line, sizeof(line), file)) {
            for (int i = 0; line[i]; i++) {
                if (line[i] == '\n') {
                    line[i] = ';';
                }
            }
            result = realloc(result, strlen(result) + strlen(line) + 2);
            strcat(result, line);
        }

        fclose(file);
        return result;
    } else {
        printf("Failed to open file: %s\n", fileName);
        return NULL;
    }
}

void sendFile(ACTIVE_SOCKET *client_socket, const char *str) {
    CHAR_BUFFER data_to_send;
    char_buffer_init(&data_to_send);
    char_buffer_append(&data_to_send, str, strlen(str));

    active_socket_write_data(client_socket, &data_to_send);

    char_buffer_destroy(&data_to_send);
}

void *handle_client(void *arg) {
    ACTIVE_SOCKET *client_socket = (ACTIVE_SOCKET *) arg;

    CHAR_BUFFER buffer;
    char_buffer_init(&buffer);

    active_socket_start_reading(client_socket);

    if (active_socket_try_get_read_data(client_socket, &buffer)) {
        if (strncmp(buffer.data, "GET/", strlen("GET/")) == 0) {
            printf("Received a GET request for: %s\n", buffer.data + strlen("GET/"));
            char *fileContent = downloadFile(buffer.data + strlen("GET/"));
            printf("File content: %s\n", fileContent);
            sendFile(client_socket, fileContent);
            free(fileContent);
        } else {
            saveFile(buffer.data);
        }
    }
    char_buffer_destroy(&buffer);
    active_socket_destroy(client_socket);
    free(client_socket);

    return NULL;
}

int main(int argc, char *argv[]) {
    bool is_client = true;
    PASSIVE_SOCKET server_socket;
    passive_socket_init(&server_socket);
    passive_socket_start_listening(&server_socket, 18888);

    while (is_client) {
        ACTIVE_SOCKET *client_socket = malloc(sizeof(ACTIVE_SOCKET));
        active_socket_init(client_socket);

        passive_socket_wait_for_client(&server_socket, client_socket);

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, client_socket);

        pthread_detach(client_thread);
        //is_client= false;
    }
    passive_socket_destroy(&server_socket);

    return 0;
}
