-- Log 1 --
[PROBLEM]
C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0\librariesの中にあるWiFi.hを使いたいが、
C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0\libraries のみが補完に出ない.

[SOLUTION]
補完に出るフォルダ(C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0\cores\esp32)にlibrariesの *.h, *.cpp群を配置する。

C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0 内で
```pwsh
Get-ChildItem -Path .\libraries\*\src\* -Recurse | ForEach-Object {
    Copy-Item -Path $_.FullName -Destination .\cores\esp32 -Force
}
```
