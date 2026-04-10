## Description
The application is designed to autorun on Windows boot, check if the deadline is expired and either quietly shutdown or open a window. Big red button on the bottom resets the counter.

## Installation
1. Simplest way is to use the provided `FanucBatRem.msi` installer. Specify the installation directory, restart Windows to make sure it's working, press the red button to reset the counter (it will close the window). You can restart Windows again to make sure it doesn't pop up anymore.
2. Copy `fanucbatrem.exe` executable somewhere on the hard drive. Make it autorun on Windows boot. Check correct operation.

## How it works
On launch the program checks `HKEY_CURRENT_USER\SOFTWARE\FanucBatRem\Last` Registry parameter. If the parameter doesn't exist, it creates it and sets it to a date in the past in the format "YYYY-MM". On every launch it checks that key and if current day is less than a year from the `Last`, then the program silently terminates.  
The installer also creates a Registry entry at `HKEY_CURRENT_USER\SOFTWARE\MICROSOFT\Windows\CurrentVersion\Run` to autorun the program on boot.

## How to set last date manually.
1. Open Windows Registry: press "Win+R", type `regedit` and press `Enter`.
2. Navigate to `HKEY_CURRENT_USER\SOFTWARE\FanucBatRem' on the left pane. Double-click on `Last` parameter on the right pane. Set date in "YYYY-MM" format and press `OK`.

## How to make executable run on Windows boot.
1. Open Windows Registry: press "Win+R", type `regedit` and press `Enter`.
2. Navigate to `HKEY_CURRENT_USER\SOFTWARE\MICROSOFT\Windows\CurrentVersion\Run` on the left pane. Right-click on the right pane and click `New` -> `String value`. Set it to the location of the executable, for example `"C:\QuintoG1\50-Qg1\10-bin\fanucbatrem.exe"` and press `OK`.

## What if it doesn't start?
Install latest version of Microsoft Visual C++ Redistributable (available in downloads).

## Download
The latest release is available here:  
[https://github.com/gunnerson/fanucbatrem/releases](https://github.com/gunnerson/fanucbatrem/releases).  
It has an executable `fanucbatrem.exe`; an installer `FanucBatRem.msi` and a "Microsoft Visual C++ Redistributable" installer in case the machine doesn't have it (needs at least 2019).
