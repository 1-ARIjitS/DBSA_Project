from bintrees import BTree

def create_btree():
    # Create a BTree
    btree = BTree()

    # Insert 10 different SAN notation chess games into the BTree
    chess_games = {
        "game1": "e4 e5 Nf3 Nc6 Bb5 a6 Ba4 Nf6 O-O Be7 Re1 b5 Bb3 d6 c3 O-O",
        "game2": "d4 Nf6 c4 e6 Nf3 b6 g3 Ba6 b3 Bb4+ Bd2 Be7 Bg2 c6 O-O O-O",
        "game3": "e4 c5 Nf3 d6 d4 cxd4 Nxd4 Nf6 Nc3 a6 Be3 e5 Nb3 Be6 f3 h5",
        "game4": "d4 d5 c4 c6 Nf3 Nf6 Nc3 e6 Bg5 h6 Bh4 dxc4 e4 g5 Bg3 b5",
        "game5": "e4 e6 d4 d5 Nd2 Nf6 e5 Nfd7 Bd3 c5 c3 Nc6 Ne2 cxd4 cxd4 f6",
        "game6": "c4 e5 Nc3 Nf6 Nf3 Nc6 g3 Bb4 Bg2 O-O O-O e4 Ng5 Bxc3 bxc3 Re8",
        "game7": "d4 Nf6 c4 e6 Nf3 b6 g3 Ba6 b3 Bb4+ Bd2 Be7 Bg2 c6 O-O O-O",
        "game8": "e4 c5 Nf3 e6 d4 cxd4 Nxd4 a6 Nc3 Qc7 Be3 Nf6 f3 Bb4 Qd2 d5",
        "game9": "e4 e5 Nf3 Nc6 Bb5 a6 Ba4 Nf6 O-O Be7 Re1 b5 Bb3 d6 c3 O-O",
        "game10": "d4 Nf6 c4 g6 Nc3 Bg7 e4 d6 Be2 O-O Be3 e5 d5 Nbd7 g4 Nc5 f3 a5"
    }
    for game_id, moves in chess_games.items():
        btree.insert(game_id, moves)

    return btree


def find_with_prefix(btree, prefix):
    # Initialize an empty list to store strings with the given prefix
    result = []

    # Find the first key in the tree that is greater than or equal to the prefix
    key = btree.ceiling_item(prefix)[0] if btree.ceiling_item(prefix) else None

    # Iterate through the tree starting from this key
    while key is not None and key.startswith(prefix):
        # Add the key to the result if it starts with the prefix
        result.append(btree[key])

        # Move to the next key in the tree
        key = btree.succ_item(key)[0] if btree.succ_item(key) else None

    return result

# Create the B-tree
btree = create_btree()

# Find and print all strings that start with 'abc'
prefix = 'e4'
strings_with_prefix = find_with_prefix(btree, prefix)
print(f"Strings that start with '{prefix}': {strings_with_prefix}")