#include "ladder.h"
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>

void error(string word1, string word2, string msg) {
    cerr << word1 << " and " << word2 << ": " << msg << std::endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();

    // table to store results of subproblems
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    // fill the known entries
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }

    // fill the rest of dp[][]
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min( { dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1] } );
            }
        }
    }

    return dp[len1][len2] <= d;
}

bool is_adjacent(const std::string& word1, const std::string& word2) {
    if (word1 == word2) { return true; } // true if words are the same
    int len1 = word1.length();
    int len2 = word2.length();
    if (std::abs(len1 - len2) > 1) { return false; } // false if lengths differ by more than one letter

    int diff_count = 0;  // num diff chars

    // index for both words
    size_t i = 0;
    size_t j = 0; 

    while (i < len1 && j < len2) {
        if (word1[i] != word2[j]) {
            diff_count++;  // found a difference
            if (diff_count > 1) { return false; }

            // if lengths differ, advance longer word's index
            if (len1 > len2) {
                i++; // deleted letter (word1 longer)
            } else if (len2 > len1) {
                j++; // added letter (word2 longer)
            } else { 
                // replace letter (same length)
                i++; 
                j++; 
            }  
        } else {
            i++; 
            j++; // move normally if characters match
        }
    }

    // if there's an extra char at the end
    if (i < len1 || j < len2) diff_count++;

    return diff_count == 1;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    std::queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    std::set<std::string> visited;
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        std::string last_word = ladder.back();
        for (const std::string& word : word_list) {
            if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);

                if (word == end_word) {
                    return new_ladder;
                }

                ladder_queue.push(new_ladder);
                visited.insert(word);
            }
        }
    }

    return {}; // no ladder found
}

void load_words(set<string> & word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
    }

    std::string word;
    while (file >> word) {
        word_list.insert(word);
    }
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        std::cout << "No word ladder found." << std::endl;
        return;
    }

    std::cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i) {
        std::cout << ladder[i] << " ";
    }
    std::cout << std::endl;
}


#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
    /*
    g++ ladder.cpp ladder_main.cpp -o main
    */
}

