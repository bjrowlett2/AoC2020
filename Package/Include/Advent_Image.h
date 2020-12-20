#ifndef Advent_Image_h
#define Advent_Image_h 1

#include "Advent_Types.h"

#define MAX_IMAGE_SIZE 256

struct Image_t {
    Int64_t Width;
    Int64_t Height;
    Char_t Pixels[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];
};

void FlipX(Image_t* Image) {
    for (Int64_t Y = 0; Y < Image->Height; ++Y) {
        for (Int64_t X = 0; X < Image->Width / 2; ++X) {
            Char_t Temporary = Image->Pixels[Y][X];
            Image->Pixels[Y][X] = Image->Pixels[Y][Image->Width - X - 1];
            Image->Pixels[Y][Image->Width - X - 1] = Temporary;
        }
    }
}

void RotateClockwise(Image_t* Image) {
    Assert(Image->Width == Image->Height);

    Char_t Copy[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE] = {};
    for (Int64_t Y = 0; Y < Image->Height; ++Y) {
        for (Int64_t X = 0; X < Image->Width; ++X) {
            Copy[Y][X] = Image->Pixels[Y][X];
        }
    }

    for (Int64_t Y = 0; Y < Image->Height; ++Y) {
        for (Int64_t X = 0; X < Image->Width; ++X) {
            Image->Pixels[Y][X] = Copy[X][Image->Width - Y - 1];
        }
    }
}

typedef Bool_t ConnectsOn_t(Image_t* ImageA, Image_t* ImageB);

Bool_t ConnectsTo(Image_t* ImageA, Image_t* ImageB, ConnectsOn_t* ConnectsOnFn) {
    for (Int64_t Flips = 0; Flips < 2; ++Flips) {
        for (Int64_t Rotations = 0; Rotations < 4; ++Rotations) {
            if (ConnectsOnFn(ImageA, ImageB)) {
                return true;
            }

            RotateClockwise(ImageB);
        }

        FlipX(ImageB);
    }

    return false;
}

Bool_t ConnectsOnTop(Image_t* ImageA, Image_t* ImageB) {
    if (ImageA->Width != ImageB->Width) {
        return false;
    }

    for (Int64_t X = 0; X < ImageA->Width; ++X) {
        if (ImageA->Pixels[0][X] != ImageB->Pixels[ImageB->Height - 1][X]) {
            return false;
        }
    }

    return true;
}

Bool_t ConnectsOnLeft(Image_t* ImageA, Image_t* ImageB) {
    if (ImageA->Height != ImageB->Height) {
        return false;
    }

    for (Int64_t Y = 0; Y < ImageA->Height; ++Y) {
        if (ImageA->Pixels[Y][0] != ImageB->Pixels[Y][ImageB->Width - 1]) {
            return false;
        }
    }

    return true;
}

Bool_t ConnectsOnRight(Image_t* ImageA, Image_t* ImageB) {
    if (ImageA->Height != ImageB->Height) {
        return false;
    }

    for (Int64_t Y = 0; Y < ImageA->Height; ++Y) {
        if (ImageA->Pixels[Y][ImageA->Width - 1] != ImageB->Pixels[Y][0]) {
            return false;
        }
    }

    return true;
}

Bool_t ConnectsOnBottom(Image_t* ImageA, Image_t* ImageB) {
    if (ImageA->Width != ImageB->Width) {
        return false;
    }

    for (Int64_t X = 0; X < ImageA->Width; ++X) {
        if (ImageA->Pixels[ImageA->Height - 1][X] != ImageB->Pixels[0][X]) {
            return false;
        }
    }

    return true;
}

void Blit(Image_t* SrcImage, Int64_t SrcX, Int64_t SrcY, Image_t* DestImage, Int64_t DestX, Int64_t DestY, Int64_t Width, Int64_t Height) {
    for (Int64_t Y = 0; Y < Height; ++Y) {
        for (Int64_t X = 0; X < Width; ++X) {
            DestImage->Pixels[DestY + Y][DestX + X] = SrcImage->Pixels[SrcY + Y][SrcX + X];
        }
    }
}

#endif // Advent_Image_h
