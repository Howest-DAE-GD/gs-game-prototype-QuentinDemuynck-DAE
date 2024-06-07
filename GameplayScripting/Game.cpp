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
	m_FontPath = TTF_OpenFont("Font.ttf", 100);
	m_FontSmaller = TTF_OpenFont("Font.ttf", 20);
	m_BalloonPtr = new Balloon(GetViewPort());
	m_CurrentPlayerPtr = new Text("Current Player: 1", Color4f{ 1, 1, 1, 1 }, m_FontPath);

	m_ControlsPtr = new Text("Controls:", Color4f{ 1, 1, 1, 0.5 }, m_FontSmaller);
	m_ZPtr = new Text("Z: End turn", Color4f{ 1, 1, 1, 0.5 }, m_FontSmaller);
	m_SPtr = new Text("S: Place spike", Color4f{ 1, 1, 1, 0.5 }, m_FontSmaller);
	m_RPtr = new Text("Spacebar: Inflate", Color4f{ 1, 1, 1, 0.5 }, m_FontSmaller);
	m_SpacePtr = new Text("R:Restart", Color4f{ 1, 1, 1, 0.5 }, m_FontSmaller);
	m_SpikesEarnedPtr = new Text("Spikes earned", Color4f{ 1,1,1, 0.5 }, m_FontSmaller);

}

void Game::Cleanup( )
{
	delete m_BalloonPtr;
	m_BalloonPtr = nullptr;

	delete m_CurrentPlayerPtr;
	m_CurrentPlayerPtr = nullptr;

	delete m_ControlsPtr;
	m_ControlsPtr = nullptr;
	
	delete m_ZPtr;
	m_ZPtr = nullptr;

	delete m_RPtr;
	m_RPtr = nullptr;

	delete m_SPtr;
	m_SPtr = nullptr;

	delete m_SpacePtr;
	m_SpacePtr = nullptr;

	delete m_SpikesEarnedPtr;
	m_SpikesEarnedPtr = nullptr;
}

void Game::Update( float elapsedSec )
{
	const bool inflate{ bool(m_KeyStateArrPtr[SDL_SCANCODE_SPACE]) };
	const bool restart{ bool(m_KeyStateArrPtr[SDL_SCANCODE_R]) };
	const bool endTurn{ bool(m_KeyStateArrPtr[SDL_SCANCODE_W]) };
	const bool placeSpike{ bool(m_KeyStateArrPtr[SDL_SCANCODE_S]) };



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
		m_BalloonPtr->SetAmountInflated(0.0f);
		m_PlayerOneHasSpikes = false;
		m_PlayerTwoHasSpikes = false;
		m_SpikesPlaced = false;
		m_SpikesEarned = false;
		m_BalloonPtr->SetIsSpiked(false);
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
		m_BalloonPtr->SetIsSpiked(m_SpikesPlaced);
		m_BalloonPtr->UpdateSpikePos();

		if (m_PlayerOne && m_SpikesPlaced)
		{
			m_PlayerOneHasSpikes = false;
		}

		if (!m_PlayerOne && m_SpikesPlaced)
		{
			m_PlayerTwoHasSpikes = false;
		}
		if (m_SpikesEarned)
		{
			if (m_PlayerOne)
			{
				m_PlayerOneHasSpikes = true;
			}
			else
			{
				m_PlayerTwoHasSpikes = true;
			}
		}

		m_BalloonPtr->SetAmountInflated(0.0f);
		m_SpikesEarned = false;
		m_SpikesPlaced = false;


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
	std::cout << m_BalloonPtr->GetAmountInflated();
	if (m_BalloonPtr->GetAmountInflated() >= 8000 * elapsedSec)
	{
		m_SpikesEarned = true;
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

	if (placeSpike && m_PlayerOne && m_PlayerOneHasSpikes)
	{
		m_SpikesPlaced = true;
	}
	else if (placeSpike && !m_PlayerOne && m_PlayerTwoHasSpikes)
	{
		m_SpikesPlaced = true;
	}
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
	DrawCurrentPlayer();
	m_ControlsPtr->Draw(Point2f{ 120, GetViewPort().height / 2 });
	m_ZPtr->Draw(Point2f{ 120, (GetViewPort().height / 2) - 30 });
	m_RPtr->Draw(Point2f{ 120, (GetViewPort().height / 2) - 60 });
	m_SpacePtr->Draw(Point2f{ 120, (GetViewPort().height / 2) - 90 });
	m_SPtr->Draw(Point2f{ 120, (GetViewPort().height / 2) - 120 });

	if (m_SpikesEarned)
	{
		m_SpikesEarnedPtr->Draw(Point2f{ GetViewPort().width / 2, GetViewPort().height / 2 - 60 });
	}

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

void Game::DrawCurrentPlayer() const
{
	if (m_PlayerOne)
	{
		if(!m_BalloonPtr->GetExploded())
		{
			m_CurrentPlayerPtr->ChangeText("Current Player: One");
			m_CurrentPlayerPtr->Draw(Point2f{ GetViewPort().width / 2 , GetViewPort().height - m_CurrentPlayerPtr->GetTexture()->GetHeight() });
		}
		else
		{
			m_CurrentPlayerPtr->ChangeText("Player Two Won");
			m_CurrentPlayerPtr->Draw(Point2f{ GetViewPort().width / 2 , GetViewPort().height - m_CurrentPlayerPtr->GetTexture()->GetHeight() });
		}

		if (m_PlayerOneHasSpikes)
		{

			utils::SetColor(Color4f{ 0.3 ,0.3,0.3,1 });
			utils::FillTriangle(Point2f{1300 , 870}, Point2f{ 1350 ,870 }, Point2f{ 1325 , 935 });
		}
	}
	else
	{
		if (!m_BalloonPtr->GetExploded())
		{
			m_CurrentPlayerPtr->ChangeText("Current Player: Two");
			m_CurrentPlayerPtr->Draw(Point2f{ GetViewPort().width / 2 , GetViewPort().height - m_CurrentPlayerPtr->GetTexture()->GetHeight() });
		}
		else
		{
			m_CurrentPlayerPtr->ChangeText("Player One Won");
			m_CurrentPlayerPtr->Draw(Point2f{ GetViewPort().width / 2 , GetViewPort().height - m_CurrentPlayerPtr->GetTexture()->GetHeight() });
		}


		if (m_PlayerTwoHasSpikes)
		{

			utils::SetColor(Color4f{ 0.3 ,0.3,0.3,1 });
			utils::FillTriangle(Point2f{ 1300 , 870 }, Point2f{ 1350 ,870 }, Point2f{ 1325 , 935 });
		}
	}
}
void Game::SpikesEarnedText()
{

}
