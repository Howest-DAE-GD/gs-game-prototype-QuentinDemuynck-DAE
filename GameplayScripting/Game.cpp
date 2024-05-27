#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{

	m_BalloonPtr = new Balloon(GetViewPort());
}

void Game::Cleanup( )
{
	delete m_BalloonPtr;
	m_BalloonPtr = nullptr;
}

void Game::Update( float elapsedSec )
{
	const bool inflate{ bool(m_KeyStateArrPtr[SDL_SCANCODE_SPACE]) };
	const bool restart{ bool(m_KeyStateArrPtr[SDL_SCANCODE_R]) };
	const bool endTurn{ bool(m_KeyStateArrPtr[SDL_SCANCODE_W]) };


	if (restart && m_BalloonPtr->GetExploded())
	{
		delete m_BalloonPtr;
		m_BalloonPtr = new Balloon(GetViewPort());

		int playerStart = rand() % 2;
		if (playerStart == 0)
		{
			m_PlayerOne = !m_PlayerOne;
		}
		m_TimePressed = 0;
	}

	m_BalloonPtr->Update(elapsedSec);
	if (m_BalloonPtr->GetExploded())
	{
		return;
	}

	if (inflate)
	{
		m_BalloonPtr->InflateBalloon(elapsedSec);
		m_TimePressed += elapsedSec;
	}

	if (endTurn && m_TimePressed > 0)
	{
		m_PlayerOne = !m_PlayerOne;
		m_TimePressed = 0;

		int randomWeather{ rand() % 4 };
		Weather newWeather{};

		switch (randomWeather)
		{
		case 0:
			newWeather = Weather::normal;
			break;
		case 1:
			newWeather = Weather::windy;
			break;
		case 2:
			newWeather = Weather::hot;
			break;
		case 3:
			newWeather = Weather::cold;
			break;
		default:
			break;
		}
		m_BalloonPtr->SetWeather(newWeather);
	}

	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );
	utils::SetColor(Color4f{ 1,0,0,1 });

	if (m_PlayerOne)
	{
		utils::FillEllipse(0, 0, 50, 50);
	}
	else
	{
		utils::FillEllipse(GetViewPort().width, 0, 50, 50);
	}

	m_BalloonPtr->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	switch (m_BalloonPtr->GetWeather())
	{
	case Weather::normal:
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	case Weather::hot:
		glClearColor(0.8f, 0.3f, 0.2f, 1.0f);
		break;
	case Weather::windy:
		glClearColor(0.8f, 0.8f, 1.f, 1.0f);
		break;
	case Weather::cold:
		glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
		break;
	default:
		break;
	}
	glClear( GL_COLOR_BUFFER_BIT );
}
