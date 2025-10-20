#include "header.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
// function from function.h
void clearConsole() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD charsWritten;
	COORD topLeft = { 0, 0 };
	FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &charsWritten);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, topLeft, &charsWritten);
	SetConsoleCursorPosition(hConsole, topLeft);
}
std::filesystem::path getExePath() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer);
}
enum TimerFlag {
	NoEndLog,
	AutoLog
};
class Timer {
public:
	Timer(string in_name = "No_Name", int in_properties = AutoLog) : name(in_name), properties(in_properties), start(std::chrono::steady_clock::now()) {}
	~Timer() {
		if (this->properties & AutoLog) {
			auto end = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			this->ofs << this->name << ": " << duration.count() << '\n';
			this->ofs.close();
		}
	}
	int duration() {
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		return duration.count();
		
		//return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
	}
private:
	string name;

	int properties;

	std::ofstream ofs = std::ofstream(getExePath().parent_path() / "debugLog.txt", std::ios::app);
	std::chrono::steady_clock::time_point start;
};

class pixel {
public:
	pixel(uint8_t in_first, uint8_t in_second) : first(in_first), second(in_second) {}
	uint8_t first = 0, second = 0;
	//bool operator<(pixel other) { return (this->first + this->second) < (other.first + other.second); }
	/*bool operator!=(pixel other) { return this->first != other.first || this->second != other.second; }
	bool operator==(pixel other) { return this->first == other.first && this->second == other.second; }*/
};
typedef uint8_t pixelIndex;
//class run {
//public:
//	run(uint16_t& in_y, uint16_t& in_x, uint16_t& in_len, pixelIndex in_content) :
//		y(in_y), x(in_x), len(in_len), content(in_content) {}
//	uint16_t y, x, len;
//	pixelIndex content;
//};



class ASCIIPlayer {
public:
	ASCIIPlayer() {
		std::filesystem::path filePath = getExePath().parent_path() / "VideoFile.txaa";
		this->ifs = std::ifstream(filePath, std::ios::binary);
		if (!ifs.is_open()) {
			this->valid = 0;
			return;
		}

		//string copyright;
		char copyright[20];
		this->ifs.read(copyright, 20);
		uint32_t audioFileSize;
		this->ifs.read(reinterpret_cast<char*>(&audioFileSize), 4);
		//vector<uint8_t> audioFileData(audioFileSize);
		this->soundBuffer.assign(audioFileSize, 0);
		this->ifs.read(reinterpret_cast<char*>(this->soundBuffer.data()), audioFileSize);
		// add save aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

		// create characterSet
		uint32_t charSetLen;
		this->ifs.read(reinterpret_cast<char*>(&charSetLen), 4);

		vector<uint8_t> charSetData(charSetLen);
		this->ifs.read(reinterpret_cast<char*>(charSetData.data()), charSetLen);
		CHAR_INFO charTemplate;
		charTemplate.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		for (int i = 0; i < charSetLen; i += 2) {
			vector<CHAR_INFO> single(2000, charTemplate);
			for (int j = 0; j < 1000; j++) { // constant needs to be changed
				single[2 * j].Char.AsciiChar = charSetData[i];
				single[2 * j + 1].Char.AsciiChar = charSetData[i + 1];
			} this->characterSet.push_back(single);
			//this->characterSet.push_back({ static_cast<char>(charSetData[i]), static_cast<char>(charSetData[i + 1]) });+
		}
	}



	inline bool isValid() { return this->valid; }
	inline HANDLE& handle() { return this->hConsole; }



