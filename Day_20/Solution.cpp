#include <cmath>
#include <cstdio>
#include <Advent.h>

struct Tile_t {
    Int64_t Id;
    Image_t Image;
    Bool_t Connected;
};

#define MAX_CAMERA_SIZE 12

struct Satellite_t {
    Int64_t Width;
    Int64_t Height;
    Int64_t TileWidth;
    Int64_t TileHeight;
    Tile_t* Tiles[MAX_CAMERA_SIZE][MAX_CAMERA_SIZE];
};

#define MAX_TILE_COUNT 256

struct Day20_t {
    Int64_t NumTiles;
    Tile_t Tiles[MAX_TILE_COUNT];

    Image_t Monster;
    Satellite_t Satellite;
};

Bool_t ConnectsOnTop(Tile_t* TileA, Tile_t* TileB) {
    return ConnectsTo(&TileA->Image, &TileB->Image, ConnectsOnTop);
}

Bool_t ConnectsOnLeft(Tile_t* TileA, Tile_t* TileB) {
    return ConnectsTo(&TileA->Image, &TileB->Image, ConnectsOnLeft);
}

Bool_t ConnectsOnRight(Tile_t* TileA, Tile_t* TileB) {
    return ConnectsTo(&TileA->Image, &TileB->Image, ConnectsOnRight);
}

Bool_t ConnectsOnBottom(Tile_t* TileA, Tile_t* TileB) {
    return ConnectsTo(&TileA->Image, &TileB->Image, ConnectsOnBottom);
}

void AssembleImage(Day20_t* Day) {
    Satellite_t* Satellite = &Day->Satellite;
    Satellite->Width = Truncate(sqrt(Day->NumTiles));
    Satellite->Height = Truncate(sqrt(Day->NumTiles));
    Satellite->TileWidth = Day->Tiles[0].Image.Width; // All tiles are the same width.
    Satellite->TileHeight = Day->Tiles[0].Image.Height; // All tiles are the same height.

    // @Incomplete: We assume that there is a corner tile that is already
    // oriented such that we can build down and to the right of it. If this is
    // not true, we will `Panic` below.

    // Find top left tile.
    for (Int64_t i = 0; i < Day->NumTiles; ++i) {
        Tile_t* TileA = &Day->Tiles[i];

        Tile_t* Top = NULL;
        Tile_t* Left = NULL;
        Tile_t* Right = NULL;
        Tile_t* Bottom = NULL;
        for (Int64_t j = 0; j < Day->NumTiles; ++j) {
            Tile_t* TileB = &Day->Tiles[j];
            if (TileA->Id == TileB->Id) {
                continue;
            }

            if (ConnectsOnTop(TileA, TileB)) {
                Top = TileB;
            } else if (ConnectsOnLeft(TileA, TileB)) {
                Left = TileB;
            } else if (ConnectsOnRight(TileA, TileB)) {
                Right = TileB;
            } else if (ConnectsOnBottom(TileA, TileB)) {
                Bottom = TileB;
            }
        }

        if (!Top && !Left && Right && Bottom) {
            TileA->Connected = true;
            Satellite->Tiles[0][0] = TileA;
            break;
        }
    }

    if (Satellite->Tiles[0][0] == NULL) {
        Panic("Could not find a top left tile.");
    }

    // Find adjacent tiles.
    for (Int64_t Y = 0; Y < Satellite->Height - 1; ++Y) {
        for (Int64_t X = 0; X < Satellite->Width - 1; ++X) {
            Tile_t* TileA = Satellite->Tiles[Y][X];

            for (Int64_t j = 0; j < Day->NumTiles; ++j) {
                Tile_t* TileB = &Day->Tiles[j];
                if (TileB->Connected) continue;

                if (ConnectsOnRight(TileA, TileB)) {
                    TileB->Connected = true;
                    Satellite->Tiles[Y][X + 1] = TileB;
                } else if (ConnectsOnBottom(TileA, TileB)) {
                    TileB->Connected = true;
                    Satellite->Tiles[Y + 1][X] = TileB;
                }
            }

            if (Satellite->Tiles[Y][X + 1] == NULL) {
                Panic("Could not find an adjacent tile.");
            } else if (Satellite->Tiles[Y + 1][X] == NULL) {
                Panic("Could not find an adjacent tile.");
            }
        }
    }

    Tile_t* TileA = Satellite->Tiles[Satellite->Height - 1][Satellite->Width - 2];
    Tile_t* TileB = Satellite->Tiles[Satellite->Height - 2][Satellite->Width - 1];

    // Find bottom right tile.
    for (Int64_t i = 0; i < Day->NumTiles; ++i) {
        Tile_t* TileC = &Day->Tiles[i];

        if (TileC->Connected == false) {
            if (ConnectsOnRight(TileA, TileC) && ConnectsOnBottom(TileB, TileC)) {
                    TileC->Connected = true;
                    Satellite->Tiles[Satellite->Height - 1][Satellite->Width - 1] = TileC;
            }
        }
    }

    if (Satellite->Tiles[Satellite->Height - 1][Satellite->Width - 1] == NULL) {
        Panic("Could not find the bottom right tile.");
    }
}

Int64_t SolvePart1(Day20_t* Day) {
    Satellite_t* Satellite = &Day->Satellite;
    return Satellite->Tiles[0][0]->Id
         * Satellite->Tiles[0][Satellite->Width - 1]->Id
         * Satellite->Tiles[Satellite->Height - 1][0]->Id
         * Satellite->Tiles[Satellite->Height - 1][Satellite->Width - 1]->Id;
}

