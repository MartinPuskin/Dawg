//
// Created by Martin on 06.05.2024.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <bitset>

using namespace std;

class Dawg {
private:
    vector<int> m_dawg;

    static vector<int> readIntegers(const string &failiNimi);

    static bool kasLasteLopp(int tipp);

    static bool kasSonaLopp(int tipp);

    static char misTaht(int tipp);

    static int misLaps(int tipp);

public:
    Dawg(string failiNimi) : m_dawg(readIntegers(failiNimi)) {}

    bool kasSona(string sona, int algus);
};

vector<int> Dawg::readIntegers(const string &failiNimi) {
    ifstream input(failiNimi, ios::binary);
    if (!input.is_open()) {
        cerr << "Error: Cannot open file " << failiNimi << '\n';
        return {};
    }

    vector<int> integers;
    char buffer[4]; // Buffer to hold 4 bytes
    while (input.read(buffer, sizeof(buffer))) {
        // Interpret buffer as an int (assuming little-endian)
        int value = *reinterpret_cast<int *>(buffer);
        integers.push_back(value);
    }

    input.close();
    return integers;
}

bool Dawg::kasLasteLopp(int tipp) {
    if ((tipp & (1 << 28)) == 0) return false;
    return true;
}

bool Dawg::kasSonaLopp(int tipp) {
    if ((tipp & (1 << 29)) == 0) return false;
    return true;
}

char Dawg::misTaht(int tipp) {
    return static_cast<char>(tipp & 255);
}

int Dawg::misLaps(int tipp) {
    return (tipp & 268435200) >> 8;
}

string capitalizeAndReplace(string str) {
    // Capitalize the string
    for (char &c : str) {
        c = std::toupper(c);
    }

    // Replace '&' with '$'
    size_t pos = 0;
    while ((pos = str.find('&', pos)) != std::string::npos) {
        str.replace(pos, 1, "$");
        pos += 1; // Move past the replaced character
    }
    return str;
}

// ÕÄÖÜŠŽ = QXCY$W
// väike š on &
// Kontrollib, kas käidud sõna on korrektne sõna.
// dawg on graaf, algus on rekursiooni jaoks esimene indeks, esimest korda panna
// selle väärtuseks 2, sona on otsitav sõna
bool Dawg::kasSona(string sona, int algus = 2) {
    sona = capitalizeAndReplace(sona);
    for (int i{algus}; i < m_dawg.size(); i++) {
        if (sona.length() > 1 && sona[0] == misTaht(m_dawg[i]) && kasSona(sona.substr(1), misLaps(m_dawg[i]) + 1)) {
            return true;
        }
        if (sona.length() == 1 && sona[0] == misTaht(m_dawg[i]) && kasSonaLopp(m_dawg[i])) {
            return true;
        }
        if (kasLasteLopp(m_dawg[i]))
            return false;
    }
    cerr << "Ei leidnud sõnaraamatut.";
    return false;
}

// ÕÄÖÜŠŽ = QXCY$W
int main(){
    Dawg dog{"../DAWG/cmake-build-debug/SonaList.dat"};
    vector<string> test{"$ahh", "šahhi", "CXK"};
    for (string str : test){
        if (dog.kasSona(str))
            cout << str << " on sõna.\n";
        else
            cout << str << " ei ole sõna.\n";
    }
    return 0;
}
