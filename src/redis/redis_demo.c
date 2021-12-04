
int redis_demo(int argc, char **argv) {
    struct timespec start, end, end1;
    unsigned int j, isunix = 0;
    redisContext *c;
    redisReply *reply;
    log_debug("%d args> arg0=%s", argc, argv[0]);
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";

    if (argc > 2) {
        if (*argv[2] == 'u' || *argv[2] == 'U') {
            isunix = 1;
            log_info("Will connect to unix socket @%s", hostname);
        }
    }

    int port = (argc > 2) ? atoi(argv[2]) : 6379;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    if (isunix) {
        c = redisConnectUnixWithTimeout(hostname, timeout);
    } else {
        c = redisConnectWithTimeout(hostname, port, timeout);
    }
    if (c == NULL || c->err) {
        if (c) {
            log_error("Connection error: %s", c->errstr);
            redisFree(c);
        } else {
            log_error("Connection error: can't allocate redis context");
        }
        log_error("Redis Connection %s:%d Failed",
          hostname, port
        );
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if(end.tv_nsec < start.tv_nsec){
      end.tv_nsec += 1000000000;
      end.tv_sec--;
    }
    log_debug("Validated Redis Connection %s:%d in %ld.%09lds", 
        hostname, port,
        (long)(end.tv_sec - start.tv_sec), (end.tv_nsec - start.tv_nsec)
    );

    reply = redisCommand(c,"PING");
    log_debug("Redis PING: %s", reply->str);
    freeReplyObject(reply);

    char *SET_VAL = 
      "KEY1"
      "\0";

    char *SET_KEY = 
      "12345"
      "\0";

    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    log_debug("SET: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET foo");
    log_debug("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"SET %s %s", SET_KEY, SET_VAL);
    log_info("Redis SET %s->%s :: Reply=%s\n", SET_KEY, SET_VAL, reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET %s", SET_KEY);
    log_info("GET %s: %lld", SET_KEY, reply->integer);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET %s", "counter");
    log_info("GET %s: %lld\n", "counter", reply->integer);
    freeReplyObject(reply);

    reply = redisCommand(c,"INCR counter");
    log_info("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET %s", "counter");
    log_info("GET %s: %lld\n", "counter", reply->integer);
    freeReplyObject(reply);

    char *LAST_UPDATE[100];
    sprintf(&LAST_UPDATE, "%lld", currentTimeMillis());
    log_debug("Setting LAST_UPDATE to %s", LAST_UPDATE);

    reply = redisCommand(c,"GET %s", "LAST_UPDATE");
    log_info("GET %s: %lld", "LAST_UPDATE", reply->integer);
    freeReplyObject(reply);

    reply = redisCommand(c,"DEL %s", "LAST_UPDATE");
    freeReplyObject(reply);
    log_debug("Deleted %s: %s", "LAST_UPDATE", reply->str);

    reply = redisCommand(c,"SET %s %s", "LAST_UPDATE", LAST_UPDATE);
    log_info("Redis SET %s->%s :: Reply=%s\n", "LAST_UPDATE", LAST_UPDATE, reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET %s", "LAST_UPDATE");
    log_info("GET %s: %s\n", "LAST_UPDATE", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);

    for (j = 0; j < 10; j++) {
        char buf[64];
        snprintf(buf,64,"%u",j);
        reply = redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }


    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            log_info("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

/*

    reply = redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%u",j);
        reply = redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

*/

/*
    char *LAST_UPDATE = sprintf("%d", currentTimeMillis());
    log_debug("Setting LAST_UPDATE to %s", LAST_UPDATE);
    reply = redisCommand(c,"SET %s %s", "LAST_UPDATE", LAST_UPDATE);
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);
*/
    /* Disconnects and frees the context */
    redisFree(c);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end1);
    if(end1.tv_nsec < start.tv_nsec){
      end1.tv_nsec += 1000000000;
      end1.tv_sec--;
    }
    log_debug("Redis Demo ended in %ld.%09lds", 
        (long)(end1.tv_sec - start.tv_sec), (end1.tv_nsec - start.tv_nsec)
    );

    return 0;
}