void ReplaceMonsters(Image_t* Image, Image_t* Monster, Int64_t BaseX, Int64_t BaseY) {
    for (Int64_t Y = 0; Y < Monster->Height; ++Y) {
        for (Int64_t X = 0; X < Monster->Width; ++X) {
            if ((Monster->Pixels[Y][X] == '#')
                && (Image->Pixels[BaseY + Y][BaseX + X] == '.')) {
                    return; // We did not find a monster at this BaseX, BaseY.
            }
        }
    }

    for (Int64_t Y = 0; Y < Monster->Height; ++Y) {
        for (Int64_t X = 0; X < Monster->Width; ++X) {
            if (Monster->Pixels[Y][X] == '#') {
                Image->Pixels[BaseY + Y][BaseX + X] = '0';
            }
        }
    }
}

Int64_t ComputeRoughness(Image_t* Image, Image_t* Monster) {
    for (Int64_t Y = 0; Y < Image->Height - Monster->Height - 1; ++Y) {
        for (Int64_t X = 0; X < Image->Width - Monster->Width - 1; ++X) {
            // @Incomplete: We assume that there is only one orientation of the
            // Image where Monsters will be found. That orientation is the only
            // one where `ReplaceMonsters` above will actually modify the Image.
            ReplaceMonsters(Image, Monster, X, Y);
        }
    }

    Int64_t Roughness = 0;
    for (Int64_t Y = 0; Y < Image->Height; ++Y) {
        for (Int64_t X = 0; X < Image->Width; ++X) {
            if (Image->Pixels[Y][X] == '#') {
                Roughness += 1;
            }
        }
    }

    return Roughness;
}

Int64_t SolvePart2(Day20_t* Day) {
    Satellite_t* Satellite = &Day->Satellite;
    Int64_t InsideWidth = Satellite->TileWidth - 2;
    Int64_t InsideHeight = Satellite->TileHeight - 2;

    Image_t Image = {};
    Image.Width = Satellite->Width * InsideWidth;
    Image.Height = Satellite->Height * InsideHeight;
    for (Int64_t j = 0; j < Satellite->Height; ++j) {
        for (Int64_t i = 0; i < Satellite->Width; ++i) {
            Image_t* TileImage = &Satellite->Tiles[j][i]->Image;
            Blit(TileImage, 1, 1, &Image, i * InsideWidth, j * InsideHeight, InsideWidth, InsideHeight);
        }
    }

    Image_t* Monster = &Day->Monster;
    Int64_t MinRoughness = INT64_MAX;
    for (Int64_t Flips = 0; Flips < 2; ++Flips) {
        for (Int64_t Rotations = 0; Rotations < 4; ++Rotations) {
            Int64_t Roughness = ComputeRoughness(&Image, Monster);
            if (Roughness < MinRoughness) {
                MinRoughness = Roughness;
            }

            RotateClockwise(&Image);
        }

        FlipX(&Image);
    }

    return MinRoughness;
}

Bool_t LoadTiles(Day20_t* Day, Char_t const* File) {
    Input_t Input = {};
    if (!LoadInput(&Input, File)) {
        return false;
    }
    
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day->NumTiles += 1;
        Assert(Day->NumTiles < MAX_TILE_COUNT);
        Tile_t* Tile = &Day->Tiles[Day->NumTiles - 1];

        Offset += 5; // Eat the "Tile ".
        Offset += ScanForInt(Input.Data + Offset, &Tile->Id);
        Offset += 2; // Eat the colon and the newline.

        Int64_t GridX = 0;
        Int64_t GridY = 0;
        Image_t* Image = &Tile->Image;
        while (Input.Data[Offset] != '\n') {
            Image->Pixels[GridY][GridX] = Input.Data[Offset++];

            GridX += 1;
            if (GridX > Image->Width) {
                Image->Width = GridX;
            }

            if (Input.Data[Offset] == '\n') {
                GridX = 0;

                GridY += 1;
                if (GridY > Image->Height) {
                    Image->Height = GridY;
                }

                Offset += 1; // Eat the newline.
            }
        }

        Offset += 1; // Eat the newline.
    }

    return true;
}

Bool_t LoadMonster(Day20_t* Day, Char_t const* File) {
    Input_t Input = {};
    if (!LoadInput(&Input, File)) {
        return false;
    }

    Int64_t GridX = 0;
    Int64_t GridY = 0;
    Image_t* Monster = &Day->Monster;
    for (Int64_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            GridX = 0;
            
            GridY += 1;
            if (GridY > Monster->Height) {
                Monster->Height = GridY;
            }
        } else {
            Monster->Pixels[GridY][GridX] = Input.Data[i];

            GridX += 1;
            if (GridX > Monster->Width) {
                Monster->Width = GridX;
            }
        }
    }

    return true;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Day20_t* Day = new Day20_t();
    if (!LoadTiles(Day, "Data/Input.txt")) {
        return EXIT_FAILURE;
    }

    if (!LoadMonster(Day, "Data/Monster.txt")) {
        return EXIT_FAILURE;
    }

    AssembleImage(Day);

    printf("Part 1: %lld\n", SolvePart1(Day));
    printf("Part 2: %lld\n", SolvePart2(Day));

    return EXIT_SUCCESS;
}
