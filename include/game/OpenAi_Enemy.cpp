#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "Card.cpp"
#pragma once

using json = nlohmann::json;

class OpenAi_Enemy{
    private:
        std::string apiKey = "sk-proj-DtUyFyKtFZRa4mOEZKUTep1EdNo4dPlwGipqJBeCu_MAu_v7k_DvZ9BZYveVDbM_PVqCsWLR7oT3BlbkF4cRO0pSo2DH4IgA5F7p9hG2vfmfbRgkHr_jcR_HkWLBqz1HYpEBvkOLD8RYyVBozUzJWM4A";
        std::string model = "gpt-4o";
        std::string temperature = "0.0";

        size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
        std::string callOpenAI(const std::string& prompt) { // 
            CURL* curl;
            CURLcode res;
            std::string content = "";
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            
            if(curl) {
                std::string readBuffer;
                curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, "Content-Type: application/json");
                headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                std::string jsonData = "{\"model\": \"  "+model+"  \", "
                                    "\"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}],"
                                    "\"temperature\": "+temperature+"}";
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                res = curl_easy_perform(curl);

                if(res == CURLE_OK) {
                    try {
                        json responseJson = json::parse(readBuffer);
                        if (responseJson.contains("choices") && !responseJson["choices"].empty() &&
                            responseJson["choices"][0].contains("message") &&
                            responseJson["choices"][0]["message"].contains("content")) {
                            content = responseJson["choices"][0]["message"]["content"];
                        } else {
                            std::cerr << "Unexpected JSON format" << std::endl;
                        }
                    } catch (const json::exception& e) {
                        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
                }

                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();
            return content;
        }
    public:
        std::vector<std::shared_ptr<Card>> handcards;
        std::string publicZone;
        std::string cardZone;
        std::string handZone;
        std::vector<int> aiMove(){
            std::string answer = callOpenAI(""); // answer = 1 2 
            std::vector<int> ret;
            // strtok
            char* token = strtok(&answer[0], " ");
            while (token != nullptr) {
                ret.push_back(std::stoi(token));
                token = strtok(nullptr, " ");
            }
            return ret;
        }
};