#pragma once

#define CURL_STATICLIB //removing this will give link warnings
#include <curl/curl.h>

#include <string>
#include <iostream>
#include <mutex>

//this class will download fromm urls to a specified string.
class CDownloader
{
private:

	// Handle
	CURL* m_rCurl = nullptr;

	bool m_bGlobalInit;

	std::mutex m_rMutex;

	static size_t WriteData(void* _pBuffer, size_t _iSize, size_t _iNmemb, void* _pParam);

public:

	CDownloader();
	~CDownloader();

	void Init();

	bool Download(const char * _strUrl, std::string& _strOutput);

};