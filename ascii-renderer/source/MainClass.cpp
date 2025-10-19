#include "MainClass.h"

std::filesystem::path getExePath() {
	return std::filesystem::path((QCoreApplication::applicationDirPath()).toStdWString());
}
std::filesystem::path getOutputFilePath(string input_filename) { // files that has index
	std::filesystem::path filename(input_filename);
	string baseName = filename.stem().string() + ' ', extension = filename.extension().string();
	std::filesystem::path exePath = getExePath();

	int fileIndex = 1;
	std::ostringstream oss;
	oss << baseName << std::setfill('0') << std::setw(2) << fileIndex << extension;
	while (std::filesystem::exists(std::filesystem::path(exePath / oss.str()))) {
		fileIndex++;
		oss.str("");
		oss.clear();
		oss << baseName << std::setfill('0') << std::setw(2) << fileIndex << extension;
	}
	return exePath / oss.str();
}










txInputWindow::txInputWindow(QWidget* parent) : QWidget(parent) {
	this->setFixedSize(400, 280);

	// setStyleSheet
	{
		this->setStyleSheet(R"(
QWidget {
    /*background-color: #f7f7f7;   整体背景浅灰白 */
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 13px;
    color: #333333;
}

/* 输入框 */
QLineEdit {
    border: 1px solid #cccccc;
    border-radius: 4px;
    padding: 4px;
    background-color: #ffffff;
}

QLineEdit:focus {
    border: 1px solid #888888;
    background-color: #fafafa;
}

/* 普通按钮 */
QPushButton {
    background-color: #e6e6e6;
    border: 1px solid #cccccc;
    border-radius: 4px;
    padding: 5px 10px;
}

QPushButton:hover {
    background-color: #dcdcdc;
}

QPushButton:pressed {
    background-color: #c8c8c8;
}

/* Confirm 按钮特殊处理 */
QPushButton#confirm {
    background-color: #dcdcdc;
    border: 1px solid #999999;
    font-weight: bold;
}

QPushButton#confirm:hover {
    background-color: #cfcfcf;
}

QPushButton#confirm:pressed {
    background-color: #bbbbbb;
}

)");
	}

	QFormLayout* inputLayout = new QFormLayout();
	QVBoxLayout* mainLayout = new QVBoxLayout();

	this->widthInput = new QLineEdit(this);
	this->heightInput = new QLineEdit(this);
	this->characterSetInput = new QLineEdit(this);
	inputLayout->addRow("Max Width:", this->widthInput);
	inputLayout->addRow("Max Height:", this->heightInput);
	inputLayout->addRow("Character Set:", this->characterSetInput);
	QObject::connect(this->widthInput, &QLineEdit::editingFinished, [this]() {
		if (this->numInputFormatCheck(this->widthInput)) this->heightInput->setFocus(); });
	QObject::connect(this->heightInput, &QLineEdit::editingFinished, [this]() {
		if (this->numInputFormatCheck(this->heightInput)) this->characterSetInput->setFocus(); });
	QObject::connect(this->characterSetInput, &QLineEdit::editingFinished, [this]() {
		this->doReverseCharacterSetCheckB->setFocus();
		});
	this->doReverseCharacterSetCheckB = new QCheckBox("Reverse Character Set", this);
	this->doUseCharacterSetOptimizationCheckB = new QCheckBox("Use Character Set Optimization", this);
	inputLayout->addRow(this->doReverseCharacterSetCheckB, this->doUseCharacterSetOptimizationCheckB);
	//inputLayout->addRow(this->doUseCharacterSetOptimizationCheckB);
	QObject::connect(this->doReverseCharacterSetCheckB, &QCheckBox::checkStateChanged, [this]() {
		this->doUseCharacterSetOptimizationCheckB->setFocus(); });
	QObject::connect(this->doUseCharacterSetOptimizationCheckB, &QCheckBox::checkStateChanged, [this]() {
		this->chooseSaveDestinationButton->setFocus();
		this->chooseSaveDestinationButton->setDefault(1);
		});

	this->chooseSaveDestinationButton = new QPushButton("Choose Save Destination", this);
	this->chooseGraphicFilesButton = new QPushButton("Choose Graphic Files", this);
	this->chooseAudioFilesButton = new QPushButton("Choose Audio File", this);
	inputLayout->addRow(chooseSaveDestinationButton);
	inputLayout->addRow(chooseGraphicFilesButton);
	inputLayout->addRow(chooseAudioFilesButton);
	QObject::connect(this->chooseSaveDestinationButton, &QPushButton::clicked, this, &txInputWindow::selectSaveDestination);
	QObject::connect(this->chooseGraphicFilesButton, &QPushButton::clicked, this, &txInputWindow::selectGraphicFiles);
	QObject::connect(this->chooseAudioFilesButton, &QPushButton::clicked, this, &txInputWindow::selectAudioFiles);

	this->cancelButton = new QPushButton("Cancel", this);
	this->confirmButton = new QPushButton("Confirm", this);
	QObject::connect(cancelButton, &QPushButton::clicked, [this]() {
		this->close();
		});
	QObject::connect(confirmButton, &QPushButton::clicked, this, &txInputWindow::startProcess);

	

	mainLayout->addLayout(inputLayout, 8);
	QHBoxLayout* confirmLayout = new QHBoxLayout();
	confirmLayout->addStretch(1);
	confirmLayout->addWidget(cancelButton, 1);
	confirmLayout->addStretch(1);
	confirmLayout->addWidget(confirmButton, 1);
	confirmLayout->addStretch(1);
	mainLayout->addLayout(confirmLayout);

	this->setLayout(mainLayout);




	







	//QObject::connect(this->inputLine, &QLineEdit::editingFinished, this, &txInputWindow::startProcess);
}

