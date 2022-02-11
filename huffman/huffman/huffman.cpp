/**
 * \brief compression.huffman - huffman compression encoding and decoding
 * \author dove-zp
 * \contact https://github.com/dove-zp
 * \version 2018/AUG/01
 * \license MIT License (Expat) 
 */

#include "huffman.h"

#include <queue>
#include <iostream>
#include <stdexcept>

//--------------------------------------------------------------------------------------------------

compression::huffman::huffman_node* compression::huffman::build_tree(std::vector<std::pair<char, unsigned __int32>>& alph)
{
    std::priority_queue<huffman_node *, std::vector<huffman_node *>, huffman_node::compare> alph_heap;
    for (std::vector<std::pair<char, unsigned __int32>>::iterator it = alph.begin(); it != alph.end(); ++it)
    {
        huffman_node* leaf = new huffman_node(it->first, it->second);
        alph_heap.push(leaf);
    }
    huffman_node* root = nullptr;
    while (alph_heap.size() > 1)
    {
        huffman_node* left = alph_heap.top();
        alph_heap.pop();
        huffman_node* right = alph_heap.top();
        alph_heap.pop();
        root = new huffman_node(0, left->character_frequency + right->character_frequency, left, right);
        alph_heap.push(root);
    }

    return root;
}

void compression::huffman::print_tree(huffman_node* t)
{
    std::deque<std::pair<huffman_node *, __int32>> deq_tree;

    deq_tree.emplace_back(t, 0);
    __int32 curlevel = -1;
    while (!deq_tree.empty())
    {
        huffman_node* parent = deq_tree.front().first;
        signed __int32 level = deq_tree.front().second;
        deq_tree.pop_front();
        if (curlevel != level)
        {
            curlevel = level;
            std::cout << "Level: " << curlevel << std::endl;
        }
        std::cout << parent->character_frequency << " " << parent->character << std::endl;
        if (parent->left)
        {
            deq_tree.emplace_back(parent->left, level + 1);
        }
        if (parent->right)
        {
            deq_tree.emplace_back(parent->right, level + 1);
        }
    }
}

std::map<char, std::vector<bool>> compression::huffman::build_lookup_table(huffman_node* htree)
{
    std::map<char, std::vector<bool>> lookup;
    std::deque<std::pair<huffman_node *, std::vector<bool>>> deq_tree;

    deq_tree.emplace_back(htree, std::vector<bool>());
    while (!deq_tree.empty())
    {
        huffman_node* lc;
        huffman_node* rc;
        huffman_node* node = deq_tree.front().first;
        std::vector<bool> code = deq_tree.front().second;
        deq_tree.pop_front();
        lc = node->left;
        rc = node->right;
        if (lc)
        {
            std::vector<bool> code_cp(code);
            deq_tree.emplace_back(lc, (code.push_back(false), code));
            deq_tree.emplace_back(rc, (code_cp.push_back(true), code_cp));
        }
        else
        {
            lookup.insert(std::make_pair(node->character, code));
        }
    }

    return lookup;
}

std::vector<bool> compression::huffman::encode(std::string input, std::map<char, std::vector<bool>>& lookup)
{
    std::vector<bool> result;

    for (std::string::iterator it = input.begin(); it != input.end(); ++it)
    {
        std::vector<bool> b_vector = lookup[*it];
        result.insert(result.end(), b_vector.begin(), b_vector.end());
    }

    return result;
}

char compression::huffman::code_lookup(std::vector<bool>::iterator& biter, const std::vector<bool>::iterator& biter_end, const huffman_node* htree)
{
    const huffman_node* node = htree;

    while (true)
    {
        if (!node->left)
        {
            break;
        }
        if (biter == biter_end)
        {
            throw std::out_of_range("<!> ERROR: no more bits to iterate");
        }
        if (*biter)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
        ++biter;
    }

    return node->character;
}

std::string compression::huffman::decode(std::vector<bool>& compressed, const huffman_node* htree)
{
    std::string result;

    std::vector<bool>::iterator biter = compressed.begin();
    while (true)
    {
        try
        {
            result += code_lookup(biter, compressed.end(), htree);
        }
        catch (const std::out_of_range& oor)
        {
            break;
        }
    }

    return result;
}

std::vector<std::pair<char, unsigned __int32>> compression::huffman::make_freq_table(std::string inp)
{
    std::map<char, unsigned __int32> char_frequency_map;
    std::vector<std::pair<char, unsigned __int32>> char_frequency_vector;

    for (char& i : inp)
    {
        if (char_frequency_map.find(i) == char_frequency_map.end())
        {
            char_frequency_map.insert(std::make_pair(i, 1));
        }
        char_frequency_map[i] += 1;
    }

    for (std::map<char, unsigned __int32>::iterator it = char_frequency_map.begin(); it != char_frequency_map.end(); ++it)
    {
        char_frequency_vector.emplace_back(it->first, it->second);
    }

    return char_frequency_vector;
}

std::string compression::huffman::bitvec_to_string(std::vector<bool>& bitvec)
{
    std::string result;

    size_t number_bits = bitvec.size() & 7;

    result += static_cast<char>(number_bits);

    char byte = 0;
    for (unsigned __int32 i = 0; i < bitvec.size(); ++i)
    {
        unsigned __int32 byte_offset = i & 7;
        byte |= bitvec[i] << byte_offset;
        if (byte_offset == 7)
        {
            result += byte;
            byte = 0;
        }
    }
    if (number_bits)
    {
        result += byte;
    }

    return result;
}

std::vector<bool> compression::huffman::string_to_bitvec(std::string packed)
{
    std::vector<bool> result;

    if (packed.size() == 1)
    {
        return result;
    }

    unsigned __int32 number_bits = static_cast<unsigned __int32>(packed[0]);
    for (std::string::iterator it = packed.begin() + 1; it != packed.end(); ++it)
    {
        for (unsigned __int8 i = 0; i < 8; ++i)
        {
            result.push_back((*it >> i) & 1);
        }
    }

    if (number_bits)
    {
        for (unsigned __int32 i = 0; i < (8 - number_bits); ++i)
        {
            result.pop_back();
        }
    }
    return result;
}
