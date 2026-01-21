#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include "ChessCase.h"
#include <string>
#include <iomanip> // for setw() : cout
#include <array>
#include <ctime>


#define RED_COUT   "\033[31m"
#define BLUE_COUT  "\033[34m"
#define RESET_COUT "\033[0m"
#define GREEN_COUT "\033[32m"
#define YELLOW_COUT "\033[33m"
#define WHITE_COUT "\033[37m"

class Chess : public Game
{
private:

	struct stDate {
		short year;
		short month;
		short day;
		short hour;
		short minute;
		short second;
	};

	struct PossibleMouvement {
		Position possiblePosition;
		ChessCase possibleCase;
		int movementType;
		bool canMove;
	};
	

	enum enActionType {
		MOUVEMENT = 0,
		CHECK_CASES = 1,

	};

	enum enPlayerNum {
		PLAYER1 = 0,
		PLAYER2 = 1,
	};

	struct stMove {
		ChessCase fromCell;
		ChessCase toCell;
		// piece info
		int pieceTeamIndex;
		enPlayerNum pieceTeamSide;
	};

	struct stPiece {
		bool pieceTeamSide;
		int pieceTeamIdx;
	};

	Grid* grid;
	Piece* piece;

	//window
	int windowWidth, windowHeigh;
	int gameFps;
	Image m_screenshot;

	//grid
	int numRows;
	int numCols;
	int cellSize;
	int leftMargin, topMargin;
	Color m_color1, m_color2, m_hoveredColor, m_backgroundColor;
	struct stTheme {
		Color color1;
		Color color2;
		Color heveredColor;
		Color backgroundColor;
	};

	//pieces
	std::vector<std::unique_ptr<Piece>> player1, player2;
	int selectdPieceID;;
	Position selectedPieceOriginalPos , selectedPieceCurrentPos;

	std::vector<PossibleMouvement> m_possibleMouvement;

	//moves history 
	std::string m_WhiteMove, m_BlackMove , m_turnMove;
	std::vector<std::string> m_allMoves;
	int m_moveIdx;

	//text 
	float m_xText;
	int m_sizeText, m_spacingText;
	Font m_fontText;


	//Board case
	struct infoCase
	{
		bool empty;
		int idCell;
		int idPiece;
		int pieceTeamIndex;
		Position cellPosition;
		Position cellCentralPosition;
		ChessCase cellCoordinates;
		std::string pieceName;
		std::string caseName;
		int playerSide;
		//bool caseHovered;
		infoCase(); // construtor
		void setToEmpty();
	};
	using Board = std::array <std::array<infoCase, 8>, 8>;  // using = type def (modern) : Boerd is the new type 
	Board m_board , m_Initial_Board;

