#include "../include/Player.hpp"

using namespace std;

Player::Player(){
    /*Inicializando o this->m_rastro*/
    for(auto a: this->m_rastro)
        for(auto b: a)
            b = false;
}

void Player::find_solution__aleatory(int x, int y){
    this->m_moves.clear(); 
    this->m_player_moves.clear();

    this->m_pacman->atualiza_posicao_pacman(x,y);
    
    if(encontrou(x,y))
        FOUND_A_WAY = true;

    if(FOUND_A_WAY){
        /*O pacman/snaze está em cima da comida*/
        this->m_moves.push_back(std::make_tuple(STAY,true,std::make_pair(x,y)));//só para printar no método my_journey, depois APAGUE
        this->m_player_moves.push_back(STAY);
    }else{
          /*Condicionais para conferir os limites do labirinto*/
        if(x != 0){
            if(this->m_maze_assembly.at(x-1).at(y) == 1){
                this->m_moves.push_back(std::make_tuple(UP,true,std::make_pair(x-1,y)));
            }else if(m_error_factor){
                this->m_moves.push_back(std::make_tuple(UP,true,std::make_pair(x-1,y)));
            }
        }
        if(y != 0){
            if(this->m_maze_assembly.at(x).at(y-1) == 1){
                this->m_moves.push_back(std::make_tuple(LEFT,true,std::make_pair(x,y-1)));
            }else if(m_error_factor){
                this->m_moves.push_back(std::make_tuple(LEFT,true,std::make_pair(x,y-1)));
            }
        }
        if(x != this->m_maze_assembly.size()){
            if(this->m_maze_assembly.at(x+1).at(y) == 1){
                this->m_moves.push_back(std::make_tuple(DOWN,true,std::make_pair(x+1,y)));
            }else if(m_error_factor){
                this->m_moves.push_back(std::make_tuple(DOWN,true,std::make_pair(x+1,y)));
            }
        }
        if(y != this->m_maze_assembly.at(0).size()){
            if(this->m_maze_assembly.at(x).at(y+1) == 1){
                this->m_moves.push_back(std::make_tuple(RIGHT,true,std::make_pair(x,y+1)));
            }else if(m_error_factor){
                this->m_moves.push_back(std::make_tuple(RIGHT,true,std::make_pair(x,y+1)));
            }
        }

        if(this->m_moves.size() ==0) //!Só entra aqui se ele for morrer
            this->m_player_moves.push_back(STAY);
        else{
            std::srand(std::time(nullptr)); 
            int ra_x = rand()%this->m_moves.size();
            this->m_player_moves.push_back(std::get<0>(this->m_moves.at(ra_x)));
        }

    }

}

int Player::next_aleatory_move(int x, int y){
    if(this->m_player_moves.size() == 0){
        this->find_solution__aleatory(x,y);
        return next_aleatory_move(x, y);
    }else{
         if(m_player_moves.at(0) == UP){
            m_player_moves.erase(m_player_moves.begin()+0);
            return 1;
        }else if(m_player_moves.at(0)  == DOWN){
            m_player_moves.erase(m_player_moves.begin()+0);
            return 2;
        }else if(m_player_moves.at(0)  == LEFT){
            m_player_moves.erase(m_player_moves.begin()+0);
            return 3;
        }else if(m_player_moves.at(0)  == RIGHT){
            m_player_moves.erase(m_player_moves.begin()+0);
            return 4;
        }else if(m_player_moves.at(0) == STAY){
            m_player_moves.erase(m_player_moves.begin()+0);
            m_player_moves.clear(); //Acabou,GAME_OVER
            return 0;
        }
    }
    return 0;
}


void Player::initialize_pacman(Pacman *pacman){
    this->m_pacman = pacman;
}

void Player::despedindo_pacman(){
    delete this->m_pacman;
}

void Player::mudando_mapa(){
    this->m_maze_assembly.clear();
}

void Player::recebe_mapa_assembly(std::vector<std::vector<int>> maze_assem){
    this->m_maze_assembly = maze_assem;
}


void Player::my_journey(){
    std::cout << "POSSO ESCOLHER ESSES: ";
    for(auto a: this->m_moves){
        if(std::get<0>(a) == UP){
            std::cout << " UP ";
        }else if(std::get<0>(a) == DOWN){
            std::cout << " DOWN ";
        }else if(std::get<0>(a) == LEFT){
            std::cout << " LEFT ";
        }else if(std::get<0>(a) == RIGHT){
            std::cout << " RIGHT ";
        }else if(std::get<0>(a) == STAY){
            std::cout << " STAY ";
        }
    }
    std::cout << std::endl;
}

std::pair<int,int> Player::get_posicao_jogador(){
    if(PACMAN_PLAYING){
        return this->m_pacman->get_posicao_pacman();
    }else if(SNAZE_PLAYING){ //enquanto ainda não há, eu não coloquei nada
        /*retornar a posição da snaze*/
    }
    return std::make_pair(0,0); //default
}

void Player::set_posicao_jogador(int x, int y){
    this->m_pacman->atualiza_posicao_pacman(x,y);
}

bool Player::encontrou(int x, int y){
    if((x == this->m_localiza_comida.first) && (y == this->m_localiza_comida.second))
        return true;
    else
        return false;
}

