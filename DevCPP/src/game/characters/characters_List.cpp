//
// Created by david on 30/01/2025.
//

#include "Characters_List.h"

#include <utility>

string Characters_List_to_string[CHARACTERS_COUNT];
void loadCharactersList() {
    Characters_List_to_string[MEDUSA_SABER]  = "Medusa Saber";
    Characters_List_to_string[MERLIN] = "Merlin";
    Characters_List_to_string[ASTOLFO] = "Astolfo";
    Characters_List_to_string[TAMAMO_NO_MAE] = "Tamamo No Mae";
    Characters_List_to_string[USHIWAKAMARU] = "Ushiwakamaru";
    Characters_List_to_string[OKITA] = "Okita";
    Characters_List_to_string[KINTOKI_RIDER] = "Kintoki Rider";
    Characters_List_to_string[ZHOU_YU] = "Zhou Yu";
    Characters_List_to_string[ARCEUID] = "Arceuid";
    Characters_List_to_string[XU_FU] = "Xu Fu";
    Characters_List_to_string[MELUSINE] = "Mélusine";
    Characters_List_to_string[MASH] = "Mash";
    Characters_List_to_string[SESSHOIN_KIARA] = "Sesshoin Kiara";
    Characters_List_to_string[NITOCRIS_ALTER] = "Nitocris Alter";
    Characters_List_to_string[BB_DUBAI] = "BB Dubai";
    Characters_List_to_string[KUKULKAN] = "Kukulkan";
    Characters_List_to_string[ARTORIA] = "Artoria";
    Characters_List_to_string[GILGAMESH] = "Gilgamesh";
    Characters_List_to_string[OBERON] = "Oberon";
    Characters_List_to_string[ZHUGE_LIANG] = "Zhuge Liang";
    Characters_List_to_string[NEMO_MARINE] = "Nemo Marine";
    Characters_List_to_string[NOBU] = "Nobu";
}

//Character_Instance::Character_Instance(Characters_List character,Pieces_List piece_origin)
//    :character(character), piece_origin(piece_origin), piece_name(Characters_List_to_string[character]) {}