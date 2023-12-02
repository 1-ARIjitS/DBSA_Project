import os
import chess.pgn

def extract_pgn_from_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        while True:
            # Use chess.pgn.read_game to extract the PGN from the file
            game = chess.pgn.read_game(file)
            if not (("'" in game.headers["White"]) or ("'" in game.headers["Black"]) or ("'" in game.headers["Event"]) or ("'" in game.headers["Site"])):
                query = "INSERT INTO pgn (event, site, date, round, white, black, result, white_elo, black_elo, eco, san)"
                values =" VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s');\n" %(game.headers["Event"],game.headers["Site"], game.headers["Date"], game.headers["Round"], game.headers["White"], game.headers["Black"], game.headers["Result"], game.headers["WhiteElo"], game.headers["BlackElo"], game.headers["ECO"], str(game.mainline()))

                txt.write(query)
                txt.write(values)
                values=[]
            if not file:
                print("Games from "+ file +" uploaded.\n") 
                return

def process_directory(directory_path):
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)
        if os.path.isfile(file_path) and file_path.endswith('.pgn'):
            # Assuming the file has a .pgn extension
            chess_game = extract_pgn_from_file(file_path)
            if chess_game:
                print(f"PGN extracted from {filename}:\n{chess_game}\n")

# We open the file where are going to write everything  

txt = open('loading.sql','w')

# First, we create the table in postgres

txt.write("CREATE TABLE pgn(event VARCHAR(30),site VARCHAR(20),date VARCHAR(10),round VARCHAR(4),white VARCHAR(20),black VARCHAR(20),result VARCHAR(7),white_elo VARCHAR(5),black_elo VARCHAR(5),eco VARCHAR(3),san gamechess);\n")

# Replace 'your_directory_path' with the path to your directory containing PGN files
directory_path = 'Games/'
process_directory(directory_path)
