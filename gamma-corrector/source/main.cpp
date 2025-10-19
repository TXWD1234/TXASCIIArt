#include "header.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
//#include "67.h"

std::filesystem::path getExePath() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer);
}
class pixel {
public:
	pixel(uint8_t in_first, uint8_t in_second) : first(in_first), second(in_second) {}
	uint8_t first = 0, second = 0;
	//bool operator<(pixel other) { return (this->first + this->second) < (other.first + other.second); }
	/*bool operator!=(pixel other) { return this->first != other.first || this->second != other.second; }
	bool operator==(pixel other) { return this->first == other.first && this->second == other.second; }*/
};
typedef uint8_t pixelIndex;



int main() {
	// 2px
	// width:  940
	// height: 295
	// 5px
	// width:  376
	// height: 118
	// 7px
	// width:  268
	// height: 84


	//for (int i = 0; i < 84; i++) {
	//	cout << "a\n";
	//}


	//cin.ignore();
	//cin.get();
	//return 0;




	






	//string ASCII = "@%#*+=-:. ";
	//string ASCII = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	//string ASCII = "@#&%$WM8B0QOZqpmdbkhao*+=-:. ";
	//string ASCII = "@&$%WM#B8Q0OZImkaioc*[+{(/-:;'`. ";
	//string ASCII = "QDMgW#HbOKmd45GAPUaeqSw3ZXVko2%hICyFL{}#ftj1TJn[]iusxYz7c=vl|+>(<?r)/\*!_\"^; ~:, '.-` ";


	//string ASCII = "QDMgW#* HbOKmd45GAPUaeqSw%CyFtjJ7c=vl|+>(<?r)/\*!_\"^; ~:, '.-` ";
	/*vector<string> pixels = {
		"QQ",
		"DD", "DS",
		"SS", "Sg",
		"gg", "gW",
		"WW", "g#",
		"||", "|#",
		"##", "##", "|*", "|,", "|,", "#,",
		"**", "*,", "*,",
		",,", ",,", "*^", ",^", ",^",
		"^^", "^^", "^^", "^=", "^=",
		"==", "=="
	};*/

	/*vector<string> selected = {
		"QQQ*",
		"QQQQQQQQQ*",
		"QQQQQQQQQQQQQQQQQ*",
		"QQQQQQQQQ*",
		"QQQ*"
	};
	vector<string> erased = {
		"    ",
		"          ",
		"                  ",
		"          ",
		"    "
	};*/
	vector<string> selected = {
		"QQQQQQQQQQQQQQQQQ*"
	};
	vector<string> erased = {
		"                  "
	};


	vector<pixel> pixels;

	string ASCII = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	std::ifstream ifsASCII(getExePath().parent_path() / "GammaCorrectorOutput.txt");
	if (ifsASCII.is_open()) {
		getline(ifsASCII, ASCII);
	}
	for (int i = 0; i < ASCII.size(); i++) {
		uint8_t Char = static_cast<uint8_t>(ASCII[i]);
		pixels.push_back({ Char, Char });
	}

	const int colorWidth = 1000;

	int colorHeight; cin >> colorHeight;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (colorHeight < 0) {
		colorHeight = std::abs(colorHeight);
		CHAR_INFO charTemplate;
		charTemplate.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		vector<CHAR_INFO> buffer(colorWidth * colorHeight * pixels.size(), charTemplate);
		int singleColorLen = colorHeight * colorWidth;
		for (int i = 0; i < ASCII.size(); i++) {
			for (int j = 0; j < singleColorLen; j++) {
				buffer[i * singleColorLen + j].Char.AsciiChar = ASCII[i];
			}
		}
		SMALL_RECT temp = { 0, 0, static_cast<short>(colorWidth + 20), static_cast<short>(ASCII.size() * colorHeight) };
		WriteConsoleOutputA(
			hConsole,
			buffer.data(),
			{ static_cast<short>(colorWidth), static_cast<short>(ASCII.size() * colorHeight) },
			{ 0, 0 },
			&temp
		);
		cin.ignore();
		cin.get();
		return 0;
	}






	DWORD writtenAmount;
	for (int j = 0; j < selected.size(); j++) {
		WriteConsoleOutputCharacterA(hConsole, selected[j].c_str(), selected[j].size(), { 0, static_cast<short>(j) }, &writtenAmount);
	}
	for (int i = 0; i < pixels.size(); i++) {
		string str(colorWidth, pixels[i].first);
		/*for (int j = 0; j < 939; j++) {
			str[2 * j] = pixels[i].first;
			str[2 * j + 1] = pixels[i].second;
		}*/
		for (int increment = 0; increment < colorHeight; increment++) {
			WriteConsoleOutputCharacterA(hConsole, str.c_str(), colorWidth, { 20, static_cast<short>(i * colorHeight + increment) }, &writtenAmount);
		}
	}




	int keyboardIn = _getch();
	int cursorIndex = 0;
	bool isSelected = 0;
	while (keyboardIn != 27) {
		switch (keyboardIn) {
		case 224:
			switch (_getch()) {
			case 72: // up
				if (cursorIndex == 0) break;
				for (int j = 0; j < erased.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, erased[j].c_str(), erased[j].size(), { 0, static_cast<short>(cursorIndex * colorHeight + j) }, &writtenAmount);
				} cursorIndex--;
				for (int j = 0; j < selected.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, selected[j].c_str(), selected[j].size(), { 0, static_cast<short>(cursorIndex * colorHeight + j) }, &writtenAmount);
				}
				if (isSelected) {
					std::swap(pixels[cursorIndex + 1], pixels[cursorIndex]);
					for (int i = 0; i < colorHeight; i++) {
						string str(colorWidth, pixels[cursorIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(cursorIndex * colorHeight + i) }, &writtenAmount);
					}
					for (int i = 0; i < colorHeight; i++) {
						string str(colorWidth, pixels[cursorIndex + 1].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>((cursorIndex + 1) * colorHeight + i) }, &writtenAmount);
					}
				} break;
			case 80: // down
				if (cursorIndex + 1 == pixels.size()) break;
				for (int j = 0; j < erased.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, erased[j].c_str(), erased[j].size(), { 0, static_cast<short>(cursorIndex * colorHeight + j) }, &writtenAmount);
				} cursorIndex++;
				for (int j = 0; j < selected.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, selected[j].c_str(), selected[j].size(), { 0, static_cast<short>(cursorIndex * colorHeight + j) }, &writtenAmount);
				}
				if (isSelected) {
					std::swap(pixels[cursorIndex - 1], pixels[cursorIndex]);
					for (int i = 0; i < colorHeight; i++) {
						string str(colorWidth, pixels[cursorIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(cursorIndex * colorHeight + i) }, &writtenAmount);
					}
					for (int i = 0; i < colorHeight; i++) {
						string str(colorWidth, pixels[cursorIndex - 1].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>((cursorIndex - 1) * colorHeight + i) }, &writtenAmount);
					}
				} break;
			case 75: // left
			case 83: // delete
				pixels.erase(pixels.begin() + cursorIndex, pixels.begin() + cursorIndex + 1);
				for (int rowIndex = cursorIndex; rowIndex < pixels.size(); rowIndex++) {
					for (int i = 0; i < colorHeight; i++) {
						string str(colorWidth, pixels[rowIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(rowIndex * colorHeight + i) }, &writtenAmount);
					}
				}
				for (int i = 0; i < colorHeight; i++) {
					string str(colorWidth, ' ');
					WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(pixels.size() * colorHeight + i) }, &writtenAmount);
				} break;
			case 77: // enter(right)
				if (isSelected) {
					isSelected = 0;
				}
				else {
					isSelected = 1;
				}
			} break;
		case 13:
			if (isSelected) {
				isSelected = 0;
			}
			else {
				isSelected = 1;
			}
		} keyboardIn = _getch();
	} pixels.push_back({ ' ', ' ' });
	std::ofstream ofs(getExePath().parent_path() / "GammaCorrectorOutput.txt");
	for (int i = 0; i < pixels.size(); i++) {
		ofs << pixels[i].first;
	}
































	//cin.ignore();
	//cin.get();
	return 0;
}







