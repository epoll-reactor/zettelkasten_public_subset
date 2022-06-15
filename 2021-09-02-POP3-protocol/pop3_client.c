#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <curl/curl.h>

typedef struct {
    CURL   *curl_handle;
    FILE   *fd;
    size_t  bytes_writed;
    char   *username;
    char   *password;
    char   *url;
    size_t  verbose;
    size_t  mail_count;
} pop3_ctx_t;

void pop3_ctx_init(pop3_ctx_t *ctx) {
    ctx->curl_handle  = NULL;
    ctx->fd           = NULL;
    ctx->bytes_writed = 0;
    ctx->username     = NULL;
    ctx->password     = NULL;
    ctx->url          = NULL;
    ctx->verbose      = 0;
    ctx->mail_count   = 0;
}

static CURLSH *shared_curl_handle;
static pthread_mutex_t share_data_lock[CURL_LOCK_DATA_LAST];

void libcurl_lock_cb(
    CURL             *handle,
    curl_lock_data    data,
    curl_lock_access  access,
    void             *stream
) {
    (void)handle;
    (void)access;
    (void)stream;
    pthread_mutex_lock(&share_data_lock[data]);
}

void libcurl_unlock_cb(CURL *handle, curl_lock_data data, void *stream) {
    (void)handle;
    (void)stream;
    pthread_mutex_unlock(&share_data_lock[data]);
}

size_t pop3_fwrite_cb(
    char   *buffer,
    size_t  size,
    size_t  nmemb,
    void   *stream
) {
    size_t real_size = size * nmemb;
    pop3_ctx_t* ctx = (pop3_ctx_t *)(stream);
    fwrite(buffer, 1, real_size, ctx->fd);
    ctx->bytes_writed += real_size;
    return real_size;
}

void init_shared_curl() {
    int i;
    for (i = 0; i < CURL_LOCK_DATA_LAST; ++i)
        pthread_mutex_init(&share_data_lock[i], NULL);

    shared_curl_handle = curl_share_init();
    curl_share_setopt(shared_curl_handle, CURLSHOPT_LOCKFUNC, libcurl_lock_cb);
    curl_share_setopt(shared_curl_handle, CURLSHOPT_UNLOCKFUNC, libcurl_unlock_cb);
    curl_share_setopt(shared_curl_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(shared_curl_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(shared_curl_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    curl_share_setopt(shared_curl_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
}

void pop3_request(pop3_ctx_t *ctx) {
    CURL *curl_handle = ctx->curl_handle;
    CURLcode curl_res;
    curl_easy_setopt(curl_handle, CURLOPT_USERNAME, ctx->username);
    curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, ctx->password);
    curl_easy_setopt(curl_handle, CURLOPT_URL, ctx->url);
    curl_easy_setopt(curl_handle, CURLOPT_SHARE, shared_curl_handle);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "pop3-curl-agent/1.0");
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, ctx);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, pop3_fwrite_cb);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, ctx->verbose);
    curl_res = curl_easy_perform(curl_handle);
    if (curl_res != CURLE_OK) {
        fprintf(stderr,
            "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(curl_res)
        );
        exit(-1);
    }
    curl_easy_reset(curl_handle);
}

void process_mail(pop3_ctx_t *ctx, int mail_index) {
    char filename[16];
    sprintf(filename, "mail_%d", mail_index);
    ctx->fd = fopen(filename, "w");
    sprintf(ctx->url, "pop3s://pop.gmail.com:995/%d", mail_index);
    pop3_request(ctx);
    fclose(ctx->fd);
}

void parse_command_line_args(int argc, char *argv[], pop3_ctx_t *args) {
    int i;
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--username") == 0)
            args->username = argv[i + 1];
            
        else if (strcmp(argv[i], "--password") == 0)
            args->password = argv[i + 1];

        else if (strcmp(argv[i], "--verbose") == 0)
            args->verbose = atoi(argv[i + 1]);

        else if (strcmp(argv[i], "--count") == 0)
            args->mail_count = atoi(argv[i + 1]);
    }
}

void current_time(char buffer[64]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );
}

void make_output_dir() {
    char time_buffer[64];
    current_time(time_buffer);
    char dirname[128];
    sprintf(dirname, "mailbox.received.%s", time_buffer);
    mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    chdir(dirname);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Please use arguments, more info:\n\tpop3_client --help\n");
        return 1;
    }
    if (strcmp(argv[1], "--help") == 0) {
        printf("Usage:    pop3_client\n"
               "          --count <mail count>\n"
               "          --username <your mailbox name>\n"
               "          --password <your mailbox password>\n"
               "\n"
               "NOTE:\tif you have an error like:\n"
               "\t-- SmtpCmdResp: 535-5.7.8 Username and Password not accepted,\n"
               "\tyou should log in into your Gmail account and in your browser go to\n"
               "\thttps://www.google.com/settings/security/lesssecureapps\n"
               "\tand TURN ON access for less secure apps.\n");
        return 1;
    }
    if ((argc != 7) && (argc != 9)) {
        fprintf(stderr, "Wrong arguments were given. Please use --help option.\n");
        return 1;
    }

    make_output_dir();
    init_shared_curl();

    CURL* curl_handle = curl_easy_init();
    if (!curl_handle) {
        fprintf(stderr, "curl_easy_init() failed");
        return 1;
    }

    pop3_ctx_t ctx;
    pop3_ctx_init(&ctx);
    parse_command_line_args(argc, argv, &ctx);

    char url[64];
    char time_buffer[64];
    char progress_log[256];
    size_t progress_log_size;
    size_t mail_index;
    const char bash_color[] = "\033[0;34m";
    const char bash_end_color[] = "\e[0m";

    for (mail_index = 1; mail_index <= ctx.mail_count; ++mail_index) {
        current_time(time_buffer);

        progress_log_size = sprintf(progress_log,
            "%s[%s]%s %s*%s Process... %s%5zu/%zu%s ",
            bash_color, time_buffer,                bash_end_color,
            bash_color,                             bash_end_color,
            bash_color, mail_index, ctx.mail_count, bash_end_color
        );

        ctx.curl_handle = curl_handle;
        ctx.url = url;

        process_mail(&ctx, mail_index);
        sprintf(progress_log + progress_log_size,
            "%s%7zu%s KiB received\n",
            bash_color, ctx.bytes_writed / 1024, bash_end_color
        );
        printf(progress_log);
        memset(progress_log, 0, sizeof(progress_log));
        ctx.bytes_writed = 0;
    }

    curl_easy_cleanup(curl_handle);
    curl_share_cleanup(shared_curl_handle);

    return 0;
}
