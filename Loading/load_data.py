import os
from chess import pgn
import psycopg2

def extract_pgn_from_file(file_path):
    cont=0
    with open(file_path, 'r', encoding='utf-8') as file:
        print("Reading file " + file_path + "...")
        while (cont<900):
            # Use chess.pgn.read_game to extract the PGN from the file
            game = pgn.read_game(file)
            if not (("'" in game.headers["White"]) or ("'" in game.headers["Black"]) or ("'" in game.headers["Event"]) or ("'" in game.headers["Site"])):
                query = """INSERT INTO pgn (event, site, date, round, white, black, result, white_elo, black_elo, eco, san)
                  VALUES ('{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}');\n""".format(
                    game.headers.get("Event", ""), game.headers.get("Site", ""),
                    game.headers.get("Date", ""), game.headers.get("Round", ""),
                    game.headers.get("White", ""), game.headers.get("Black", ""),
                    game.headers.get("Result", ""), game.headers.get("WhiteElo", ""),
                    game.headers.get("BlackElo", ""), game.headers.get("ECO", ""),
                    game.mainline()
                )
                #establishing the connection
                conn = psycopg2.connect(
                database="chess", user='postgres', password='conejolibra899', host='127.0.0.1', port= '5432'
                )
                #Creating a cursor object using the cursor() method
                cursor = conn.cursor()

                #Executing an MYSQL function using the execute() method and commiting
                cursor.execute(query)

                conn.commit()

                #Closing the connection
                conn.close()

                #txt.write(query)
                #txt.write(values)
                cont=cont+1
                values=[]

def process_directory(directory_path):
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)
        if os.path.isfile(file_path) and file_path.endswith('.pgn'):
            # Assuming the file has a .pgn extension
            chess_game = extract_pgn_from_file(file_path)

# We open the file where are going to write everything  

txt = open('loading.sql','w')

# First, we create the table in postgres

#establishing the connection
conn = psycopg2.connect(
database="chess", user='postgres', password='conejolibra899', host='127.0.0.1', port= '5432'
)
#Creating a cursor object using the cursor() method
cursor = conn.cursor()

#Executing an MYSQL function using the execute() method and commiting
drop="DROP TABLE IF EXISTS pgn;"
cursor.execute(drop)
create="CREATE TABLE pgn(event VARCHAR(40),site VARCHAR(40),date VARCHAR(10),round VARCHAR(10),white VARCHAR(40),black VARCHAR(40),result VARCHAR(7),white_elo VARCHAR(5),black_elo VARCHAR(5),eco VARCHAR(3),san chessgame);\n"
cursor.execute(create)

conn.commit()

# Replace 'your_directory_path' with the path to your directory containing PGN files
directory_path = 'Games/'
process_directory(directory_path)