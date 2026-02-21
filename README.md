# 🚀 AutoProxy – Windows Network-Based Proxy Switcher

AutoProxy is a lightweight Windows tray application that automatically enables or disables system and Git proxy settings based on your current network profile.

Built using pure Win32 C++ (MinGW), no external dependencies.

## ✨ Features

🔄 Automatic proxy switching based on subnet

🧠 Multi-profile support via config.ini

🔧 Optional Git proxy toggling

🖥 Runs silently in system tray

🔔 Smart notifications

⚙ Simple settings GUI (Enable/Disable App, Notifications)

📝 Optional logging

## ⚙ Configuration

Create a config.ini file in the same directory as the executable.

```
Example
[Global]
logging=true
logfile=autoproxy.log

[College]
subnet=172.19.4.0/22
proxy=172.31.2.4:8080
enable_git=true

[Home]
subnet=192.168.1.0/24
proxy=
enable_git=false
```

## When your system connects to a network:

Current IP is detected

Matching profile is selected

Proxy settings are applied automatically

## 🖥 Tray Menu

### Right-click the tray icon to:

Enable / Disable App

Open Settings

Exit

## 🛠 Build (MinGW)
```
g++ src/*.cpp -Iinclude -o AutoProxy.exe -liphlpapi -lwininet -lws2_32 -lshell32 -mwindows
```
## 📌 Use Case

### Designed for environments like:

College networks requiring proxy access

Office networks with restricted internet

Developers switching between corporate and home networks
