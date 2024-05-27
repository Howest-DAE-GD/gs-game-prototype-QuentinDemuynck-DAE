#include "pch.h"
#include "Balloon.h"

Balloon::Balloon(Rectf viewport)
	:m_Viewport(viewport), m_BalloonHolder{ viewport.width / 2.f - 2.5f, -250, 5, 370 }, m_BalloonEllipse(viewport.width / 2.f, 180, 45, 60)
{
	
}

void Balloon::Draw() const
{
	float odds = (1.0f / (int(90000.f / m_BalloonEllipse.radiusY) + 1.0f) *100 );
	if (odds > 1)
	{
		odds = 1;
	}
	else if (odds < 0)
	{
		odds = 0;
	}

	utils::SetColor(Color4f{ 0.8,1,1,1 });
	utils::FillRect(m_BalloonHolder);
	utils::SetColor(Color4f{ 1,1 - odds,1 - odds,1 });
	utils::FillEllipse(m_BalloonEllipse);
}

void Balloon::Update(float elapsedSeconds)
{
	if (m_Exploded)
	{
		m_Velocity.y += 10 * elapsedSeconds;
		m_BalloonEllipse.center.y += m_Velocity.y * elapsedSeconds;
		m_BalloonHolder.bottom += m_Velocity.y * elapsedSeconds;

		if(rand() % 30 == 0)
		{
			m_Velocity.x = (rand() % 300 - 150) * elapsedSeconds;
		}

		m_BalloonEllipse.center.x += 150 * m_Velocity.x * elapsedSeconds;
		m_BalloonHolder.left += 150 * m_Velocity.x * elapsedSeconds;
	}
}

bool Balloon::GetExploded()
{
	return m_Exploded;
}


void Balloon::CheckExplosion()
{
	int odds{ (rand() % 100 + rand() * 100) % (int(90000.f / m_BalloonEllipse.radiusY) + 3) };

	switch (m_Weather)
	{
	case Weather::normal:
		break;
	case Weather::hot:
		odds = (rand() % 100+ rand() * 100) % (int(40000.f / m_BalloonEllipse.radiusY) + 3);
		break;
	case Weather::windy:
		break;
	case Weather::cold:
		odds = (rand() % 100 + rand() *100) % (int(350000.f / m_BalloonEllipse.radiusY) + 3);
		break;
	default:
		break;
	}

	std::cout << odds << std::endl;
	if (odds == 0)
	{
		m_Exploded = true;
		m_Velocity.y = 85;
	}
}

void Balloon::InflateBalloon(float elapsedSeconds)
{
	m_BalloonEllipse.radiusY += 80 * elapsedSeconds;
	m_BalloonEllipse.radiusX = m_BalloonEllipse.radiusY * 3 / 4;
	m_BalloonEllipse.center.y = 120 + m_BalloonEllipse.radiusY;

	if(m_BalloonEllipse.radiusY > 180)
	CheckExplosion();
}

Weather Balloon::GetWeather()
{
	return m_Weather;
}

void Balloon::SetWeather(const Weather weather)
{
	m_Weather = weather;
}