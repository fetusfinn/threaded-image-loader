//
//
//

#include "image_handler.h"
#include "task_download.h"
#include "task_update.h"

//
//
//
CImageHandler::CImageHandler()
{

}

//
//
//
CImageHandler::~CImageHandler()
{
	// destroy textures
	// Destroy sprites
}

//
// Splits the downloaded URLs into our url vector
//
void CImageHandler::SplitUrls(std::string& _strData)
{
	size_t pos = 0;
	size_t oldPos = 0;
	while (pos != std::string::npos)
	{
		pos = _strData.find('\n', oldPos);
		if (oldPos < _strData.length())
		{
			m_vUrls.push_back(_strData.substr(oldPos, pos - oldPos));
			printf("url [%zd] : %s\n", m_vUrls.size() - 1, m_vUrls[m_vUrls.size() - 1].c_str());
			oldPos = pos + 1;
		}
	}
}

//
//
//
void CImageHandler::DownloadAndUpdate(int _iPage, const int _kiNumPages, short _iDir, sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	// The index of the first image we're trying to download in our vector
	int iOffset = _iPage * 4;
	size_t iSize = m_vUrls.size();

	for (int i = 0; i < 4; i++)
	{
		// If we're at end of the vector we dont want to try access
		// elements outside our range, all remaining sprites wont be shown
		if (iOffset + i >= iSize)
		{
			m_bIgnore[i] = true;
			m_arrData[i] = "";
			continue;
		}

		CTask* rTask = new CTaskDownload(m_vUrls.at(iOffset + i).c_str(), &m_arrData[i], &m_bIgnore[i]); // , & m_arrTextures[i], & _arrSprites[i], & _rWindow);

		Global::rPool.AddTask(rTask);
	}

	// Wait til all download have complete
	Global::rPool.WaitTilFinished();

	// Wait a little
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Then update our textures and sprites
	this->Update(_rWindow, _arrSprites);
}

//
// Updates all the textures and sprites with the 
//
void CImageHandler::Update(sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	for (int i = 0; i < 4; i++)
	{
		CTask* rTask = new CTaskUpdate(&m_arrData[i], &m_arrTextures[i], &_arrSprites[i], &_rWindow);

		Global::rPool.AddTask(rTask);
	}
}

//
// Moves everything forward/backwards depending on the direction that we're moving
//
void CImageHandler::ChangePage(short _iDir, sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	for (int i = 0; i < 4; i++)
	{
		if (_iDir == 1)
		{
			// Move forward a page

			// The page we were just on now becomes the previous page
			m_arrDataPrev[i] = m_arrData[i];

			// And the nex page now becomes the current page
			m_arrData[i] = m_arrDataNext[i];
		}
		else if (_iDir == 2)
		{
			// Move back a page

			// The page we were just on now becomes the next page
			m_arrDataNext[i] = m_arrData[i];

			// And the previous page now becomes the current page
			m_arrData[i] = m_arrDataPrev[i];
		}
	}

	this->Update(_rWindow, _arrSprites);
}

//
// Downloads all images for the next page
//
void CImageHandler::DownloadNextPage(int _iPage, const int _kiNumPages)
{
	int iOffset = (_iPage + 1) * 4;

	// If we're on the last page then the next page is
	// the first page, so offset will be 0
	if (_iPage+1 == _kiNumPages)
	{
		iOffset = 0;
	}

	size_t iSize = m_vUrls.size();

	for (int i = 0; i < 4; i++)
	{
		// If we're at end of the vector we dont want to try access
		// elements outside our range, all remaining sprites wont be shown
		if (iOffset + i >= iSize)
		{
			m_bIgnore[i] = true;
			m_arrData[i] = "";
			continue;
		}

		CTask* rTask = new CTaskDownload(m_vUrls.at(iOffset + i).c_str(), &m_arrDataNext[i], &m_bIgnore[i]);

		Global::rPool.AddTask(rTask);
	}

	printf("Next page downloaded!\n");
}

