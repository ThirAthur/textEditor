# **Panduan Instalasi dan setting enviorenment** :blue_book:
>Catatan: Panduan ini dikhususkan untuk pengguna Visual Studio Code dan komputer dengan sistem operasi Windows

## Persiapan
### 1. Instal MSYS2 dan GTK
Download dan Instal dari: https://www.msys2.org/

Setelah melakukan instalasi, buka MSYS2 MINGW64 lalu ketik command:
```bash
pacman -S mingw-w64-x86_64-gtk4
```
karena menggunakan bahasa pemrograman C maka perlu instal compiler maka ketikan command berikut:
```bash
pacman -S mingw-w64-x86_64-toolchain base-devel
```

### 2. Set Enviorment
#### A. Gunakan window lalu cari "Edit the system enviorment variabel"
```bash
Edit the system enviorment variabel
```
#### B. Cari dan klik "Enviormetn Variabel"
#### C. Pada kolom "User variabel for USER" cari dan klik "Path" lalu klik tombol "New" dan tuliskan "C:\MSYS2\mingw64\bin"
```bash
C:\MSYS2\mingw64\bin
```
## Set Enviorment pada Visual Studio Code
### 1. Buka Visual Studio Code dan buka menu extention lalu cari extention "C/C++"'
### 2. Instal extention "C/C++"
### 3. Buatlah folder dan file yang terdapat penggunaan GTK
### 4. Set up Tasks.json dan c_cpp_properties
gunakan shortcut `ctrl + shift + p` lalu ketik 
```bash
C/C++:Edit Configuration (JSON)
```
Klik opsi yang tertampil dan akan terbuka file `c_cpp_properties.json`
pada file `c_cpp_properties.json` ketikan
```bash
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/msys64/mingw64/include/gtk-4.0",
                "C:/msys64/mingw64/include/glib-2.0",
                "C:/msys64/mingw64/include/**",
                "C:/msys64/mingw64/lib/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.22000.0",
            "compilerPath": "C:/msys64/mingw64/bin/gcc.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```
Setelah itu save fie tersebut
buat file baru dengan nama `tasks.json` pada folder yang sama dengan file `c_cpp_properties.json`
pada file `task.json` ketikan
```bash

{
 "version": "2.0.0",
 "tasks": 
 [
  {
   "type": "cppbuild",
   "label": "C/C++: gcc.exe build active file",
   "command": "C:/msys64/mingw64/bin/gcc.exe",
   "args": [
    "-fdiagnostics-color=always",
    "-g",
    "-IC:/msys64/mingw64/include/gtk-4.0",
    "-IC:/msys64/mingw64/include/pango-1.0",
    "-IC:/msys64/mingw64/include",
    "-IC:/msys64/mingw64/include/glib-2.0",
    "-IC:/msys64/mingw64/lib/glib-2.0/include",
    "-IC:/msys64/mingw64/include/harfbuzz",
    "-IC:/msys64/mingw64/include/freetype2",
    "-IC:/msys64/mingw64/include/libpng16",
    "-IC:/msys64/mingw64/include/fribidi",
    "-IC:/msys64/mingw64/include/cairo",
    "-IC:/msys64/mingw64/include/pixman-1",
    "-IC:/msys64/mingw64/include/gdk-pixbuf-2.0",
    "-IC:/msys64/mingw64/include/webp",
    "-DLIBDEFLATE_DLL",
    "-IC:/msys64/mingw64/include/graphene-1.0",
    "-IC:/msys64/mingw64/lib/graphene-1.0/include",
    "-mfpmath=sse",
    "-msse",
    "-msse2",

    "${file}",

    "-LC:/msys64/mingw64/lib",
    "-lgtk-4",
    "-lpangowin32-1.0",
    "-lpangocairo-1.0",
    "-lpango-1.0",
    "-lharfbuzz",
    "-lgdk_pixbuf-2.0",
    "-lcairo-gobject",
    "-lcairo",
    "-lgraphene-1.0",
    "-lgio-2.0",
    "-lgobject-2.0",
    "-lglib-2.0",
    "-lintl",

    "-o",
    "${fileDirname}\\${fileBasenameNoExtension}.exe"
   ],
   "options": {
    "cwd": "C:/msys64/mingw64/bin"
   },
   "problemMatcher": [
    "$gcc"
   ],
   "group": "build",
   "detail": "compiler: C:/msys64/mingw64/bin/gcc.exe"
  }
 ]
}
```


**Setting enviorment komputer dan visual studio code telah berhasil dan cobalah untuk menajalankan program yang tersedia**