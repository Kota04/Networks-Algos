#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring> // For memcpy
#include <fstream>
#include <unistd.h>
#include <bits/stdc++.h>

// Maximum number of pages the cache can store
int MAX_CACHE_SIZE = 5;

// Structure to store cached web pages
struct WebPage {
    std::string url;
    std::string content;
};

// Custom comparison function for unordered_map to store WebPage objects
struct WebPageHasher {
    size_t operator()(const WebPage& page) const {
        return std::hash<std::string>()(page.url);
    }
};

struct WebPageEqualFn {
    bool operator()(const WebPage& lhs, const WebPage& rhs) const {
        return lhs.url == rhs.url;
    }
};

// Define a global cache using an unordered_map and a linked list
std::unordered_map<WebPage, std::list<WebPage>::iterator, WebPageHasher, WebPageEqualFn> webPageCache;
std::list<WebPage> cacheList;

// Function to perform an HTTP GET request and return the content using sockets
std::string HttpGet(const std::string& url) {
    // Parse the URL to extract host and path
    size_t hostStart;
    if(url.find("://") != std::string::npos) {
        hostStart = url.find("://") + 3;
    } else {
        hostStart = 0;
    }
    size_t hostEnd = url.find("/", hostStart);
    std::string host = url.substr(hostStart, hostEnd - hostStart);
    std::string path = (hostEnd == std::string::npos) ? "/" : url.substr(hostEnd);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return "";
    }

    // Resolve host name to IP address
    struct hostent* server = gethostbyname(host.c_str());
    if (server == nullptr) {
        std::cerr << "Error resolving host" << std::endl;
        close(sockfd);
        return "";
    }

    // Set up socket address structure
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        return "";
    }

    // Construct the HTTP GET request
    std::string request = "GET " + path + " HTTP/1.1\r\n"
                          "Host: " + host + "\r\n"
                          "Connection: close\r\n\r\n";

    // Send the request
    if (send(sockfd, request.c_str(), request.length(), 0) == -1) {
        std::cerr << "Error sending request" << std::endl;
        close(sockfd);
        return "";
    }

    // Receive and store the response
    std::string response;
    char buffer[4096];
    int bytesRead;
    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytesRead);
    }

    // Close the socket
    close(sockfd);

    // Extract and return the content from the response
    size_t contentStart = response.find("\r\n\r\n") + 4;
    if (contentStart != std::string::npos) {
        return response.substr(contentStart);
    } else {
        std::cerr << "Error parsing response" << std::endl;
        return "";
    }
}

// Function to retrieve a web page from the cache or fetch it if not present
std::string GetWebPage(const std::string& url) {
    auto it = webPageCache.find({url, ""}); // Search for the URL in the cache
    if (it != webPageCache.end()) {
        // Page is in the cache, move it to the front and return the content
        cacheList.splice(cacheList.begin(), cacheList, it->second);
        return it->second->content;
    }

    // Page is not in the cache, fetch it
    std::string content = HttpGet(url);
    // Add the fetched page to the cache
    WebPage newPage = {url, content};
    cacheList.push_front(newPage);
    webPageCache[newPage] = cacheList.begin();

    // If the cache size exceeds the limit, evict the least recently used page
    if (webPageCache.size() > MAX_CACHE_SIZE) {
        WebPage& lruPage = cacheList.back();
        webPageCache.erase(lruPage);
        cacheList.pop_back();
    }

    return content;
}

// Function to display the contents of the cache
void DisplayCache() {
    std::cout << "Cache size: " << MAX_CACHE_SIZE << std::endl;
    std::cout << "Cache Contents (Most to Least Recently Used):" << std::endl;
    int index = 1;
    for (const auto& page : cacheList) {
        std::cout << index << ". " << page.url << std::endl;
        index++;
    }
}

// Function to save content to a temporary file and open it in the web browser
void OpenInBrowser(const std::string& content) {
    std::ofstream tempFile("temp.html");
    if (!tempFile) {
        std::cerr << "Error creating temporary file" << std::endl;
        return;
    }
    tempFile << content;
    tempFile.close();

    // Use xdg-open to open the file in the default web browser
    system("xdg-open temp.html");
}

int main() {
    std::string userInput;
    std::cout << "Enter Cache Size: ";
    int cacheSize;
    std::cin >> cacheSize;
    MAX_CACHE_SIZE = cacheSize;
    while (true) {
        std::cout << "Enter URL (or 'exit' to quit): ";
        std::cin >> userInput;
        if (userInput == "exit") {
            break;
        }

        std::string content = GetWebPage(userInput);
        if (!content.empty()) {
            std::cout << "Page content:\n" << content << std::endl;
            DisplayCache();

            // Open the content in the web browser
            //OpenInBrowser(content);
        }
    }

    return 0;
}
