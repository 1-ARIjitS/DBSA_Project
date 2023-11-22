import chess.pgn
import os

txt = open('loading.sql','w')
path = '/Games/'

# First, we create the table in postgres

txt.write("CREATE TABLE pgn(event VARCHAR(30),site VARCHAR(20),date VARCHAR(10),round VARCHAR(4),white VARCHAR(20),black VARCHAR(20),result VARCHAR(7),white_elo VARCHAR(5),black_elo VARCHAR(5),eco VARCHAR(3),san gamechess);\n")

# Once the table is created, we can upload all the data on it.

for fname in os.listdir(path):
    with open("Carlsen.pgn",'r') as pgn:
        while True:
            game = chess.pgn.read_game(pgn)
            query = "INSERT INTO pgn (event, site, date, round, white, black, result, white_elo, black_elo, eco, san)"
            values =" VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s');\n" %(game.headers["Event"],game.headers["Site"], game.headers["Date"], game.headers["Round"], game.headers["White"], game.headers["Black"], game.headers["Result"], game.headers["WhiteElo"], game.headers["BlackElo"], game.headers["ECO"], str(game.mainline()))

            txt.write(query)
            txt.write(values)
            values=[]
    
            if not pgn:
                break



