#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <iostream>

#pragma once3

// 判定是否為序列 (至少五張點數連續的牌)
bool isStraight(const std::vector<int> &numbers)
{
    if (numbers.size() < 5)
        return false; // 少於五張牌無法構成序列

    std::vector<int> sortedNums = numbers;
    std::sort(sortedNums.begin(), sortedNums.end());
    sortedNums.erase(std::unique(sortedNums.begin(), sortedNums.end()), sortedNums.end()); // 去重

    int consecutiveCount = 1;
    for (size_t i = 1; i < sortedNums.size(); ++i)
    {
        if (sortedNums[i] == sortedNums[i - 1] + 1)
        {
            consecutiveCount++;
            if (consecutiveCount == 5)
                return true; // 找到連續的五張牌
        }
        else
        {
            consecutiveCount = 1; // 重置計數
        }
    }

    return false; // 沒有找到連續五張牌
}

bool isStraightFlush(const std::vector<int> &numbers, const std::vector<int> &suits)
{
    if (numbers.size() < 5)
        return false;

    // 分組花色
    std::map<int, std::vector<int>> suitGroups;
    for (size_t i = 0; i < suits.size(); ++i)
    {
        suitGroups[suits[i]].push_back(numbers[i]);
    }

    // 對每個花色檢查是否有連續
    for (const auto &group : suitGroups)
    {
        auto nums = group.second;
        if (nums.size() < 5)
            continue;

        // 處理 Ace 的高位
        std::sort(nums.begin(), nums.end());
        if (std::find(nums.begin(), nums.end(), 1) != nums.end())
        {
            nums.push_back(14); // Ace 作為高位
        }

        // 檢查是否連續
        int consecutiveCount = 1;
        for (size_t i = 1; i < nums.size(); ++i)
        {
            if (nums[i] == nums[i - 1] + 1)
            {
                consecutiveCount++;
                if (consecutiveCount == 5)
                    return true; // 找到同花順
            }
            else if (nums[i] != nums[i - 1]) // 非連續重置計數
            {
                consecutiveCount = 1;
            }
        }
    }

    return false;
}

// 判定是否為四騎士 (四張相同點數的牌)
bool isFourOfAKind(const std::vector<int> &numbers)
{
    std::map<int, int> countMap;
    for (int num : numbers)
        countMap[num]++;
    return std::any_of(countMap.begin(), countMap.end(), [](const std::pair<int, int> &p)
                       { return p.second == 4; });
}

// 判定是否為同色 (至少五張同花色)
bool isFlush(const std::vector<int> &suits)
{
    if (suits.size() < 5)
        return false; // 少於五張牌無法構成同色

    std::map<int, int> suitCount;
    for (int suit : suits)
    {
        suitCount[suit]++;
        if (suitCount[suit] >= 5)
            return true; // 找到至少五張同花色的牌
    }

    return false; // 沒有找到五張同花色的牌
}

// 判定是否為滿座 (三張相同 + 兩張相同)
bool isFullHouse(const std::vector<int> &numbers)
{
    std::map<int, int> countMap;
    for (int num : numbers)
        countMap[num]++;
    bool three = false, two = false;
    for (const auto &p : countMap)
    {
        if (p.second == 3)
            three = true;
        if (p.second == 2)
            two = true;
    }
    return three && two;
}

// 判定是否為雙偶星 (兩組兩張相同的牌)
bool isTwoPair(const std::vector<int> &numbers)
{
    std::map<int, int> countMap;
    for (int num : numbers)
        countMap[num]++;
    int pairCount = 0;
    for (const auto &p : countMap)
    {
        if (p.second == 2)
            pairCount++;
    }
    return pairCount == 2; // 必須恰好有兩對
}

// 判定是否為三賢者 (三張相同)
bool isThreeOfAKind(const std::vector<int> &numbers)
{
    std::map<int, int> countMap;
    for (int num : numbers)
        countMap[num]++;
    return std::any_of(countMap.begin(), countMap.end(), [](const std::pair<int, int> &p)
                       { return p.second == 3; });
}

// 判定是否為偶星 (一組兩張相同的牌)
bool isOnePair(const std::vector<int> &numbers)
{
    std::map<int, int> countMap;
    for (int num : numbers)
        countMap[num]++;
    return std::any_of(countMap.begin(), countMap.end(), [](const std::pair<int, int> &p)
                       { return p.second == 2; });
}

