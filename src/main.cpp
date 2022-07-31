#include <iostream>
#include "SnakeGame.hpp"

using namespace std;

int main(int argc, char *argv[]){

    if(argc == 1){
        std::cout << "Usage: ./snaze <level_file> -mode [snaze|pacmaze] -ia [random|find] \n        onde <level_file> é um arquivo txt contendo a descrição dos níveis no formato especificado neste documento \n        -mode [snaze|pacmaze] representa o modo do jogo, sendo possíveis dois valores snaze ou pacmaze \n        -ia [random|find] representa o modo da ia do jogo, sendo possível: 'random' onde a classe player \n             apenas escolhe decisões aleatorias, possívelmente não letais para a Snake/Pacman;\n             ou 'find' onde a classe player decide um caminho possível para a comida. \n        -d por fim o programa pode receber o argumento -d, que sinaliza à interface para exibir as decisões\n             calculadas pela classe player. Esse argumento é opcional." << std::endl;
        return 0;
    }

    /*Confere se o jogo está funcionando no mode debug*/
    bool debug_mode = false;

    if(argc == 7)
        debug_mode = true;

    SnakeGame game(argv[1], argv[3],argv[5],debug_mode);
    game.loop(); //bloqueia aqui e só saí quando o jogo termina

    return 0;
}