	//flags
	bool    flag_isPlayer1Turn,
			flag_leftMouseButtonPressed,
			flag_rightMouseButtonPressed,
			flag_leftMouseButtonDown,
			flag_leftMouseButtonReleased,
			flag_isAnyPieceSelected,
			flag_player1InCheck,
			flag_player2InCheck,
			flag_checkMate,
			flag_isMovementAllowed,
			flag_isAnyPieceCaptured,
			flag_possibleMouvemntsAreCalculated,
			flag_isTheme1;


public :
	Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin, Color c1, Color c2);
	~Chess();

	//----------------------
	// principal functions
	//----------------------
	void Init();  
	void Input();
	void Update(); 
	void Draw();

	//----------------------
	// additional functions
	//----------------------
	void setGameFPS(int fps);
	void DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player);
	void MovePieceByStep(Piece& piece, Position step);
	void MovePieceToNewPos(Piece& piece, Position newPos);
	void MovePieceToNewCase(Piece& piece, ChessCase CCase);
	void ChangeTurn();
	void ChangeTheme();

	void DragPiece();
	void ReleasePiece();
	void CorrectDragedPiecePosition();
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);
	Position GetCorrespondingBoardCase(Position centerPos);
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);
	void SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[]);
	void DrawPossiblePositions(Color color);
	bool GetPossiblePositionsOnBoard(	Piece const& piece, 
										std::vector<PossibleMouvement> & PossibleMvt , 
										enActionType const & ActionType);
	bool GenerateLegalMoves(	ChessCase const & cellCoords, 
								Board const & board, 
								std::vector<PossibleMouvement>& PossibleMvt, 
								enActionType const& ActionType);
	bool  IsAnyPieceCaptured(Piece const& piece);
	bool IsPlayerInCheckPosition(Board const& board , enPlayerNum const & playerNum);
	bool IsTurnFinished();
	Color GetBackgroundColor();

	//conversion cell to pos
	ChessCase PosToCase(Position pos);
	Position CaseToPos(ChessCase CCase);


	// m_borad fonctions
	Position GetCellPosition(int i, int j);
	Position GetCellCentralPosition(int i, int j);
	Position GetPostionFromBoardCaseName(std::string const &caseName) const;
	void InitBoardInfo();
	std::string GetCaseName(int i, int j);
	void UpdateCellInfo(Piece const& piece , int row, int col, Board &board);
	void SetCellInfo(Piece const& piece, int row, int col, Board& board);
	void UpdateBoardInfo(bool player1Turn);
	void SetEmptyBoardInfo(bool player1Turn);
	void SetNewValuesToBoardInfo(bool player1Turn);
	bool Compare2Cells(infoCase const& cell1, infoCase const& cell2);
	bool IsSameCoordinates(ChessCase const& cell1, ChessCase const& cell2);
	void GetBoardRowColFromCaseName(std::string caseName , int& row, int& col);
	void GetBoardRowColFromPiecePosition(Position const & pos, int& row, int& col , bool beforePieceReleased) const;
	void PrintBoardQuickInfo(std::string infoType , Board const & board) const;
	int  AddPossiblePossition(	enActionType const& ActionType ,
								Piece const& piece , 
								std::vector<PossibleMouvement> &PossibleMvt, 
								int const &row, 
								int const &col ) ;
	int AddPossiblePossitionFromBoard(  enActionType const& ActionType,
										Board const& board,
										std::vector<PossibleMouvement>& PossibleMvt,
										int const& row,
										int const& col,
										ChessCase const& attackerCoords);
	bool IsCapturableObstacle(int const &row, int const &col) const;
	bool IsCapturableObstacle_2(int const& row, int const& col, Board const &board) const;
	bool IsEnemyObstacle(int const& row, int const& col) const;
	bool IsEnemyObstacle_2(int const& row, int const& col, ChessCase const& attackerCoords , Board const& board) const;
	bool IsCheckPosition(int const&row , int const &col, Piece const& piece) const;
	bool IsCheckPosition_2(int const& row, int const& col, ChessCase const& attackerCoords, Board const & board) const;
	bool IsAnyCheckPossitionExists(std::vector<PossibleMouvement> const & PossibleMvt);  // for each piece
	ChessCase GetAttackedKingCoordsOnTheBoard(bool const & attackedSide, Board const& board) const;
	bool IsSelectedMoveLegal(Piece const &piece, ChessCase const & nextPosition , Board const & board , enPlayerNum const& playerNum);

	bool IsLegalMove(stMove const & move, Board const& board);
	stMove SetMoveInfo(ChessCase const& fromCell, ChessCase const& toCell, int pieceTeamIndex ,enPlayerNum playerSide);
	std::string GetCurrentMoveText(stMove const & move);

	void ValidateCurrentMove( ChessCase & selectedMoveCell );
	stPiece GetPieceFromBoardCell(infoCase const& cell);
	void PionPromotion();
	bool IsCheckmat();

	bool IsValidIdx(int const &row, int const &col) const;
	void ErrorIndex(int row, int col);

	// text
	void DrawPlayerTurn();
	void DrawPlayerInCheck();
	void DrawCheckMate();
	void DrawTime();
	void DrawLastMoves();
	void DrawLetters();
	void DrawLateralTexts();

	// time
	stDate GetSystemDateTime(void);
	std::string GetDateTimeToString(stDate const& date, std::string sep = ":");
};


/*check mat
* 1 player1 turn + player1 in check // should be the first iff
* 2 all roi positions are invalide + ( its own position // no need)
* 3 all player1 pieces possible positions are invalid
*/

/* rock roi side : to be added to roi possible positions
* 1 selectedPiece = king // no need
* 2 king never moved && tour 13 never moved
* 3 empty cells between king & tour && no check on this cases
* 4 king moves to cavalier 11 case
* 5 tour moves to fou 9 case
*/


// isPat
// priseEnPassant
// 3 times same position
// materiel insufisant roi vs roi

// temp, fin de partie en temps,
// son, mvt, echec, mat
// orientation du plateaux

// historique des coups,
// annuler / refaire un coups / redemarrer la partie / abondon
// sauvegarde / chargement de partie