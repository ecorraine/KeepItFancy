#ifndef GAME_H
#define GAME_H

class APPLICATION;

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! GAME Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class GAME Game.h "Game.h"
/*! \brief The Game Class where all Scene Objects are managed
 *  \brief 全てのシーンオブジェクトを管理するゲームクラス
 */
class GAME
{
public:
	static void InitGame(APPLICATION* pApp);
	static void ReleaseGame();
	static void UpdateGame(float tick);
	static void DrawGame();
};

#endif // !GAME_H