	void play() {
		// first frame
		this->ifs.read(reinterpret_cast<char*>(this->wh), 8);
		vector<uint8_t> firstFrame(this->wh[0] * this->wh[1] * 2);
		this->ifs.read(reinterpret_cast<char*>(firstFrame.data()), firstFrame.size());
		uint8_t* ffptr = firstFrame.data();

		//// warm up: essential for Windows API
		//CHAR_INFO warmupCharTemplate;
		//warmupCharTemplate.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		//warmupCharTemplate.Char.AsciiChar = ' ';
		//vector<CHAR_INFO> warmupBuffer(wh[0] * 2 * wh[1], warmupCharTemplate);
		//SMALL_RECT warmupRange = { 0, 0, static_cast<short>(wh[0] * 2 - 1), static_cast<short>(wh[1] - 1) };
		//for (int i = 0; i < 200; i++) {
		//	Timer timer("Preparation");
		//	WriteConsoleOutputA(
		//		hConsole,
		//		warmupBuffer.data(),
		//		{ static_cast<short>(wh[0] * 2), static_cast<short>(wh[1]) },
		//		{ 0, 0 },
		//		&warmupRange
		//	);
		//}

		// audio
		ma_engine engine;
		ma_engine_init(NULL, &engine);

		ma_decoder decoder_main;
		ma_decoder_init_memory(this->soundBuffer.data(), this->soundBuffer.size(), NULL, &decoder_main);
		ma_sound sound_main;
		ma_sound_init_from_data_source(&engine, &decoder_main, 0, NULL, &sound_main);



		// display first frame
		for (int i = 0; i < this->wh[1]; i++) {
			WriteConsoleOutputCharacterA(this->hConsole, reinterpret_cast<LPCSTR>(ffptr), this->wh[0] * 2, { 0, static_cast<short>(i) }, &this->writtenAmount);
			ffptr += this->wh[0] * 2;
		} //std::this_thread::sleep_for(std::chrono::milliseconds(50));

		int frameAmount;
		this->ifs.read(reinterpret_cast<char*>(&frameAmount), 4);
		bool displayingBuffer1 = 1;

		frameAmount--;
		this->videoBuffer = vector<CHAR_INFO>(this->wh[0] * this->wh[1] * 2);
		this->readFrame(this->videoBuffer.data());

		ma_sound_start(&sound_main);
		
		timeBeginPeriod(1);
		std::chrono::steady_clock::time_point updateTimePoint = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
		while (frameAmount--) {
			//cin.get()
			Timer timerf("Frame", NoEndLog);
			if (frameAmount < 0) break;

			this->displayFrame(videoBuffer.data(), this->wh, this->hConsole);
			this->readFrame(videoBuffer.data());

			int losingFrameAmount = timerf.duration() / 50;
			while (losingFrameAmount--) {
				frameAmount--;
				this->readFrame(videoBuffer.data());
				updateTimePoint += std::chrono::milliseconds(50);
			}

			std::this_thread::sleep_until(updateTimePoint);
			updateTimePoint += std::chrono::milliseconds(50);
		} timeEndPeriod(1);

		ma_sound_stop(&sound_main);
		ma_sound_uninit(&sound_main);
		ma_decoder_uninit(&decoder_main);
		ma_engine_uninit(&engine);
		cin.ignore();
		cin.get();
	}

private:
	std::ifstream ifs;
	bool valid = 1;

	//std::ofstream ofs = std::ofstream(getExePath().parent_path() / "debugLog.txt");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD writtenAmount;

	std::vector<vector<CHAR_INFO>> characterSet;
	uint32_t wh[2]; // pixel WH (width need * 2)

	vector<CHAR_INFO> videoBuffer;
	bool displayingBuffer1 = 0;
	std::atomic_bool inProcessDisplaying = 0;

	vector<char> soundBuffer;

	void readFrame(CHAR_INFO* bufferptr) {
		//Timer timer("ReadFrame");
		uint32_t frameDataLen;
		this->ifs.read(reinterpret_cast<char*>(&frameDataLen), 4);
		auto pos = this->ifs.tellg();
		uint32_t frameRunAmount = frameDataLen / 3;
		vector<uint8_t> frameData(frameDataLen);
		uint8_t* frameDataPtr = frameData.data();
		uint8_t* frameDataEndPtr = frameDataPtr + frameDataLen;  
		this->ifs.clear();
		this->ifs.read(reinterpret_cast<char*>(frameData.data()), frameDataLen);

		auto pos2 = this->ifs.tellg();

		//vector<run>& buffer = !this->displayingBuffer1 ? this->buffer1 : this->buffer2;

		//vector<CHAR_INFO>& buffer = *bufferptr;
		
		//buffer = vector<CHAR_INFO>(this->wh[0] * this->wh[1] * 2);

		uint32_t index = 0;
		while (frameDataPtr < frameDataEndPtr) {
			uint16_t len = *reinterpret_cast<const uint16_t*>(frameDataPtr) * 2;
			uint8_t& pixIndex = *(frameDataPtr + 2);

			memcpy(bufferptr, reinterpret_cast<char*>(this->characterSet[pixIndex].data()), len * sizeof(CHAR_INFO));

			frameDataPtr += 3;
			bufferptr += len;
		}
	}
	void displayFrame(CHAR_INFO* buffer, uint32_t wh[2], HANDLE hConsole) {
		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

		//Timer timer("DisplayFrame");
		this->inProcessDisplaying = 1;
		// WriteConsoleOutput
		//vector<CHAR_INFO> buffertemp(buffer.size());
		/*for (auto& i : buffertemp) {
			i.Char.AsciiChar = 'Q';
			i.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}*/

		SMALL_RECT temp = { 0, 0, static_cast<short>(wh[0] * 2 - 1), static_cast<short>(wh[1] - 1) };
		WriteConsoleOutputA(
			hConsole,
			buffer,
			{ static_cast<short>(wh[0] * 2), static_cast<short>(wh[1]) },
			{ 0, 0 },
			&temp
		);
		//buffer.clear();
		this->inProcessDisplaying = 0;
	}

	


};

















