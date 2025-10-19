#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtProject3.h"

#include "qtheader.h"
#include "header.h"

typedef long long ll;






class Main;
class txInputWindow : public QWidget{
	Q_OBJECT
public:
	txInputWindow(QWidget* parent = nullptr);


private:
	// UI
	QLineEdit* widthInput;
	QLineEdit* heightInput;
	QLineEdit* characterSetInput;
	QCheckBox* doReverseCharacterSetCheckB;
	QCheckBox* doUseCharacterSetOptimizationCheckB;
	QString saveDestination;
	QStringList graphicFilePaths;
	QString audioFilePath;
	QPushButton* chooseSaveDestinationButton;
	QPushButton* chooseGraphicFilesButton;
	QPushButton* chooseAudioFilesButton;
	QPushButton* cancelButton;
	QPushButton* confirmButton;

	bool numInputFormatCheck(QLineEdit* input);
	void selectSaveDestination();
	void selectGraphicFiles();
	void selectAudioFiles();

	

	void startProcess();


};

class pixel {
public:
	uint8_t first = 0, second = 0;
	bool operator!=(pixel other) { return this->first != other.first || this->second != other.second; }
	bool operator==(pixel other) { return this->first == other.first && this->second == other.second; }
};
typedef uint8_t pixelIndex;

class Main : public QWidget
{
    Q_OBJECT

public:
    Main(QString input_saveDestination, QString input_characterSet, bool useCharacterSetOptimization, QStringList& input_graphicFilePaths, QString& input_audioFilePath, int input_maxWidth, int input_maxHieght, QWidget *parent = nullptr);
	Main() {}
	~Main() {}

private:

	std::ofstream ofs;
	string saveDestination;

	QStringList graphicFilePaths;
	QString audioFilePath;

	// process
	void transformation();
	void processFrame(QImage& img);
	void modifyImg(QImage& img);
	void processFirstFrame();

	vector<pixelIndex> previousFrame;

	vector<uint8_t> frameDiffData;
	pixelIndex previousPix;
	uint16_t continuousPixAmount = 0;
	uint16_t startAtX = -1;
	uint16_t previousX = -1;
	uint16_t previousY = -1;

	inline pixelIndex& getPixel(int y, int x) {
		return this->previousFrame[maxWidth * y + x];
	}
	void push(pixelIndex input_pix, uint16_t y, uint16_t x);
	void storeUnit();

	//string ASCII = "@%#*+=-:. ";
	//string ASCII = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	//string ASCII = "@#&%$WM8B0QOZqpmdbkhao*+=-:. ";
	//string ASCII = "@&$%WM#B8Q0OZImkaioc*[+{(/-:;'`. ";
	//string ASCII = "MBNW@#8gRQ0D$&6p9EHbOKmd45GAPUaeqSw3ZXVko2%hICyFL{}ftj1TJn[]iusxYz7c=vl|+>(<?r)/\*!_\"^; ~:, '.-` ";

	string ASCII = "QDSgW|#*,^=";
	// reversed
	//std::vector<pixel> pixels = {
	//{81, 81},        // "QQ"
	//{68, 68}, {68, 83},   // "DD", "DS"
	//{83, 83}, {83, 103},  // "SS", "Sg"
	//{103, 103}, {103, 87}, // "gg", "gW"
	//{87, 87}, {103, 35},   // "WW", "g#"
	//{124, 124}, {124, 35}, // "||", "|#"
	//{35, 35}, {35, 35}, {124, 42}, {124, 44}, {124, 44}, {35, 44}, // "##", "##", "|*", "|,", "|,", "#,"
	//{42, 42}, {42, 44}, {42, 44}, // "**", "*,", "*,"
	//{44, 44}, {44, 44}, {42, 94}, {44, 94}, {44, 94}, // ",,", ",,", "*^", ",^", ",^"
	//{94, 94}, {94, 94}, {94, 94}, {94, 61}, {94, 61}, // "^^", "^^", "^^", "^=", "^="
	//{61, 61}, {61, 61}  // "==", "=="
	//};

	//std::vector<pixel> pixels = {
	//{61, 61}, {61, 61},         // "==", "=="
	//{94, 61}, {94, 61}, {94, 94}, {94, 94}, {94, 94}, // "^=", "^=", "^^", "^^", "^^"
	//{44, 94}, {44, 94}, {42, 94}, {44, 44}, {44, 44}, // ",^", ",^", "*^", ",,", ",,"
	//{42, 44}, {42, 44}, {42, 42},                     // "*,", "*,", "**"
	//{35, 44}, {124, 44}, {124, 44}, {124, 42}, {35, 35}, {35, 35}, // "#,", "|,", "|,", "|*", "##", "##"
	//{124, 35}, {124, 124},                             // "|#", "||"
	//{103, 35}, {87, 87},                               // "g#", "WW"
	//{103, 87}, {103, 103},                             // "gW", "gg"
	//{83, 103}, {83, 83},                               // "Sg", "SS"
	//{68, 83}, {68, 68},                                // "DS", "DD"
	//{81, 81}                                           // "QQ"
	//};

	vector<pixel> pixels;


	int maxWidth, maxHeight;



	// UI

	QLabel* frameAmountLabel;
	QLabel* frameProgressLabel;
	QProgressBar* frameProgress;
	QLabel* overallProgressLabel;
	QProgressBar* overallProgress;

	int frameAmount, processedFrameAmount = 0;
	int framePixalAmount, frameProcessedPixalAmount;
	ll totalPixalAmount, totalProcessedPixalAmount = 0;



	bool inProgress = 1;
	bool inUnusualExiting = 0;
	void closeEvent(QCloseEvent* event) override;
};

