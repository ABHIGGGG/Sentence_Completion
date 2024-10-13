#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

using namespace std;

// Trie Node Structure
class TrieNode {
public:
    TrieNode* children[26]; // Array for 26 lowercase letters
    bool isEndOfWord; // Indicates if a word ends at this node
    string fullSentence; // Store the complete sentence for this node

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; ++i) {
            children[i] = nullptr; // Initialize all children to nullptr
        }
    }
};

// Trie Class
class Trie {
private:
    TrieNode* root;

    int charToIndex(char c) {
        return c - 'a'; // Convert character to array index (assuming lowercase letters)
    }

    // Recursive function to find words starting with a given prefix
    void findWords(TrieNode* node, const string& prefix, vector<string>& suggestions) {
        if (node->isEndOfWord) {
            suggestions.push_back(node->fullSentence); // Add the full sentence to suggestions
        }
        for (int i = 0; i < 26; ++i) {
            if (node->children[i] != nullptr) {
                findWords(node->children[i], prefix, suggestions);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a sentence into the trie
    void insert(const string& sentence) {
        TrieNode* node = root;
        for (char c : sentence) {
            if (c == ' ') continue; // Ignore spaces for indexing
            int index = charToIndex(c);
            if (node->children[index] == nullptr) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true; // Mark the end of a word
        node->fullSentence = sentence; // Store the complete sentence at this node
    }

    // Autocomplete function to suggest words based on a prefix
    vector<string> autocomplete(const string& prefix) {
        TrieNode* node = root;
        vector<string> suggestions;

        // Traverse the Trie based on the prefix
        for (char c : prefix) {
            if (c == ' ') continue; // Ignore spaces
            int index = charToIndex(c);
            if (node->children[index] == nullptr) {
                return suggestions; // No suggestions if the path does not exist
            }
            node = node->children[index];
        }

        // Find all words that start with the given prefix
        findWords(node, prefix, suggestions);
        return suggestions;
    }
};

// Main function
int main() {
    Trie trie; // Instantiate the Trie

    // Read sentences from a file
    ifstream file("wikipedia.txt");
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        trie.insert(line); // Use the trie object to insert sentences
    }
    file.close();

    // Search for prefixes and get completions
    string input;
    while (true) {
        cout << "\nEnter a prefix to autocomplete (or type 'exit' to quit): ";
        getline(cin, input);

        if (input == "exit") {
            break; // Exit the loop if the user types 'exit'
        }

        auto results = trie.autocomplete(input);
        cout << "Autocomplete suggestions for '" << input << "':\n";
        if (results.empty()) {
            cout << " - No suggestions found.\n";
        } else {
            for (const string& suggestion : results) {
                cout << " - " << suggestion << "\n"; // Display each suggestion
            }
        }
    }

    return 0;
}