bool txInputWindow::numInputFormatCheck(QLineEdit* input) {
	QString str = input->text();
	bool isValidStr;
	int tabLength = str.toInt(&isValidStr);
	if (!isValidStr) {
		input->setText("");
		return 0;
	} return 1;
}

void txInputWindow::selectSaveDestination() {
	this->saveDestination = QFileDialog::getSaveFileName(nullptr, "choose save destination", QDir::homePath() + "/desktop/VideoFile.txaa", "TXASCIIArt File (*.txaa)");
	this->chooseGraphicFilesButton->setFocus();
	this->chooseGraphicFilesButton->setDefault(1);
}
void txInputWindow::selectGraphicFiles() {
	this->graphicFilePaths = QFileDialog::getOpenFileNames(nullptr, "select graphic files", QDir::homePath() + "/desktop", "graphic (*.png; *.jpg)");
	this->chooseAudioFilesButton->setFocus();
	this->chooseAudioFilesButton->setDefault(1);
}
void txInputWindow::selectAudioFiles() {
	this->audioFilePath = QFileDialog::getOpenFileName(nullptr, "select audio files", QDir::homePath() + "/desktop", "audio (*.mp3)");
	this->confirmButton->setFocus();
	this->confirmButton->setDefault(1);
}


void txInputWindow::startProcess() {
	QString charSet = this->characterSetInput->text();
	if (this->doReverseCharacterSetCheckB->isChecked()) std::reverse(charSet.begin(), charSet.end());
	Main* main = new Main(
		this->saveDestination, 
		charSet, this->doUseCharacterSetOptimizationCheckB->isChecked(),
		this->graphicFilePaths, this->audioFilePath,
		this->widthInput->text().toInt(),
		this->heightInput->text().toInt());
	main->show();
	main->setAttribute(Qt::WA_DeleteOnClose);
	this->close();
}