void writeColor(HANDLE hConsole, CHAR_INFO* buffer, short y, short x, short height, short width = 1000) {
	SMALL_RECT temp = { x, y, x + width - 1, y + height - 1 };
	COORD coord = { 0, 0 };
	WriteConsoleOutputA(
		hConsole,
		buffer,
		{ width, height },
		coord,
		&temp
	);
}





int main() {

	// width = 1888
	// height = 1064

	ASCIIPlayer player;
	if (!player.isValid()) {
		cout << "[Error]: Missing Video File: Video Filename Specification: \"VideoFile.txaa\"" << endl;
		cin.get();
		return 0;
	}

	cout << "Press enter to play the video.\n";
	cin.get();
	player.play();
	return 0;

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

	vector<string> selected = {
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
	
	

	int colorWidth; cin >> colorWidth;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (colorWidth < 0) {
		colorWidth = std::abs(colorWidth);
		CHAR_INFO charTemplate;
		charTemplate.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		vector<CHAR_INFO> buffer(1000 * colorWidth * pixels.size(), charTemplate);
		int singleColorLen = colorWidth * 1000;
		for (int i = 0; i < ASCII.size(); i++) {
			for (int j = 0; j < singleColorLen; j++) {
				buffer[i * singleColorLen + j].Char.AsciiChar = ASCII[i];
			}
		}
		SMALL_RECT temp = { 0, 0, static_cast<short>(1020), static_cast<short>(ASCII.size() * colorWidth) };
		WriteConsoleOutputA(
			hConsole,
			buffer.data(),
			{ static_cast<short>(1000), static_cast<short>(ASCII.size() * colorWidth) },
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
		string str(1268, pixels[i].first);
		/*for (int j = 0; j < 939; j++) {
			str[2 * j] = pixels[i].first;
			str[2 * j + 1] = pixels[i].second;
		}*/
		for (int increment = 0; increment < colorWidth; increment++) {
			WriteConsoleOutputCharacterA(hConsole, str.c_str(), 1268, {20, static_cast<short>(i * colorWidth + increment)}, &writtenAmount);
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
					WriteConsoleOutputCharacterA(hConsole, erased[j].c_str(), erased[j].size(), { 0, static_cast<short>(cursorIndex * colorWidth + j) }, &writtenAmount);
				} cursorIndex--;
				for (int j = 0; j < selected.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, selected[j].c_str(), selected[j].size(), { 0, static_cast<short>(cursorIndex * colorWidth + j) }, &writtenAmount);
				}
				if (isSelected) {
					std::swap(pixels[cursorIndex + 1], pixels[cursorIndex]);
					for (int i = 0; i < colorWidth; i++) {
						string str(1268, pixels[cursorIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(cursorIndex * colorWidth + i) }, &writtenAmount);
					}
					for (int i = 0; i < colorWidth; i++) {
						string str(1268, pixels[cursorIndex + 1].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>((cursorIndex + 1) * colorWidth + i) }, &writtenAmount);
					}
				} break;
			case 80: // down
				if (cursorIndex + 1 == pixels.size()) break;
				for (int j = 0; j < erased.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, erased[j].c_str(), erased[j].size(), { 0, static_cast<short>(cursorIndex * colorWidth + j) }, &writtenAmount);
				} cursorIndex++;
				for (int j = 0; j < selected.size(); j++) {
					WriteConsoleOutputCharacterA(hConsole, selected[j].c_str(), selected[j].size(), { 0, static_cast<short>(cursorIndex * colorWidth + j) }, &writtenAmount);
				}
				if (isSelected) {
					std::swap(pixels[cursorIndex - 1], pixels[cursorIndex]);
					for (int i = 0; i < colorWidth; i++) {
						string str(1268, pixels[cursorIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(cursorIndex * colorWidth + i) }, &writtenAmount);
					}
					for (int i = 0; i < colorWidth; i++) {
						string str(1268, pixels[cursorIndex - 1].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>((cursorIndex - 1) * colorWidth + i) }, &writtenAmount);
					}
				} break;
			case 75: // left
			case 83: // delete
				pixels.erase(pixels.begin() + cursorIndex, pixels.begin() + cursorIndex + 1);
				for (int rowIndex = cursorIndex; rowIndex < pixels.size(); rowIndex++) {
					for (int i = 0; i < colorWidth; i++) {
						string str(1268, pixels[rowIndex].first);
						WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(rowIndex * colorWidth + i) }, &writtenAmount);
					}
				}
				for (int i = 0; i < colorWidth; i++) {
					string str(1268, ' ');
					WriteConsoleOutputCharacterA(hConsole, str.c_str(), str.size(), { 20, static_cast<short>(pixels.size() * colorWidth + i) }, &writtenAmount);
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