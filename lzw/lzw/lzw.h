/**
 * \brief compression.lzw - lzw compression
 * \author dove-zp
 * \contact https://github.com/dove-zp
 * \version 2018/JULY/22
 * \license MIT License (Expat) 
 */

#pragma once

#include <map>
#include <string>

//--------------------------------------------------------------------------------------------------

namespace compression
{
    namespace lzw
    {
        template <typename Iterator>
        Iterator compress(const std::string& uncompressed, Iterator result)
        {
            unsigned __int16 dict_size = 256;
            std::map<std::string, unsigned __int32> frequency;
            for (unsigned __int16 i = 0; i < 256; i++)
            {
                frequency[std::string(1, i)] = i;
            }
            std::string w;
            for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it)
            {
                char c = *it;
                std::string wc = w + c;
                if (frequency.count(wc))
                {
                    w = wc;
                }
                else
                {
                    *result++ = frequency[w];
                    frequency[wc] = dict_size++;
                    w = std::string(1, c);
                }
            }
            if (!w.empty())
            {
                *result++ = frequency[w];
            }
            return result;
        }

        template <typename Iterator>
        std::string decompress(Iterator begin, Iterator end)
        {
            unsigned __int16 dict_size = 256;
            std::map<unsigned __int32, std::string> frequency;
            for (unsigned __int16 i = 0; i < 256; i++)
            {
                frequency[i] = std::string(1, i);
            }
            std::string w(1, *begin++);
            std::string result = w;
            std::string entry;
            for (; begin != end; ++begin)
            {
                unsigned __int32 k = *begin;
                if (frequency.count(k))
                {
                    entry = frequency[k];
                }
                else if (k == dict_size)
                {
                    entry = w + w[0];
                }
                else
                {
                    throw("<!> ERROR: bad compression usage of k");
                }
                result += entry;
                frequency[dict_size++] = w + entry[0];
                w = entry;
            }
            return result;
        }
    }
}
