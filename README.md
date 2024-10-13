# Bypassing-Windows-Defender

## Project Overview

This project focuses on developing a C++ reverse shell that initially triggers detection by Windows Defender and other AV solutions. The challenge was to bypass these detections through various obfuscation techniques. The current state of the project has successfully reduced detection rates significantly, as demonstrated by testing on VirusTotal.

## Key Features of the Project


- **Advanced API Obfuscation:** 
  - Applied a custom obfuscation algorithm to crucial Windows API functions like `LoadLibrary` and `GetProcAddress`. By obscuring these function calls, the reverse shell becomes significantly harder to reverse-engineer, providing an additional layer of evasion against static analysis tools.
  - Implemented runtime string decryption to hide function names and other sensitive information from memory until they are required for execution

  
- **Network Communication via Winsock:** 
  - Integrated socket programming to establish TCP communication dynamically. This setup allows the reverse shell to connect to predefined IP addresses and ports for remote system access.
  - Implemented flexible communication parameters, allowing easy modification of the server's IP and port settings without altering the core code.


- **Dynamic De-obfuscation Logic:** 
  - Developed lightweight, runtime de-obfuscation functions to resolve obfuscated API calls. These functions allow the reverse shell to perform necessary tasks while keeping obfuscation in place until the last possible moment, reducing the chance of detection by behavioral analysis.

# VirusTotal Results

The obfuscated reverse shell has been tested on VirusTotal and achieved a detection rate of 6/72, indicating significant success in bypassing most antivirus engines, including Windows Defender. The goal of future iterations will be to further reduce this detection rate.

![VirusTotal_POC](https://github.com/user-attachments/assets/b0a095f6-5ffd-467b-a646-1cf7a47353c4)
