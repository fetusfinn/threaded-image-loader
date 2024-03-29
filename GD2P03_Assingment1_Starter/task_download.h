//
// task_download.h
//
#pragma once

#include <string>

#include "globals.h"
#include "task.h"

class CTaskDownload : public CTask
{
private:

	// Url to download from
	const char* m_strUrl;

	// Pointer to the data string that we're writing to
	std::string* m_pData;

	// Pointer to the ignore bool for this image
	bool* m_pIgnore;

	// Pointer to the texture for this image
	sf::Texture* m_pTexture;

	// Pointer to the sprite for this image
	sf::Sprite* m_pSprite;

	// Pointer to our window
	sf::RenderWindow* m_pWindow;

public:

	CTaskDownload(const char* _strUrl, std::string* _pData, bool* _pIgnore, sf::Texture* _pTex = nullptr, sf::Sprite* _pSpr = nullptr, sf::RenderWindow* _pWindow = nullptr) : m_strUrl(_strUrl), m_pData(_pData), m_pIgnore(_pIgnore), m_pTexture(_pTex), m_pSprite(_pSpr), m_pWindow(_pWindow)
	{
		printf("Download task created\n");
	}

	~CTaskDownload()
	{
		
	}

	//
	// Downloads a given image to memory
	//
	void DoWork() override
	{
		// Clear our data to be reused again
		*m_pData = "";

		// Create a downloader object for this thread
		// so it doesnt interfere with other download threads
		CDownloader rDownloader;

		// Try to download the images
		if (rDownloader.Download(m_strUrl, *m_pData))
		{
			*m_pIgnore = false;
		}
		else
		{
			// Failed to download, dont draw
			*m_pIgnore = true;
			printf("Download failed\n");

			return;
		}

		printf("Image downloaded!\n");

		// If we're only downloading the page then next the update part
		if (m_pTexture == nullptr)
			return;

		// Try load the image from memory
		if (!m_pTexture->loadFromMemory(m_pData->c_str(), m_pData->size()))
		{
			printf("Failed to load texture\n");
			return;
		}

		// Calculate size to scale to
		float fXScale = 1.f / (float(m_pTexture->getSize().x) / float(m_pWindow->getSize().x)) / 2.f;
		float fYScale = 1.f / (float(m_pTexture->getSize().y) / float(m_pWindow->getSize().y)) / 2.f;

		// Set the sprites texture
		m_pSprite->setTexture(*m_pTexture, true);

		// Scale sprite to quarter of the screen
		m_pSprite->setScale(fXScale, fYScale);

		printf("Update task finished\n");
	}

};