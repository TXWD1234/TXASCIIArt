# Welcome to TXASCIIArt
This project is targeted to render pictures and videos in ASCII characters, and display them on Windows Terminal in realtime.

**Project Idea:** Use ASCII characters to show pictures by mapping the grey value of each pixel of the picture into ASCII.

**Development time:** Sept.13.2025 ~ Oct.17.2025.

**Technical Support:** 
- Windows API (<windows.h>)
- miniaudio (https://miniaud.io/)
- Qt (https://www.qt.io/)
  
**Environment:** Win11

*This project is written with C++.*

# Program Structure:
TXASCIIArt consists of three independent programs working together:
- ASCIIRenderer - render pictures or frame sequences into .txaa data files
- ASCIIPlayer - read the .txaa file and display the content in Windows Termianl (cmd / Windows Console) as ASCII animations
- GammaCorrecter - create and correct the character set used in ASCIIRenderer

***The executables can be found directly in the `release` directory.*** 

# ASCII Renderer
Render pictures into .txaa files, which are the file to store the data of pictures.

You can make your own ASCII pictures and videos with this.

It can accept .png and .jpg files. If you want to do videos, please convert the video into frames.

It can also accept audio files (.mp3), you can separate the audio from the video and use that audio file.

You can use *Format Factory (https://format-factory.en.softonic.com/)* or *ffmpeg (https://www.ffmpeg.org/)* to complete the operations above.

*GUI created with Qt.*

## Parameter Specifications
### Max Width / Max Height
The maximum character the terminal can contain in a single line / the maximum line the terminal can contain in a single screen without scrolling.

You can test it out by your self, or use the parameter below:
| Font Size | Width | Height |
| --------- | ----- | ------ |
| 1px       | 1888  | 532    |
| 2px       | 940   | 295    |
| 5px       | 376   | 118    |
| 7px       | 268   | 84     |

*The npx means the size of the character. Usually we use 1px as it can show the highest quality*

### Character Set
The characters used to form the graphics.

**Every different screen producer has there own settings, that means different monitor won't 100% work with the same character set.**

Therefore, we had designed GammaCorrector to create character set for different screens.

You can also use the character sets in /main/character-sets.txt if there's one matches your screen.

If you would like to create a character set for your screen that was not on the list, we will be very appreciated.

### Reverse Character Set
The character set should be in order of darker(such as ' ' and ',') to lighter(such as 'M' and 'Q').

But sometimes the output of the GammaCorrecter is reversed, therefore there's this option to reverse it when creating the ASCIIArt file.

### Use Character Set Optimization
Basicly this option will double the length of the character set by an algorithm and with no any downside.

*The longer the character set, the higher the quality of the graphic.*

Then why not check this option everytime?

# ASCII Player
Plays the data in .txaa files, and display ASCII images on Windows Terminal.

**This program require the battary option to be set to `high performance` mode, and in full screen and 1px font size to preform the best.**

It would only play the file in the same directory of the .exe file that named: `VideoFile.txaa`. *(It's hardcoded i know, but we don't need to input the path and do these pointless extra operations)*

Press enter to start playing.

# Gamma Corrector
Create and correct the character set used in ASCIIRenderer.

The program would display all the valid characters in ASCII as bars on the screen. You can sort the color from darker(at the top) to lighter(at the bottom) to correct the character set.

You can input a number of the height of the bar.

The program would output a file called `GammaCorrectorOutput.txt` in the directory that the .exe file was at as the result of the correction process, after you hit the Esc button.

If there's a file that has the same name already existed, the program will load the content in that file as the character set and display it.

## Control
First enter the height of the bar that displaies each character (usually to be 3 ~ 5), and then press enter.

As soon as you press enter, the bars of colors would appear.

There will also appears a cursor.

Use the arrow keys (right, up, left, down keys) on the keyboard to contral the cursor.

### Up / Down key
Move the cursor up / down one block.

### Left key
Delete the current bar that the cursor is on.

### Right key
Select the current bar. Press again to deselect.

When a bar is selected, it will move with the cursor.

### Esc key
Exit the program and save the result.

To exit without changing the already exist file, just use Alt + F4.

# Credits
**Produced by:** TX Studio

**Developer:** TX_Jerry [@TXWD1234](https://github.com/TXWD1234)
