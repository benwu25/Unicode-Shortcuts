Welcome to Unicode-Shortcuts!!

"\heart" -> 💓, "\questionman" -> 🧐, "\nabla" -> ∇, "\nerd" -> 🤓, "\moneysmiles" -> 🤑  

and many more fun character mappings.

<b>Building</b>

To build this project on a 64-bit machine with Windows 10,
1. Ensure MSBuild is installed, available with the Windows SDK.
2. Clone this repository: "git clone https://github.com/benwu25/Unicode-Shortcuts"
3. Execute the command "msbuild Unicode-Shortcuts\Unicode-Shortcuts.sln"
4. To enable character mappings, the application is stored in "Unicode-Shortcuts\x64\Debug"!

<b>Notes:</b>  
Currently, it is only possible to hard-code string -> Unicode mappings in the source file  
Unicode-Shortcuts.cpp. I am hoping to add a feature allowing new mappings to be added and stored in the Window.
