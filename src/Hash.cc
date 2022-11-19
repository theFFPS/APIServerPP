#include <Hash.hh>
string Hash::GetHash(string text, unsigned int length) {
    hash.clear();
    this->length = length;
    string salt = Salt(text); 
    text += salt;
    for (int i = 0; i < (int)text.size(); i++) text[i] = text[i]*2;
    if (length < 5) { cout << "Output hash length should be minimum 5 symbols!" << endl; exit(-1); }
    GenHash(text, salt); 
    return hash; 
}
bool Hash::Validate(string text, string hashTT) {
    hash.clear();
    for (int i = 0; i < (int)text.size(); i++) text[i] = text[i]*2;
    string hashT = hashTT;
    vector<string> saltAndHash = split(hashT, '$');
    string line;
    string salt = saltAndHash[0];
    this->length = hashTT.size(); 
    GenHash(text, salt);
    if (hashTT == hash) return true;
    return false;
}