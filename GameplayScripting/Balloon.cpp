#include "pch.h"
#include "Balloon.h"

Balloon::Balloon(Rectf viewport)
	:m_Viewport(viewport), m_BalloonHolder{ viewport.width / 2.f - 2.5f, -250, 5, 370 }, m_BalloonEllipse(viewport.width / 2.f, 180, 45, 60)
{
	m_Font = TTF_OpenFont("Font.ttf", 20);
	
	m_Text = new Text("", Color4f{ 1,1,1,1 }, m_Font);
}

Balloon::~Balloon()
{
	delete m_Text;
	m_Text = nullptr;
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

	if (m_IsSpiked && !m_Exploded)
	{
		utils::SetColor(Color4f{ 0.5 ,0.5,0.5,1 });
		utils::FillTriangle(Point2f{ m_SpikePosition }, Point2f{ m_SpikePosition.x + 30, m_SpikePosition.y + 50 }, Point2f{ m_SpikePosition.x - 30, m_SpikePosition.y + 50 });
	}
	switch (m_Weather)
	{
	case Weather::normal:;

		m_Text->ChangeText("Normal Weather");
		m_Text->Draw(Point2f{ m_Viewport.width / 2 , m_Viewport.height / 2 });

		break;
	case Weather::hot:
		m_Text->ChangeText("It's hot, odds are bad");
		m_Text->Draw(Point2f{ m_Viewport.width / 2 , m_Viewport.height / 2 });


		break;
	case Weather::windy:
		m_Text->ChangeText("It's windy, change of deflating");
		m_Text->Draw(Point2f{ m_Viewport.width / 2 , m_Viewport.height / 2 });

		break;
	case Weather::cold:
		m_Text->ChangeText("It's cold, odds are good");
		m_Text->Draw(Point2f{ m_Viewport.width / 2 , m_Viewport.height / 2 });


		break;
	default:
		break;
	}
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
	float spikeMultyplyer{ 1 };
	if (m_IsSpiked)
	{
		spikeMultyplyer = 0.8;
	}
	int odds{ (rand() % 100 + rand() * 100) % (int(((90000.f / m_BalloonEllipse.radiusY) + 3) * spikeMultyplyer) )};
	int deflate{};
	deflate = rand() % 25;
	 
	switch (m_Weather)
	{
	case Weather::normal:
		break;
	case Weather::hot:
		odds = (rand() % 100+ rand() * 100) % (int(((40000.f / m_BalloonEllipse.radiusY) + 3) * spikeMultyplyer) );
		break;
	case Weather::windy:

		if (deflate == 0)
		{
			int amount{ rand() % 20 };
			if (m_BalloonEllipse.radiusY >= 180.f)
			{
				m_BalloonEllipse.radiusX -= float(amount) * float(3 / 4);
				m_BalloonEllipse.radiusY -= float(amount);
				std::cout << "defalting; " << amount;
				if (m_BalloonEllipse.radiusY < 80)
				{
					m_BalloonEllipse.radiusY = 80;
					m_BalloonEllipse.radiusX = 80 * 3 / 4;
				}
			}
		}
		break;
	case Weather::cold:
		odds = (rand() % 100 + rand() *100) % (int(((350000.f / m_BalloonEllipse.radiusY) + 3) * spikeMultyplyer));
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
	m_AmountInflatedThisRound += 80 * elapsedSeconds;
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

float Balloon::GetAmountInflated()
{
	return m_AmountInflatedThisRound;
}

void Balloon::SetAmountInflated(float amount)
{
	m_AmountInflatedThisRound = amount;
}

void Balloon::SetIsSpiked(bool isSpiked)
{
	m_IsSpiked = isSpiked;
}
void Balloon::UpdateSpikePos()
{
	m_SpikePosition = m_BalloonEllipse.center;
}