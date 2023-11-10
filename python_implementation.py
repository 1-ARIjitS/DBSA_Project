import pgntofen

class chessgame:
    def __init__(self,san):
        self.san = san

class chessboard:
    def __init__(self, fen):
        self.fen = fen

def getBoard(chessgame, half_move_count):
    converter = pgntofen.PgnToFen()

    for move in chessgame.san.split(' ')[:half_move_count]:
        converter.move(move)

    # Convert to FEN and return
    return converter.getFullFen()

def get_first_moves(chessgame, half_move_count):
    # Return the first N half-moves
    return chessgame.san.split(' ')[:half_move_count]

def has_opening(chessgame1, chessgame2):
    # Compare the opening moves of this game with another ChessGame
    # Just with the subset check implementation
    return False

def has_board(chessgame1, chessboard1, half_move_count):
    # Iterate through the first N half-moves
    converter = pgntofen.PgnToFen()
    for move in chessgame1.san.split(' ')[:half_move_count]:
        converter.move(move)
        if converter.getFullFen() == chessboard1.fen:
            return True
    return False

if __name__ == "__main__":
    san = 'c4 Nc6 Nc3 e5 Nf3 Nf6 g3 d5 cxd5 Nxd5 Bg2 Nb6 O-O Be7 a3 Be6 b4 a5 b5 Nd4 Nxd4 exd4 Na4 Bd5 Nxb6 cxb6 Bxd5'
    chessgame_1 = chessgame(san)
    print(getBoard(chessgame_1,10))
    print(get_first_moves(chessgame_1,5))
    fen = 'r2qk2r/1p2bppp/1p6/pP1B4/3p4/P5P1/3PPP1P/R1BQ1RK1 b - kq'
    print(has_board(chessgame_1,chessboard(fen),100))



