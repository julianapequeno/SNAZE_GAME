#include "Pacman.hpp"

Pacman::Pacman(std::pair<int,int> position_pacman, int pacman_life){
    this->m_pacman_positions.first = position_pacman.first;
    this->m_pacman_positions.second = position_pacman.second;
    this->m_pacman_life = pacman_life;
}

void Pacman::criado_com_sucesso(){
    std::cout << "Seu pacman foi localizado e inicializado com sucesso!! :)" << std::endl;
}

std::pair<int,int> Pacman::get_posicao_pacman(){
    return this->m_pacman_positions;
}

void Pacman::atualiza_posicao_pacman(int x, int y){
    this->m_pacman_positions.first = x;
    this->m_pacman_positions.second = y;
}

int Pacman::get_food_pacman(){
    return m_food_pacman;
}

void Pacman::set_food_pacman(){
    this->m_food_pacman++;
}

int Pacman::get_pacman_live(){
    return m_pacman_life;
}

char Pacman::movement_pacman_side(std::string move){
    if(move.compare("UP")==0){
        return '^';
    }else if(move.compare("DOWN")==0){
        return 'v';
    }else if(move.compare("LEFT")==0){
        return '<';
    }else if(move.compare("RIGHT")==0){
        return '>';
    }
}

void Pacman::loss_of_life(){
    this->m_pacman_life = m_pacman_life - 1;
}