# Bypassing-Windows-Defender

## Key Features of the Project

- **API Obfuscation:** 
  - Implemented a custom algorithm to obfuscate essential API functions, including `LoadLibrary` and `GetProcAddress`. This technique enhances security by making reverse-engineering significantly more challenging.
  
- **Network Communication:** 
  - Utilized socket programming for TCP communication, allowing the project to dynamically establish connections to predefined IP addresses and ports. This enables flexible communication with remote systems.

- **De-obfuscation Logic:** 
  - Developed efficient functions to reverse the obfuscation process during runtime. This allows the program to resolve API calls dynamically, facilitating the execution of necessary tasks while maintaining a low profile.
