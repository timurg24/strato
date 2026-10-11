#include "HttpClient.hpp"

#include <curl/curl.h>
#include <tul/ErrorOps.hpp>

size_t Strato::HttpClient::writeCallback(char *data, size_t size, size_t count, void *userData)
{
    auto* buffer = static_cast<std::vector<uint8_t>*>(userData);

    size_t bytes = size * count;

    buffer->insert(
        buffer->end(),
        reinterpret_cast<uint8_t*>(data),
        reinterpret_cast<uint8_t*>(data) + bytes
    );

    return bytes;
}

/// @brief GETs some data from a URL
std::vector<uint8_t> Strato::HttpClient::get(const std::string& url) {
    std::vector<uint8_t> data;

    CURL* curl = curl_easy_init();
    if(!curl) tul::FatalError({"Failed to initialize CURL"});

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURL_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURL_WRITEFUNCTION, Strato::HttpClient::writeCallback);
    curl_easy_setopt(curl, CURL_WRITEDTATA, &data);

    CURLcode result = curl_easy_perform(curl);

    long status = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    curl_easy_cleanup(curl);

    if (result != CURLE_OK || status != 200)
        tul::Alert({"Failed to GET data from URL: ", url});

    return data;
}
