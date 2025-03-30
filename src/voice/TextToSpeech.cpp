#include "include/voice/TextToSpeech.h"
#include <iostream>
#include <Windows.h>
#include <sapi.h>
#pragma comment(lib, "sapi.lib")

TextToSpeech::TextToSpeech() : espeakInstance(nullptr) {
}

TextToSpeech::~TextToSpeech() {
    shutdown();
}

bool TextToSpeech::initialize() {
    CoInitialize(NULL);
    return true;
}

bool TextToSpeech::speak(const std::string& text) {
    if (text.empty()) {
        return false;
    }

    ISpVoice* pVoice = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

    if (SUCCEEDED(hr)) {
        // Convert ASCII text to wide characters
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
        std::wstring wstrText(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wstrText[0], size_needed);

        hr = pVoice->Speak(wstrText.c_str(), 0, NULL);
        pVoice->Release();
        return SUCCEEDED(hr);
    }

    return false;
}

void TextToSpeech::shutdown() {
    CoUninitialize();
}