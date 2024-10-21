# Chess Database Project

INFOH417 - Database Systems Architecture (2023/24) - ULB <br />
Team: Simon Coessens 🇧🇪, Ben Gold 🇺🇸, Nils Van Es Ostos 🇪🇸, Arijit Samal 🇮🇳

<div align="center">
    <img src="https://actus.ulb.be/medias/photo/logo-universite-libre-bruxelles_1661952138925-png?ID_FICHE=19524" alt="ULB Logo" width="300"/>
</div>

<br>
This project aims to develop a PostgreSQL extension for storing and retrieving chess games, utilizing data types and functions based on common chess notations (PGN, SAN, FEN). The extension will support efficient queries on chess game data with custom indexing.

## Meeting Summaries

- **November 6, 2023**

  - **Tasks**: Arijit & Nils started implementing data types in C, Simon worked on `getBoard` and `getFirstMoves` functions, and Ben focused on `hasOpening` and `hasBoard` functions.
  - **Discussions**: Agreed on using FEN for `chessboard` and SAN for `chessgame`. Reviewed algorithms for SAN to FEN conversion.

- **November 13, 2023**

  - **Progress Review**: Checked status of data type implementations and function development. Addressed challenges in SAN to FEN conversion and discussed smallchesslib integration.
  - **Next Steps**: Planned indexing strategies and set tasks for the next phase.

- **November 21, 2023**
  - **Focus**: Started integrating indexing with function logic. Discussed optimizations.
