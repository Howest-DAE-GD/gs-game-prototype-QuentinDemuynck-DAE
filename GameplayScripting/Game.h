#pragma once
#include "BaseGame.h"
#include "Balloon.h"
#include "Text.h"
#include "vector"
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	Balloon* m_BalloonPtr;
	Text* m_CurrentPlayerPtr;
	TTF_Font* m_FontPath;
	TTF_Font* m_FontSmaller;

	Text* m_ControlsPtr;
	Text* m_RPtr;
	Text* m_ZPtr;
	Text* m_SpacePtr;
	Text* m_SpikesEarnedPtr;
	Text* m_SPtr;

	bool m_PlayerOneHasSpikes{ false };
	bool m_PlayerTwoHasSpikes{ false };
	bool m_SpikesPlaced{ false };
	bool m_SpikesEarned{ false };


	const Uint8* m_KeyStateArrPtr{ SDL_GetKeyboardState(nullptr) };

	bool m_PlayerOne{false};
	float m_TimePressed{0};

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawCurrentPlayer() const;
	void SpikesEarnedText();
};