Main::Main(QString input_saveDestination, QString input_characterSet, bool useCharacterSetOptimization, QStringList& input_graphicFilePaths, QString& input_audioFilePath, int input_maxWidth, int input_maxHeight, QWidget* parent) :
	saveDestination(input_saveDestination.toStdString()),
	graphicFilePaths(input_graphicFilePaths),
	audioFilePath(input_audioFilePath),
	maxWidth(input_maxWidth / 2),
	maxHeight(input_maxHeight),
	frameAmount(input_graphicFilePaths.size()),
	framePixalAmount(maxWidth * maxHeight),
	QWidget(parent) {

	// setStyleSheet
	{
	this->setStyleSheet(R"(
QWidget {
    background-color: #f7f7f7;
    font-family: "Segoe UI", "Microsoft YaHei";
    font-size: 13px;
    color: #333333;
}

QLabel {
    color: #444444;
    font-size: 14px;
    padding: 2px;
}

QProgressBar {
    border: 1px solid #666;
    border-radius: 5px;
    background-color: #eee;
    text-align: center;
}

QProgressBar::chunk {
    border-radius: 5px;
    background: qlineargradient(
        x1:0, y1:0, x2:1, y2:0,
        stop:0 #7adc5a,  /* 浅绿 */
        stop:1 #4caf50 /* 中绿 */
        /*stop:1 #3a8c3a   深绿 */
    );
}

QProgressBar::chunk:hover {
    background: qlineargradient(
        x1:0, y1:0, x2:1, y2:0,
        stop:0 #43e97b, stop:1 #38f9d7
    );
})");}
	this->resize(400, 250);
	this->raise();
	this->setWindowTitle("TXASCIIArt Renderer");

	this->totalPixalAmount = frameAmount * (ll)framePixalAmount;

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	this->frameAmountLabel = new QLabel(this);
	this->frameAmountLabel->setText(QString("0 / %1 frame completed").arg(this->frameAmount));
	this->frameProgressLabel = new QLabel(this);
	this->frameProgress = new QProgressBar(this);
	this->frameProgress->setRange(0, 1000);
	//this->frameProgress->setTextVisible(1);
	this->frameProgress->setOrientation(Qt::Horizontal);
	this->overallProgressLabel = new QLabel(this);
	this->overallProgress = new QProgressBar(this);
	this->overallProgress->setRange(0, 1000);
	//this->overallProgress->setTextVisible(1);
	this->overallProgress->setOrientation(Qt::Horizontal);

	mainLayout->setContentsMargins(30, 25, 20, 35);
	mainLayout->addWidget(this->frameAmountLabel, 2);
	mainLayout->addStretch(1);
	mainLayout->addWidget(this->frameProgressLabel, 2);
	mainLayout->addWidget(this->frameProgress, 3);
	mainLayout->addStretch(1);
	mainLayout->addWidget(this->overallProgressLabel, 2);
	mainLayout->addWidget(this->overallProgress, 3);

	this->ofs = std::ofstream(input_saveDestination.toStdWString(), std::ios::binary);

	string charSet = input_characterSet.toStdString();
	uint8_t previousChar = 0;
	for (char i : charSet) {
		uint8_t ui = static_cast<uint8_t>(i);
		if (previousChar != 0 && useCharacterSetOptimization) {
			this->pixels.push_back({ previousChar, ui });
		} previousChar = ui;
		this->pixels.push_back({ ui, ui });
		
	}



	this->frameDiffData.reserve(input_maxHeight * input_maxWidth);
	QTimer::singleShot(1, this, &Main::transformation);
}

void Main::closeEvent(QCloseEvent* event) {
	if (this->inProgress) {
		this->inUnusualExiting = 1;
		this->ofs.close();
		std::filesystem::path filePath(this->saveDestination);
		std::filesystem::remove(filePath);
	} event->accept();
}




void Main::modifyImg(QImage& img) {
	img = img.convertToFormat(QImage::Format_Grayscale8);
	//img.save("output.png");
	img = img.scaled(this->maxWidth, this->maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	
}
void Main::processFirstFrame() {
	QImage img(this->graphicFilePaths[0]);
	this->modifyImg(img);
	//img.save("output.png"); 
	uint32_t sizeData[] = { img.width(), img.height() };
	this->ofs.write(reinterpret_cast<char*>(sizeData), 8);

	auto& aimg = this->previousFrame;
	aimg.reserve(img.width() * img.height() * 2);
	vector<uint8_t> outputData(img.width() * img.height() * 2);
	uchar* imgb = img.bits();
	for (int y = 0; y < img.height(); y++) {
		int offset = y * img.bytesPerLine();
		for (int x = 0; x < img.width(); x++) {
			aimg.push_back(std::round(imgb[offset + x] * (this->pixels.size() - 1) / 256.0));
			outputData[(y * img.width() + x) * 2] = this->pixels[aimg.back()].first;
			outputData[(y * img.width() + x) * 2 + 1] = this->pixels[aimg.back()].second;
		}
	} ofs.write(reinterpret_cast<char*>(outputData.data()), outputData.size());
}
void Main::push(pixelIndex input_pix, uint16_t y, uint16_t x) {
	if (this->previousX == x - 1 && this->previousY == y && input_pix == previousPix) { // if is continuous
		this->continuousPixAmount++;
	} else { // start new unit
		// store previous unit
		this->storeUnit();

		this->continuousPixAmount = 1;
		this->previousPix = input_pix;
		this->previousY = y;
		this->startAtX = x;
	} this->previousX = x;
}
void Main::storeUnit() {
	if (!this->continuousPixAmount) return;
	uint8_t ptr[3];
	uint16_t buffer[] = { this->continuousPixAmount };
	memcpy(ptr, buffer, 2);
	ptr[2] = this->previousPix;
	this->frameDiffData.insert(this->frameDiffData.end(), ptr, ptr + 3);
}

void Main::transformation() {
	
	this->ofs.write("Copyright@TXASCIIArt", 20);
	// Audio
	QFile audioFile(this->audioFilePath);
	if (audioFile.exists()) {
		int audioFileSize = static_cast<int>(audioFile.size());
		this->ofs.write(reinterpret_cast<char*>(&audioFileSize), 4);
		std::ifstream ifs(std::filesystem::path(this->audioFilePath.toStdWString()), std::ios::binary);
		const int ByteSize = 1024 * 1024;
		int byteRead = ByteSize;
		vector<uint8_t> filedata(ByteSize);
		while (byteRead == ByteSize) {
			ifs.read(reinterpret_cast<char*>(filedata.data()), ByteSize);
			byteRead = ifs.gcount();
			//for (int i = 0; i < byteRead; i++) { // for each byte
			//	// operation
			//}
			this->ofs.write(reinterpret_cast<char*>(filedata.data()), byteRead);
		}
	} else {
		int zero = 0;
		this->ofs.write(reinterpret_cast<char*>(&zero), 4);
	}

	// Graphic
	// define run anArrayOfContinuousSameCharacter


	// Write Character Set Prefix
	uint32_t charSetLen = this->pixels.size() * 2;
	vector<uint8_t> charSet(charSetLen);
	for (int i = 0; i < this->pixels.size(); i++) {
		charSet[i * 2] = this->pixels[i].first;
		charSet[i * 2 + 1] = this->pixels[i].second;
	} this->ofs.write(reinterpret_cast<char*>(&charSetLen), 4);
	this->ofs.write(reinterpret_cast<char*>(charSet.data()), charSetLen);

	this->processFirstFrame();

	int frameAmount = this->graphicFilePaths.size() - 1;
	this->ofs.write(reinterpret_cast<char*>(&frameAmount), 4);

	for (int i = 1; i <= frameAmount; i++) { // for each frame
		QImage img(this->graphicFilePaths[i]);
		this->modifyImg(img);
		
		processFrame(img);
		if (this->inUnusualExiting) return;
		this->frameAmountLabel->setText(QString("%1 / %2 frame completed")
			.arg(++this->processedFrameAmount)
			.arg(this->frameAmount));

	} this->ofs.close();
	this->inProgress = 0;
	this->close();
}
void Main::processFrame(QImage& img) {
	uchar* imgb = img.bits();
	int offset;
	for (int y = 0; y < img.height(); y++) {
		offset = y * img.bytesPerLine();
		for (int x = 0; x < img.width(); x++) {
			if (this->inUnusualExiting) return;
			pixelIndex currentPix = std::round(imgb[offset + x] * (this->pixels.size() - 1) / 256.0);
			this->push(currentPix, y, x);
			this->getPixel(y, x) = currentPix;
		}
		this->frameProgressLabel->setText(QString("%1 / %2 pixels processed in current frame").arg(this->frameProcessedPixalAmount += img.width()).arg(this->framePixalAmount));
		this->frameProgress->setValue(this->frameProcessedPixalAmount * 1000 / this->framePixalAmount);
		this->overallProgressLabel->setText(QString("%1 / %2 pixels processed overall").arg(this->totalProcessedPixalAmount += img.width()).arg(this->totalPixalAmount));
		this->overallProgress->setValue(this->totalProcessedPixalAmount * 1000 / this->totalPixalAmount);
		QCoreApplication::processEvents();
	}
	// store last unit
	this->storeUnit();

	this->continuousPixAmount = 0;
	this->previousPix = pixelIndex();
	this->previousY = -1;
	this->startAtX = -1;
	this->previousX = -1;
	/*
	cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
	Problem: result after deconpress has larger length then original
	Pos: second frame
	-see storeUint() and push()
	
	
	*/

	

	uint32_t outputLength = static_cast<uint32_t>(this->frameDiffData.size());
	this->ofs.write(reinterpret_cast<char*>(&outputLength), 4);
	auto pos = this->ofs.tellp();
	this->ofs.write(reinterpret_cast<char*>(this->frameDiffData.data()), outputLength);
	this->ofs.flush();
	auto pos2 = this->ofs.tellp();
	this->frameDiffData.clear();
	this->frameProcessedPixalAmount = 0;
}




