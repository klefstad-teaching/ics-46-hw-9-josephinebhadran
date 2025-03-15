#include "ladder.h"
#include <queue>
#include <set>
#include <algorithm>

void error(string word1, string word2, string msg) {

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

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // why does this cause an infinite loop????????
    std::queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    std::set<std::string> visited;
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        std::string last_word = ladder.back();
        // std::cout << "processing word: " << last_word << std::endl;
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
        // for (std::string word : word_list) {
        //     std::cout << "checking adjacency with word: " << word << std::endl;
        //     if (is_adjacent(last_word, word)) {
        //         std::cout << "reached inside the if statement lmfao" << std::endl;
        //         if (visited.find(word) == visited.end()) {
        //             visited.insert(word);
        //             vector<string> new_ladder = ladder;
        //             new_ladder.push_back(word);
        //             if (word == end_word) {
        //                 return new_ladder;
        //             }
        //             std::cout << "pushing word: " << word << std::endl;
        //             ladder_queue.push(new_ladder);
        //         }
        //     }
        // }
    }

    return {}; // no ladder found
}

void load_words(set<string> & word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        // error("unable to open input file");
        std::cerr << "Error opening file." << std::endl;
    }

    std::string word;
    while (file >> word) {
        word_list.insert(word);
    }
}

void print_word_ladder(const vector<string>& ladder) {
    for (std::string word : ladder) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}


#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    // vector<string> lad = generate_word_ladder("cat", "mat", word_list);
    // print_word_ladder(lad);
    // print_word_ladder((generate_word_ladder("cat", "dog", word_list)));
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

