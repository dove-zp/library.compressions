/**
 * \brief compression.lz78 - lz78 compression
 * \author dove-zp
 * \contact https://github.com/dove-zp
 * \version 2018/AUG/01
 * \license MIT License (Expat) 
 */



#pragma once

#include <map>
#include <string>
#include <bitset>

//--------------------------------------------------------------------------------------------------

namespace compression
{
    namespace lz78
    {
        std::string compress(const std::string& uncompressed)
        {
            std::map<std::string, unsigned __int32> frequency;
            for (unsigned __int16 i = 0; i < 256; i++)
            {
                frequency[std::string(1, i)] = i;
            }

            std::string w;
            std::string result;
            for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it)
            {
                char c = *it;

                if (frequency.count(w + c))
                {
                    w += c;
                }
                else
                {
                    if (!w.empty())
                    {
                        result += std::bitset<8>(frequency[w]).to_string();
                    }
                    else
                    {
                        result += "00000000";
                    }
                    result += std::bitset<8>(c).to_string();
                    frequency[w + c] = frequency.size() + 1;
                    w.clear();
                }
            }
            return result;
        }

        std::string decompress(const std::string& compressed)
        {
            std::map<unsigned __int32, std::string> frequency;
            for (unsigned __int16 i = 0; i < 256; i++)
            {
                frequency[i] = std::string(1, i);
            }

            int k;
            std::string result;
            for (size_t x = 0; x < compressed.length(); ++++x)
            {
                std::bitset<8> dec(compressed[x]);
                std::string c = compressed.substr(x + 1, 1);

                if (k != 0)
                {
                    result += frequency[dec.to_ulong()];
                }
                result += c;

                if (dec.to_ulong() != 0)
                {
                    frequency[frequency.size() + 1] = frequency[dec.to_ulong()] + c;
                }
                else
                {
                    frequency[frequency.size() + 1] = c;
                }
            }
            return result;
        }
    }
}
