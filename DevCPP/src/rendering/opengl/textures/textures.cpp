//
// Created by sebas on 24/01/2025.
//

#include "textures.h"

#include "RenderEngine.h"
#include "glFunctions.h"

Texture* faridTexture;
Texture* ltrTexture;
Texture* frameTexture;
Texture* backgroundTexture;
Texture* boardTexture;

Texture* arcueidTexture;
Texture* artoriaTexture;
Texture* astolfoTexture;
Texture* bbdubaiTexture;
Texture* gilgameshTexture;
Texture* kiaraTexture;
Texture* kintoshiTexture;
Texture* kukulcanTexture;
Texture* mashTexture;
Texture* medusaTexture;
Texture* melusineTexture;
Texture* merlinTexture;
Texture* nemoMarxTexture;
Texture* nitocrisTexture;
Texture* nobuTexture;
Texture* oberonTexture;
Texture* okitaTexture;
Texture* tanamoTexture;
Texture* ushiwakamaruTexture;
Texture* waverTexture;
Texture* xufuTexture;
Texture* zhouTexture;

std::string texNames[TEX_COUNT];

void loadTextures() {
    GLuint texture_ids[TEX_COUNT];
    glGenTextures(TEX_COUNT, texture_ids);

    faridTexture = new Texture(texture_ids[TEX_FARID], "farid.png", GL_RGBA);
    ltrTexture = new Texture(texture_ids[TEX_LTR], "LTR.png", GL_RGBA);
    frameTexture = new Texture(texture_ids[TEX_FRAME_TEXTURE], RenderEngine::getMaxWindowSize(), GL_RGB);
    backgroundTexture = new Texture(texture_ids[TEX_BACKGROUND], "background.jpg", GL_RGB);
    boardTexture = new Texture(texture_ids[TEX_BOARD], "board.png", GL_RGBA);

    arcueidTexture = new Texture(texture_ids[TEX_ARCUEID],"arcueid.png",GL_RGBA);
    artoriaTexture = new Texture(texture_ids[TEX_ARTORIA],"artoria.png",GL_RGBA);
    astolfoTexture = new Texture(texture_ids[TEX_ASTOLFO],"astolfo.png",GL_RGBA);
    bbdubaiTexture = new Texture(texture_ids[TEX_BBDUBAI],"bbdubai.png",GL_RGBA);
    gilgameshTexture = new Texture(texture_ids[TEX_GILGAMESH],"gilgamesh.png",GL_RGBA);
    kiaraTexture = new Texture(texture_ids[TEX_KIARA],"kiara.png",GL_RGBA);
    kintoshiTexture = new Texture(texture_ids[TEX_KINTOKI],"kintoshi.png",GL_RGBA);
    kukulcanTexture = new Texture(texture_ids[TEX_KUKULCAN],"kukulcan.png",GL_RGBA);
    mashTexture = new Texture(texture_ids[TEX_MASH],"mash.png",GL_RGBA);
    medusaTexture = new Texture(texture_ids[TEX_MEDUSA],"medusa.png",GL_RGBA);
    melusineTexture = new Texture(texture_ids[TEX_MELUSINE],"melusine.png",GL_RGBA);
    merlinTexture = new Texture(texture_ids[TEX_MERLIN],"merlin.png",GL_RGBA);
    nemoMarxTexture = new Texture(texture_ids[TEX_NEMO_MARX],"nemo_marx.png",GL_RGBA);
    nitocrisTexture = new Texture(texture_ids[TEX_NITOCRIS],"nitocris.png",GL_RGBA);
    nobuTexture = new Texture(texture_ids[TEX_NOBU],"nobu.png",GL_RGBA);
    oberonTexture = new Texture(texture_ids[TEX_OBERON],"oberon.png",GL_RGBA);
    okitaTexture = new Texture(texture_ids[TEX_OKITA],"okita.png",GL_RGBA);
    tanamoTexture = new Texture(texture_ids[TEX_TANAMO],"tanamo.png",GL_RGBA);
    ushiwakamaruTexture = new Texture(texture_ids[TEX_USHIWAKAMARU],"ushiwakamaru.png",GL_RGBA);
    waverTexture = new Texture(texture_ids[TEX_WAVER],"waver.png",GL_RGBA);
    xufuTexture = new Texture(texture_ids[TEX_XUFU],"xufu.png",GL_RGBA);
    zhouTexture = new Texture(texture_ids[TEX_ZHOU],"zhou.png",GL_RGBA);
}

void loadTextureNames() {
    texNames[TEX_FARID] = "farid.png";
    texNames[TEX_LTR] = "LTR.png";
    texNames[TEX_FRAME_TEXTURE] = "frame.png";
}