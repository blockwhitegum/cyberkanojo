#include <stdio.h>
#include <curl/curl.h>
#include <string>

// 用于处理来自libcurl的响应数据的回调函数
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

// 发送请求到OpenAI的函数
int send_request_to_openai(const char *api_key, const char *prompt) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // 设置目标URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/davinci-codex/completions");

        // 设置HTTP头，包括认证信息
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 设置POST请求数据
        char postfields[1024];
        snprintf(postfields, sizeof(postfields), "{\"prompt\": \"%s\", \"max_tokens\": 150}", prompt);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);

        // 设置响应数据处理函数
        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // 执行请求
        res = curl_easy_perform(curl);

        // 清理
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        // 输出响应
        printf("%s\n", response_string.c_str());
    }

    curl_global_cleanup();
    return 0;
}

int main() {
    const char *api_key = "YOUR_OPENAI_API_KEY"; // 替换为你的API密钥
    const char *prompt = "Cheer up your girlfriend with a sweet message:"; // 你的提示

    send_request_to_openai(api_key, prompt);

    return 0;
}
