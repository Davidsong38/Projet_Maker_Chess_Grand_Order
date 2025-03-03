//
// Created by sebas on 24/01/2025.
//

#ifndef TEXTURES_H
#define TEXTURES_H

#include "Texture.h"

enum tex_type {
    TEX_FARID,
    TEX_LTR,
    TEX_FRAME_TEXTURE,
    TEX_BACKGROUND,
    TEX_BOARD,

    TEX_ARCUEID,
    TEX_ARTORIA,
    TEX_ASTOLFO,
    TEX_BBDUBAI,
    TEX_GILGAMESH,
    TEX_KIARA,
    TEX_KINTOKI,
    TEX_KUKULCAN,
    TEX_MASH,
    TEX_MEDUSA,
    TEX_MELUSINE,
    TEX_MERLIN,
    TEX_NEMO_MARX,
    TEX_NITOCRIS,
    TEX_NOBU,
    TEX_OBERON,
    TEX_OKITA,
    TEX_TANAMO,
    TEX_USHIWAKAMARU,
    TEX_WAVER,
    TEX_XUFU,
    TEX_ZHOU,

    TEX_EMPTY,
    TEX_WHITE,
    TEX_SCREEN_BLOCKER,
    TEX_SELECTION,

    TEX_COUNT
};

extern Texture* faridTexture;
extern Texture* ltrTexture;
extern Texture* frameTexture;
extern Texture* backgroundTexture;
extern Texture* boardTexture;

extern Texture* arcueidTexture;
extern Texture* artoriaTexture;
extern Texture* astolfoTexture;
extern Texture* bbdubaiTexture;
extern Texture* gilgameshTexture;
extern Texture* kiaraTexture;
extern Texture* kintokiTexture;
extern Texture* kukulcanTexture;
extern Texture* mashTexture;
extern Texture* medusaTexture;
extern Texture* melusineTexture;
extern Texture* merlinTexture;
extern Texture* nemoMarxTexture;
extern Texture* nitocrisTexture;
extern Texture* nobuTexture;
extern Texture* oberonTexture;
extern Texture* okitaTexture;
extern Texture* tanamoTexture;
extern Texture* ushiwakamaruTexture;
extern Texture* waverTexture;
extern Texture* xufuTexture;
extern Texture* zhouTexture;

extern Texture* emptyTexture;
extern Texture* whiteTexture;
extern Texture* screenBlockerTexture;
extern Texture* selectionTexture;

void loadTextures();

#endif //TEXTURES_H
