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

    static bool samaTaht(wchar_t taht1, wchar_t taht2);

public:
    Dawg(string failiNimi) : m_dawg(readIntegers(failiNimi)) {}

    bool kasSona(vector<int> dawg, int algus, wchar_t *sona);
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

bool Dawg::samaTaht(wchar_t taht1, wchar_t taht2) {
    int arv1 = static_cast<int>(taht1);
    int arv2 = static_cast<int>(taht2);

    if (arv1 > 1000) {
        arv1 -= 65536;
    }
    if (arv2 > 1000) {
        arv2 -= 65536;
    }

    //tavalised tähed
    if (0 < arv1 && arv1 < 256 && 0 < arv2 && arv2 < 256) {
        return arv1 == arv2;
    }
    //täpitähed
    if (arv1 < 256 && arv2 < 256) {
        return (arv1 - arv2) % 256 == 0;
    }
    //š ja ž
    if (arv1 < 0 && arv2 > 256 || arv1 > 256 && arv2 < 0) {
        return (arv1 + arv2 == 284 || arv1 + arv2 == 251);
    }
    return false;
}

// Kontrollib, kas käidud sõna on korrektne sõna.
// dawg on graaf, algus on rekursiooni jaoks esimene indeks, esimest korda panna
// selle väärtuseks 2, sona on otsitav sõna
bool Dawg::kasSona(vector<int> dawg, int algus, wchar_t sona[]) {
    for (int i{algus}; i < dawg.size(); i++) {
        if (wcslen(sona) > 1 && samaTaht(sona[0], misTaht(dawg[i])) && kasSona(dawg, misLaps(dawg[i]) + 1, &sona[1])) {
            return true;
        }
        if (wcslen(sona) == 1 && samaTaht(sona[0], misTaht(dawg[i])) && kasSonaLopp(dawg[i])) {
            return true;
        }
        return !kasLasteLopp(dawg[i]);
    }
    cerr << "Ei leidnud sõnaraamatut.";
    return false;
}


int main(){

    return 0;
}
