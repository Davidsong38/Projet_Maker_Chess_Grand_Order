//
// Created by david on 30/01/2025.
//

#ifndef CHARACTERS_LIST_H
#define CHARACTERS_LIST_H
#include <string>

using namespace std;

enum Characters_List {
    MEDUSA_SABER,
    MERLIN,
    ASTOLFO,
    TAMAMO_NO_MAE,
    USHIWAKAMARU,
    OKITA,
    KINTOKI_RIDER,
    ZHOU_YU,
    ARCEUID,
    XU_FU,
    MELUSINE,
    MASH,
    SESSHOIN_KIARA,
    NITOCRIS_ALTER,
    BB_DUBAI,
    KUKULKAN,
    ARTORIA,
    GILGAMESH,
    OBERON,
    ZHUGE_LIANG,
    NEMO_MARINE,
    NOBU,
    CHARACTERS_COUNT,

};

extern string Characters_List_to_string[CHARACTERS_COUNT];
void loadCharactersList();

/*struct Character_Instance {
    Characters_List character;
    Pieces_List piece_origin;
    string piece_name;

    explicit Character_Instance(Characters_List character,Pieces_List piece_origin);

    [[nodiscard]] Characters_List getCharacter() const {
        return character;
    }

    [[nodiscard]] Pieces_List getPieceOrigin() const {
        return piece_origin;
    }

    [[nodiscard]] string getPieceName() const {
        return piece_name;
    }
};*/



#endif //CHARACTERS_LIST_H