// 計算點數分 (所有牌號的總和)
int calculatePointSum(const std::vector<int> &numbers)
{
    return std::accumulate(numbers.begin(), numbers.end(), 0, [](int sum, int num)
                           {
        // 將 10, 11, 12, 13, 1 統一為 10 分
        if (num == 10 || num == 11 || num == 12 || num == 13 || num == 1)
        {
            return sum + 10;
        }
        return sum + num; });
}

// // 顯示牌和花色
// void displayCards(const std::vector<int> &numbers, const std::vector<int> &suits, const std::string &description)
// {
//     std::cout << description << ":\n";
//     for (size_t i = 0; i < numbers.size(); ++i)
//     {
//         std::cout << "牌 " << i + 1 << ": 點數=" << numbers[i] << ", 花色=" << suits[i] << "\n";
//     }
//     std::cout << std::endl;
// }

// 判定牌型
std::pair<int, std::string> determineHandType(
    const std::vector<int> &allNumbers,
    const std::vector<int> &allSuits)
{
    int pointSum = calculatePointSum(allNumbers);

    if (isStraightFlush(allNumbers, allSuits))
    {
        return {120 + pointSum, "同色順序"};
    }
    else if (isFourOfAKind(allNumbers))
    {
        return {80 + pointSum, "四騎士"};
    }
    else if (isFlush(allSuits))
    {
        return {70 + pointSum, "同色"};
    }
    else if (isStraight(allNumbers))
    {
        return {60 + pointSum, "序列"};
    }
    else if (isFullHouse(allNumbers))
    {
        return {55 + pointSum, "滿座"};
    }
    else if (isTwoPair(allNumbers))
    {
        return {30 + pointSum, "雙偶星"};
    }
    else if (isThreeOfAKind(allNumbers))
    {
        return {30 + pointSum, "三賢者"};
    }
    else if (isOnePair(allNumbers))
    {
        return {15 + pointSum, "偶星"};
    }
    else
    {
        return {pointSum, "無牌型"};
    }
}
//無牌型偶星三賢者雙偶星滿座序列同色四騎士同色順序
int countPoint(
    std::vector<int> &publicNumber, // 公共區域牌號 (1-13)
    std::vector<int> &publicSuit,   // 公共區域花色 (0-3)
    std::vector<int> &handNumber,   // 手牌牌號 (1-13)
    std::vector<int> &handSuit      // 手牌花色 (0-3)
)
{
    std::vector<int> allNumbers = publicNumber;
    std::vector<int> allSuits = publicSuit;
    allNumbers.insert(allNumbers.end(), handNumber.begin(), handNumber.end());
    allSuits.insert(allSuits.end(), handSuit.begin(), handSuit.end());

    // 新增檢查：如果沒有輸入任何牌，直接返回 "無牌型"
    if (allNumbers.empty() || allSuits.empty())
    {
        // std::cout << "所有牌的點數與花色:\n(無牌)\n";
        return 0; // 返回分數為0，並視為 "無牌型"
    }
    else
    {
        // displayCards(allNumbers, allSuits, "所有牌的點數與花色");
        return determineHandType(allNumbers, allSuits).first; // 使用 determineHandType 的返回值計算分數
    }
}
std::string countPoint_s(
    std::vector<int> &publicNumber, // 公共區域牌號 (1-13)
    std::vector<int> &publicSuit,   // 公共區域花色 (0-3)
    std::vector<int> &handNumber,   // 手牌牌號 (1-13)
    std::vector<int> &handSuit      // 手牌花色 (0-3)
)
{
    std::vector<int> allNumbers = publicNumber;
    std::vector<int> allSuits = publicSuit;
    allNumbers.insert(allNumbers.end(), handNumber.begin(), handNumber.end());
    allSuits.insert(allSuits.end(), handSuit.begin(), handSuit.end());

    // 新增檢查：如果沒有輸入任何牌，直接返回 "無牌型"
    if (allNumbers.empty() || allSuits.empty())
    {
        // std::cout << "所有牌的點數與花色:\n(無牌)\n";
        return "無牌型"; // 返回分數為0，並視為 "無牌型"
    }
    else
    {
        // displayCards(allNumbers, allSuits, "所有牌的點數與花色");
        return determineHandType(allNumbers, allSuits).second; // 使用 determineHandType 的返回值計算分數
    }
}