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
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}




class OpenAi_Enemy{
    private:
        std::string apiKey = ""; // 请替换为你的API密钥

        
        std::string model = "gpt-4o";
        std::string temperature = "0.0";

        
        // 呼叫 OpenAI API，傳遞題詞並獲取 AI 回應
        std::string callOpenAI(const std::string &prompt)
        {
            CURL *curl;
            CURLcode res;
            std::string content = "";

            // 初始化 CURL
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (curl)
            {
                std::string readBuffer;
                curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

                // 設置請求標頭
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, "Content-Type: application/json");
                headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                // 構建 JSON 請求數據
                nlohmann::json payload;
                payload["model"] = model;
                payload["messages"] = {{"role", "user"}, {"content", prompt}};
                payload["temperature"] = temperature;

                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.dump().c_str());

                // 設置回應處理
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                // cout payload
                std::cout << payload.dump() << std::endl;
                // 發送請求
                res = curl_easy_perform(curl);

                if (res == CURLE_OK)
                {
                    try
                    {
                        // 解析 JSON 回應
                        json responseJson = json::parse(readBuffer);
                        if (responseJson.contains("choices") && !responseJson["choices"].empty() &&
                            responseJson["choices"][0].contains("message") &&
                            responseJson["choices"][0]["message"].contains("content"))
                        {
                            content = responseJson["choices"][0]["message"]["content"];
                        }
                        else
                        {
                            std::cerr << "Unexpected JSON format" << std::endl;
                            std::cerr << responseJson.dump(4) << std::endl; // 輸出 JSON 內容
                        }
                    }
                    catch (const json::exception &e)
                    {
                        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    }
                }
                else
                {
                    std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
                }

                // 清理
                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();
            return content;
        }
    
        // 根據規則與場上狀態生成 GPT 的提示詞
        std::string generateAITurn(std::string &publicZone, std::string &cardZone, std::string &handZone, int maxCardsPerTurn){
            // 構建提示詞
            std::string prompt = "以下是遊戲規則與場上狀態：\n\n";
            prompt += "1. 遊戲目標：\n";
            prompt += "   - 從手牌中選擇最多 " + std::to_string(maxCardsPerTurn) + " 張牌，放入出牌區域，最大化得分。\n";
            prompt += "   - 遊戲會根據計分邏輯進行得分計算。\n\n";
            prompt += "2. 計分規則：\n";
            prompt += "   - 同色順序: 120 分 + 卡片總點數（同花色且連續的牌）。\n";
            prompt += "   - 四騎士: 80 分 + 卡片總點數（四張相同點數的牌）。\n";
            prompt += "   - 同色: 70 分 + 卡片總點數（至少五張同花色的牌）。\n";
            prompt += "   - 序列: 60 分 + 卡片總點數（五張連續的牌，不限花色）。\n";
            prompt += "   - 滿座: 55 分 + 卡片總點數（三張相同 + 兩張相同點數的牌）。\n";
            prompt += "   - 雙偶星: 30 分 + 卡片總點數(兩組兩張相同點數的牌，且兩組點數不相同)。\n";
            prompt += "   - 三賢者：30 分 + 卡片總點數(三張相同的牌，且其他牌沒辦法組出滿座)。\n";
            prompt += "   - 偶星：15 分 + 卡片總點數(兩張相同點數的牌)。\n";
            prompt += "   - 若無牌型，僅計算所有卡片的總點數。\n\n";
            prompt += "3. 策略建議：\n";
            prompt += "   - 優先構成高分牌型（同色順序 > 四騎士 > 同色 > 序列 > 滿座 > 雙偶星 = 三賢者 > 偶星）。\n";
            prompt += "   - 若無法構成高分牌型，選擇點數最大的卡片組合。\n";
            prompt += "4. 場上狀態：\n";
            prompt += "   - 公共區域: " + publicZone + "\n";
            prompt += "   - 出牌區域: " + cardZone + "\n";
            prompt += "   - 手牌區域: " + handZone + "\n\n";
            prompt += "請根據以上資訊，返回選擇的手牌索引，回應格式需符合以下正則表達式：\n";
            prompt += "   - 正則表達式: ^(\\d+\\s?)*$\n";
            prompt += "   - 僅包含索引數字，並以空格分隔，例如 \"1 3\" 或 \"2\"，無其他文字或標點符號。";
            return prompt;
        }
    
    public:
        std::vector<std::shared_ptr<Card>> handcards;  // 須由外部設定
        std::string publicZone; // 須由外部設定
        std::string cardZone;  // 須由外部設定
        std::string handZone;
        std::vector<int> aiMove(){
            handZone = "";
            for (auto& card : handcards){
                handZone += card->to_string() + " ";
            } // handZone 初始化

            std:: cout << "handZone: " << handZone << std::endl;
            std:: cout << "publicZone: " << publicZone << std::endl;
            std:: cout << "cardZone: " << cardZone << std::endl;


            

            // 呼叫 API 並獲取回應
            std::string prompt = generateAITurn(publicZone, cardZone, handZone, 2);
            std::string aiResponse = callOpenAI(prompt);
            // std::cout << "Prompt: " << prompt << std::endl; // 輸出提示詞
            std::cout << "AI 的回應（索引）: " << aiResponse << std::endl;

            // 解析 AI 回應的索引
            std::istringstream responseStream(aiResponse);
            std::vector<int> indices;
            int index;
            while (responseStream >> index)
            {
                indices.push_back(index - 1); // 索引從 1 開始，轉換為 0 索引
            }
            return indices;
        }
};