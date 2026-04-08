## Description
The application is designed to autorun on Windows boot, check if the deadline is expired and either quietly shutdown or open a window. Big red button on the bottom resets the counter.

## Installation
1. Simplest way is to use the provided `fanucbatrem.msi` installer. Specify the installation directory, restart Windows to make sure it's working, press the red button to reset the counter (it will close the window). You can restart Windows again to make sure it doesn't pop up anymore.
2. Copy `fanucbatrem.exe` executable somewhere on the hardrive. Make it autorun on Windows boot. Check correct operation.

## How it works
On launch the program checks `HKEY_CURRENT_USER\SOFTWARE\FanucBatRem\Last` Registry parameter. If the parameter doesn't exist, it creates it and sets it to a date in the past in the format "YYYY-MM". On every launch it checks that key and if current day is less than a year from the `Last`, then the program silently terminates. 

The installer also creates a Registry entry at `HKEY_CURRENT_USER\SOFTWARE\MICROSOFT\Windows\CurrentVersion\Run` to autorun the program on boot.
