#ifndef REACH_H
#define REACH_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static inline bool reachable(const char *target) {
    if (target == NULL || strlen(target) == 0) {
        return false;
    }
    
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
    
    // Check if we extracted a valid hostname
    if (strlen(cleaned_target) == 0) {
        return false;
    }
    
    char command[512];
    int result;
    
    #ifdef _WIN32
        snprintf(command, sizeof(command), "ping -n 1 -w 1000 %s >nul 2>nul", cleaned_target);
    #elif __APPLE__
        snprintf(command, sizeof(command), "ping -c 1 -W 1000 %s >/dev/null 2>&1", cleaned_target);
    #else
        snprintf(command, sizeof(command), "ping -c 1 -W 1 %s >/dev/null 2>&1", cleaned_target);
    #endif
    
    result = system(command);
    
    return (result == 0);
}

#endif