//
// Downloads all the images for the previous page
//
void CImageHandler::DownloadPreviousPage(int _iPage, const int _kiNumPages)
{
	int iOffset = (_iPage - 1) * 4;

	// If we're on the first page then the previous page is
	// the last page, so offset will be numpages * 4
	if (_iPage == 0)
	{
		iOffset = (_kiNumPages - 1) * 4;
	}

	size_t iSize = m_vUrls.size();

	for (int i = 0; i < 4; i++)
	{
		// If we're at end of the vector we dont want to try access
		// elements outside our range, all remaining sprites wont be shown
		if (iOffset + i >= iSize)
		{
			m_bIgnore[i] = true;
			m_arrData[i] = "";
			continue;
		}

		CTask* rTask = new CTaskDownload(m_vUrls.at(iOffset + i).c_str(), &m_arrDataPrev[i], &m_bIgnore[i]);

		Global::rPool.AddTask(rTask); 
	}

	printf("Previous page downloaded!\n");
}

//
// Draw all sprites
//
void CImageHandler::Draw(sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	// TODO : For all ignores, have text saying "image unavailable"

	for (int i = 0; i < 4; i++)
	{
		if (!m_bIgnore[i])
		{
			_rWindow.draw(_arrSprites[i]);
		}
	}
}



//
// Unused v
//

//
//
//
void CImageHandler::Download(int _iPage, int _iDir)
{
	// The index of the first image we're trying to download in our vector
	int iOffset = _iPage * 4;
	size_t iSize = m_vUrls.size();

	/*
	for (int i = 0; i < 4; i++)
	{
		// If we're at end of the vector we dont want to try access
		// elements outside our range, all remaining sprites wont be shown
		if (iOffset + i >= iSize)
		{
			m_bIgnore[i] = true;
			continue;
		}

		// TODO
		if (_iDir == 1)
		{
			m_arrDataPrev[i] = m_arrData[i];

			// Load the images for the next page
		}
		else if (_iDir == 2)
		{
			m_arrDataNext[i] = m_arrData[i];

			// Load the images for the previous page
		}
		else if (_iDir == 0)
		{
			// Load the previous images
			// Load the next times
		}

		// Clear our data to be reused again
		m_arrData[i] = "";

		// Try to download the images
		if (_rDownloader.Download(m_vUrls.at(iOffset + i).c_str(), m_arrData[i]))
		{
			m_bIgnore[i] = false;
		}
		else
		{
			// Failed to download, dont draw
			m_bIgnore[i] = true;
			std::cout << "Download failed\n";
		}
	}


	std::function<void()> rFunc = [&]()
	{
		int i = 0;

		printf("Download lambda\n");

		// Clear our data to be reused again
		m_arrData[i] = "";

		// Try to download the images
		if (Global::rDownloader.Download(m_vUrls.at(iOffset + i).c_str(), m_arrData[i]))
		{
			m_bIgnore[i] = false;
		}
		else
		{
			// Failed to download, dont draw
			m_bIgnore[i] = true;
			printf("Download failed\n");
		}
	};

	for (int i = 0; i < 4; i++)
	{
		// If we're at end of the vector we dont want to try access
		// elements outside our range, all remaining sprites wont be shown
		if (iOffset + i >= iSize)
		{
			m_bIgnore[i] = true;
			continue;
		}

		// Create a download task to be executed by our thread pool
		// CTask* rTask = new CTaskDownload(m_vUrls.at(iOffset + i).c_str(), &m_arrData[i], &m_bIgnore[i]);

		// Global::rPool.Submit(rTask);
	}

	*/
}

//
// Loads the new images from memory and updates the sprites with them
//
void CImageHandler::UpdateTexturesAndSprites(const sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	// Wait for threads to finish before doing anything
	Global::rPool.Stop();

	// TODO : use threads

	// Used to scale the sprite to a quarter of the screen
	float fXScale = 0.f, fYScale = 0.f;

	for (int i = 0; i < 4; i++)
	{
		// Try load the image from memory
		if (!m_arrTextures[i].loadFromMemory(m_arrData[i].c_str(), m_arrData[i].size()))
		{
			std::cout << "Failed to load texture\n";
			continue;
		}

		// Calculate size to scale to
		fXScale = 1.f / (float(m_arrTextures[i].getSize().x) / float(_rWindow.getSize().x)) / 2.f;
		fYScale = 1.f / (float(m_arrTextures[i].getSize().y) / float(_rWindow.getSize().y)) / 2.f;

		// Set the sprites texture
		_arrSprites[i].setTexture(m_arrTextures[i], true);

		// Scale sprite to quarter of the screen
		_arrSprites[i].setScale(fXScale, fYScale);
	}
}
