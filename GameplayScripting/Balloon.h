#pragma once
#include "utils.h"
#include <iostream>
#include "Text.h"

enum class Weather
{
	normal,
	hot,
	windy,
	cold
};

class Balloon
{
public:
	Balloon(Rectf viewport);
	~Balloon();


	void Draw() const;
	bool GetExploded();
	void InflateBalloon(float elapsedSeconds);
	void Update(float elapsedSeconds);

	Weather GetWeather();
	void SetWeather(const Weather weather);

	float GetAmountInflated();
	void SetAmountInflated(float amount);
	void SetIsSpiked(bool isSpiked);
	void UpdateSpikePos();

private:
	void CheckExplosion();

	float m_AmountInflatedThisRound{};

	bool m_Exploded = false;
	Rectf m_Viewport;
	Ellipsef m_BalloonEllipse;
	Rectf m_BalloonHolder;

	Weather m_Weather{Weather::normal};
	Vector2f m_Velocity;
	TTF_Font* m_Font;
	Text* m_Text;

	bool m_IsSpiked;
	Point2f m_SpikePosition;
};