bool Player::permitido(int x, int y){
    if(m_maze_assembly.at(x).at(y) != 0)
        return true;
    else  
        return false;
}

bool Player::visitado(int x, int y){
    if(this->m_rastro.at(x).at(y) != true)
        return false;
    else if(this->m_rastro.at(x).at(y) == true) 
        return true;
}

void Player::resetar_find(){
    for(int i = 0; i < this->m_rastro.size();i++){
        for(int y = 0; y < this->m_rastro.at(0).size();y++){
            this->m_rastro.at(i).at(y) = false;
        }
    }
}


bool Player::find_solution(int x, int y){ 
    if (encontrou(x,y)){ //caso o pac esteja em cima da foods
        return true;
    }else{
        if(permitido(x,y+1) && !(visitado(x,y+1))){
            m_way.push_back(4);//4 == DIREITA
            this->m_rastro.at(x).at(y+1) = true; 
            if(find_solution(x,y+1))
                return true;
            else{
                m_way.pop_back();
                m_rastro.at(x).at(y+1) = false;
            }
        }
        if(permitido(x,y-1) && !(visitado(x,y-1))){
            m_way.push_back(3); //3 == ESQUERDA
            this->m_rastro.at(x).at(y-1) = true; 
            if(find_solution(x,y-1))
                return true;
            else{
                m_way.pop_back();
                m_rastro.at(x).at(y-1) = false;
            }
        }   
        if(permitido(x+1,y) && !(visitado(x+1,y))){//DOWN
            m_way.push_back(2); //2 = DOWN
            this->m_rastro.at(x+1).at(y) = true; 
            if(find_solution(x+1,y))
                return true;
            else{
                m_way.pop_back();
                m_rastro.at(x+1).at(y) = false;
            }
        }
        if(permitido(x-1,y) && !(visitado(x-1,y))){
            m_way.push_back(1); //1 == UP
            this->m_rastro.at(x-1).at(y) = true; 
            if(find_solution(x-1,y))
                return true;
            else{
                m_way.pop_back();
                m_rastro.at(x-1).at(y) = false;
            }
        }
    }
    return false;
}

void Player::atualiza_posicao_food(int x_comida, int y_comida){
    this->m_localiza_comida.first = x_comida;
    this->m_localiza_comida.second = y_comida;
} 

int Player::next_move(){
    if(this->m_way.size() == 0){
        this->m_way.clear();
        return 0;
    }else{
        auto retorno = this->m_way.at(0);
        this->m_way.erase(this->m_way.begin()+0);
        return retorno;
    }
}

void Player::find_solution_debug(std::vector<std::vector<int>> maze_map,int pos_food_X, int pos_food_y){
    std::vector<std::pair<int,int>> positions;

    maze_map.at(pos_food_X).at(pos_food_y) = 4;

    int x = this->m_pacman->get_posicao_pacman().first;
    int y = this->m_pacman->get_posicao_pacman().second;

    for(auto a: this->m_way){
        if(a == 1){ //UP
            if(maze_map.at(x-1).at(y) == 1){
                maze_map.at(x-1).at(y) = 5;
            }
            positions.push_back(std::make_pair(x-1,y));
            x -=1;
        }else if(a == 2){ //DOWN
            if(maze_map.at(x+1).at(y) == 1){
                maze_map.at(x+1).at(y) = 6;
            }
            positions.push_back(std::make_pair(x+1,y));
            x+=1;
        }else if(a == 3){ //LEFT
            if(maze_map.at(x).at(y-1) == 1){
                maze_map.at(x).at(y-1) = 7;
            }
            positions.push_back(std::make_pair(x,y-1));
            y-=1;
        }else if(a == 4){ //RIGHT
            if(maze_map.at(x).at(y+1) == 1){
                maze_map.at(x).at(y+1) = 8;
            }
            positions.push_back(std::make_pair(x,y+1));
            y+=1;
        }
    }

    for(int l = 0; l < maze_map.size(); l++){
        for(int c = 0; c < maze_map[0].size(); c++){
            if(maze_map.at(l).at(c) == 0){
                std::cout<<BG_GREEN ANSI_COLOR_BLACK "´" ANSI_COLOR_RESET BG_RESET;
            } else if(maze_map.at(l).at(c) == 1){
                std::cout<< ANSI_COLOR_BLACK " " ANSI_COLOR_RESET ;
            } else if(maze_map.at(l).at(c) == 4){
                std::cout<< ANSI_COLOR_RED "*" ANSI_COLOR_RESET;
            }else if(maze_map.at(l).at(c) == 5){//UP
                std::cout<< ANSI_COLOR_YELLOW << "^" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 6){//DOWN
                std::cout<< ANSI_COLOR_YELLOW << "v" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 7){//LEFT
                std::cout<< ANSI_COLOR_YELLOW << "<" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 8){//RIGHT
                std::cout<< ANSI_COLOR_YELLOW << ">" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 3){
                std::cout << ANSI_COLOR_RED << "o" <<ANSI_COLOR_RESET;
                // std::cout<<"😀" ;//eh_pacman
            }
        }
        std::cout<<"\n";
    }
}

void Player::set_last_move(int move){
    this->last_move = move;
}
int Player::get_last_move(){
    return this->last_move;
}

void Player::show_m_player_moves(){
   for(auto a: this->m_way){
        std::cout << a << " ";
   }
}