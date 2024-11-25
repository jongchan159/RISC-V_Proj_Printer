#include <iostream>
#include <fstream>
#include "toDecimal.h"
#include "formatType.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input-file>" << endl;
        return 1;
    }

    // 명령줄 인수로 전달된 파일 이름
    string filename = argv[1];

    // filename을 사용하여 파일을 열고 처리
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    int instnum = 0;
    unsigned int instruction;
    
    // 각 32비트 명령어를 읽어온 후 처리
    while (file.read(reinterpret_cast<char*>(&instruction), sizeof(unsigned int))) {
        // 32비트 명령어를 16진수 형식으로 출력
        cout << "inst " << instnum << ": " << hex << setw(8) << setfill('0') << instruction << " ";
        
        // 명령어 해독 함수 호출
        whatType(&instruction);

        instnum++;
    }

    file.close();

    return 0;
}