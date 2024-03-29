//
//
//
#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "downloader.h"
#include "thread_pool.h"

class CImageHandler
{
private:

	// List of all our urls
	std::vector<std::string> m_vUrls;

	// Array of strings for the 4 images to be shown on screen
	std::string m_arrData[4];

	// Data for the images on the previous page to what we're on
	std::string m_arrDataPrev[4];

	// Data for the images on the next page
	std::string m_arrDataNext[4];

	// Array of textures that will be used to display our images
	sf::Texture m_arrTextures[4];

	// Whether or not we should ignore the image and not draw it
	bool m_bIgnore[4] = { false, false, false, false };

public:

	CImageHandler();
	~CImageHandler();


	void SplitUrls(std::string& _strData);

	void DownloadAndUpdate(int _iPage, const int _kiNumPages, short _iDir, sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites);

	void Update(sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites);

	void ChangePage(short _iDir, sf::RenderWindow& _rWindow, sf::Sprite* _arrSprite);

	void DownloadNextPage(int _iPage, const int _kiNumPages);

	void DownloadPreviousPage(int _iPage, const int _kiNumPages);

	void Draw(sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites);

	std::vector<std::string> GetUrls() const { return m_vUrls; }


	void Download(int _iPage, int _iDir);
	void UpdateTexturesAndSprites(const sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites);
};
