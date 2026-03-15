# 🔒 Free Password Checker & Secure Generator

A 100% local, privacy-first password strength analyzer and secure password generator. Built with a C++ core compiled to WebAssembly (WASM), this tool runs entirely in the browser—meaning user passwords never leave their device.

Live Demo: [freepasswordchecker.com](https://www.freepasswordchecker.com/)

## ✨ Features

* **Zero-Server Architecture:** All cryptography math and strength analysis is done client-side via WebAssembly. No databases, no API calls, no network logging.
* **Advanced Entropy Math:** Calculates true cryptographic search space and entropy bits based on character pool analysis.
* **Real-World Crack Times:** Estimates time-to-crack for both online rate-limited attacks (e.g., 1k/sec) and offline GPU rigs (e.g., arrays of RTX 4090s).
* **Secure Generator:** Uses Cryptographically Secure Pseudo-Random Number Generation (CSPRNG) via C++ `<random>` to generate robust passwords, with options to exclude ambiguous characters.
* **Educational Hub:** Integrated learning center explaining the math behind password security, data breaches, and password managers.

## 🛠️ Tech Stack

* **Core Logic:** C++
* **Compilation:** Emscripten (WASM)
* **Frontend:** Vanilla HTML5, CSS3, JavaScript
* **Data Visualization:** Chart.js
