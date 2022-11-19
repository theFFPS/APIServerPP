#pragma once
#ifndef __HASH__HH__
#define __HASH__HH__
#include <iostream>
#include <regex>
#include <cmath>
using namespace std;
class Hash {
    private:
        string hash;
        unsigned int length = 64;
        vector<string> split (string &s, char delim) {
            vector<string> result;
            stringstream ss (s);
            string item;
            while (getline (ss, item, delim)) result.push_back (item);
            return result;
        }
        unsigned int iForSalt = 0;
        void GenHash(string text, string salt) {
            unsigned int minLen = 2;
            unsigned int realMinLen = 0;
            length -= salt.size();
            length--;
            unsigned int strLenOrig = text.size();
            while (minLen <= length) realMinLen = (minLen *= 2);
            while (minLen < strLenOrig) minLen *= 2;
            if ((minLen - strLenOrig) < minLen) minLen *= 2;
            unsigned int addCount = minLen - strLenOrig;
            for (int n = 0; n < (int)addCount; n++) text += ReceivingExistCodes(text[n] + text[n+1]);
            while (text.size() != realMinLen) {
                for (int i = 0, center = text.size() / 2; i < center; i++) hash += ReceivingExistCodes(text[center - i] + text[center + 1]);
                text = hash;
                hash.clear();
            }
            unsigned int rem = realMinLen - length;
            for (unsigned int i = 0, countCompress = realMinLen / rem; hash.size() < length; i++) {
                if (i % countCompress == 0) { hash += ReceivingExistCodes(text[i] + text[i+1]); i++; }
                else hash += text[i];
            }
            string cache = hash;
            hash = salt + "$" + cache;
        }
        const char *dictionary = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM1234567890";
        string Salt(string seed) {
            unsigned int addition = 0;
            for (int i = 0; i < (int)seed.size(); i++) addition += seed[i];
            srand(time(NULL)+iForSalt+addition);
            iForSalt++;
            if (iForSalt == 100) iForSalt = 0;
            string out;
            for (int i = 0; i != (int)length / 2; i++) out += dictionary[rand() % 62];
            return out;
        }
        int ReceivingExistCodes(int x) {
            x += 256;
            while (!(((x <= 57) && (x >= 48)) || ((x <= 90) && (x >= 65)) || ((x <= 122) && (x >= 97)))) { if (x < 48) x += 24; else x -=47; } 
            return x; 
        }
    public:
        string GetHash(string text, unsigned int length);
        bool Validate(string text, string hashTT);
};
#endif