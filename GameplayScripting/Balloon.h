#pragma once
#include "utils.h"
#include <iostream>

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

	void Draw() const;
	bool GetExploded();
	void InflateBalloon(float elapsedSeconds);
	void Update(float elapsedSeconds);

	Weather GetWeather();
	void SetWeather(const Weather weather);


private:
	void CheckExplosion();

	bool m_Exploded = false;
	Rectf m_Viewport;
	Ellipsef m_BalloonEllipse;
	Rectf m_BalloonHolder;

	Weather m_Weather{Weather::normal};
	Vector2f m_Velocity;
};

