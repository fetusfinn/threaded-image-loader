//
//
//
#include "downloader.h"

//
// Constructor
//
CDownloader::CDownloader() : m_bGlobalInit(false)
{
	Init();
}

// 
// Destructor
//
CDownloader::~CDownloader()
{
	curl_easy_cleanup(m_rCurl);
}

//
// Initialises cURL
//
void CDownloader::Init()
{
	if (m_bGlobalInit == false)
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		m_bGlobalInit = true;
	}

	m_rCurl = curl_easy_init();
}

//
// Downloads data from the given url to the given output str
//
bool CDownloader::Download(const char* _strUrl, std::string& _strOutput)
{

	m_rCurl = curl_easy_init();

	std::cout << "Attempting to download URL : " << _strUrl << "\n";

	if (m_rCurl)
	{
		CURLcode res;
		curl_easy_setopt(m_rCurl, CURLOPT_URL, _strUrl);
		curl_easy_setopt(m_rCurl, CURLOPT_WRITEFUNCTION, CDownloader::WriteData);
		curl_easy_setopt(m_rCurl, CURLOPT_WRITEDATA, &_strOutput);
		curl_easy_setopt(m_rCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

#ifdef FINN_DEBUG
		curl_easy_setopt(m_rCurl, CURLOPT_VERBOSE, 1L);
#endif

		res = curl_easy_perform(m_rCurl);

		if (res != CURLE_OK)
		{
			std::cerr << " CURL error: " << res << "\n";
		}

		curl_easy_cleanup(m_rCurl);

		return res == CURLE_OK;
	}

	return false;
}

//
//
//
size_t CDownloader::WriteData(void* _pBuffer, size_t _iSize, size_t _iNmemb, void* _pParam)
{
	std::string& text = *static_cast<std::string*>(_pParam);
	size_t totalsize = _iSize * _iNmemb;
	text.append(static_cast<char*>(_pBuffer), totalsize);
	return totalsize;
}
