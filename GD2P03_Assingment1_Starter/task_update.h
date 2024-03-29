//
//
//
#pragma once

#include <SFML/Graphics.hpp>

#include "task.h"

class CTaskUpdate : public CTask
{
private:

	// Pointer to the data to load this image from
	std::string* m_pData;

	// Pointer to the texture for this image
	sf::Texture* m_pTexture;

	// Pointer to the sprite for this image
	sf::Sprite* m_pSprite;

	// Pointer to our window
	sf::RenderWindow* m_pWindow;

public:

	CTaskUpdate(std::string* _pData, sf::Texture* _pTex, sf::Sprite* _pSpr, sf::RenderWindow* _pWindow) : m_pData(_pData), m_pTexture(_pTex), m_pSprite(_pSpr), m_pWindow(_pWindow)
	{
		printf("Update task created!\n");
	}

	~CTaskUpdate()
	{
		
	}

	//
	// Updates the given texture and sprite with a new image
	//
	void DoWork() override
	{
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