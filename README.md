# next-of
Takes a chain of commands, detects the first command not already running, and launches it.

Example:
```cmd
next-of "C:\Program Files (x86)\Steam\steam.exe" "C:\Program Files (x86)\Steam\steamapps\common\Starfield\sfse_loader.exe"
```
Note: for sfse_loader to work properly be sure to set the working directory to `"C:\Program Files (x86)\Steam\steamapps\common\Starfield"`
