    char cleaned_target[256];
    const char *host_start = target;
    
    const char *protocol_end = strstr(target, "://");
    if (protocol_end != NULL) {
        host_start = protocol_end + 3;
    }

size_t i = 0;
    while (host_start[i] != '\0' && host_start[i] != ':' && host_start[i] != '/' && i < sizeof(cleaned_target) - 1) {
        cleaned_target[i] = host_start[i];
        i++;
    }
    cleaned_target[i] = '\0';