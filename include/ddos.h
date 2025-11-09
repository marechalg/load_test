#ifndef DDOS_H
#define DDOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void ddos(char*, int, int, int);

static inline void ddos(char* target, int threads, int connections, int duration) {
    char cleaned_target[256];
    const char *host_start = target;
    
    // Check for protocol prefix (e.g., http://, https://)
    const char *protocol_end = strstr(target, "://");
    if (protocol_end != NULL) {
        host_start = protocol_end + 3;  // Skip past "://"
    }
    
    // Copy the hostname part, stopping at ':' (port) or '/' (path)
    size_t i = 0;
    while (host_start[i] != '\0' && host_start[i] != ':' && host_start[i] != '/' && i < sizeof(cleaned_target) - 1) {
        cleaned_target[i] = host_start[i];
        i++;
    }
    cleaned_target[i] = '\0';

    char wrk_command[256];
    snprintf(wrk_command, sizeof(wrk_command), "wrk -t%d -c%d -d%ds %s", 
             threads, connections, duration, target);
    
    char command[1024];
    snprintf(command, sizeof(command), "mkdir -p ./output/%s && %s > ./output/%s/temp", 
             cleaned_target, wrk_command, cleaned_target);
    
    system(command);
}

#endif