/**
 * \brief compression.huffman - huffman compression encoding and decoding
 * \author dove-zp
 * \contact https://github.com/dove-zp
 * \version 2018/AUG/01
 * \license MIT License (Expat) 
 */

#pragma once

#include <map>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------------------------

namespace compression
{
    namespace huffman
    {
        struct huffman_node
        {
            char character;
            unsigned __int32 character_frequency;
            struct huffman_node* left;
            struct huffman_node* right;

            huffman_node(char character, unsigned __int32 character_frequency, struct huffman_node* left = nullptr,
                         struct huffman_node* right = nullptr) : character(character), character_frequency(character_frequency), left(left), right(right)
            {
            }

            ~huffman_node()
            {
                delete left;
                delete right;
            }

            class compare
            {
            public:
                bool operator()(huffman_node* a, huffman_node* b)
                {
                    return a->character_frequency > b->character_frequency;
                }
            };
        };

        huffman_node* build_tree(std::vector<std::pair<char, unsigned __int32>>& alph);

        void print_tree(huffman_node* t);

        std::map<char, std::vector<bool>> build_lookup_table(huffman_node* htree);

        std::vector<bool> encode(std::string input, std::map<char, std::vector<bool>>& lookup);

        char code_lookup(std::vector<bool>::iterator& biter, const std::vector<bool>::iterator& biter_end, const huffman_node* htree);

        std::string decode(std::vector<bool>& compressed, const huffman_node* htree);

        std::vector<std::pair<char, unsigned __int32>> make_freq_table(std::string inp);

        std::string bitvec_to_string(std::vector<bool>& bitvec);

        std::vector<bool> string_to_bitvec(std::string packed);
    }
}
