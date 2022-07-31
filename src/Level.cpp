#include "../include/Level.hpp"


int Level::_level = 0;

Level::Level(string name_file, int size_line, int size_col, int amount_food){
    m_amount_foods = amount_food;
    m_name_file = name_file;
    m_size_col = size_col;
    m_size_line = size_line;
    Level::_level++;
    process_map();
}


Level::Level(){

}

std::pair<int, int> Level::get_pacman_position_found(){
    return m_pacman_position_found;
}

std::vector<std::vector<int>> Level::get_process_map(){
    this->m_maze_map = std::vector< std::vector<int>> (m_size_line, std::vector<int>(m_size_col));
    for(int l = 0; l < m_size_line; l++){
        for(int c = 0; c < m_size_col; c++){
            if(m_container_map.at(l).at(c) == 'X'){
                this->m_maze_map.at(l).at(c) = 0;
            } else if(m_container_map.at(l).at(c) == ' '){
                    this->m_maze_map.at(l).at(c) = 1;
                    this->m_coord_free.push_back(std::make_pair(l, c));
            } else {
                if(this->_level == 1 && this->m_round_atual == 0){
                    //!Se estiver no nível 1 e o ele estiver no round 0, ou seja, apenas na primeira vez
                    this->m_maze_map.at(l).at(c) = 3; //No caso é o pacman
                    m_pacman_position_found.first = l;
                    m_pacman_position_found.second = c;
                }else{
                    this->m_maze_map.at(l).at(c) = 1; 
                }
                this->m_status_pacman = true;
            }     
        }
    }
    return m_maze_map;
}

void Level::process_map(){
    ifstream levelFile(this->m_name_file); 
    int lineCount = 0;
    string line;
   
    if(levelFile.is_open()){
        while(getline(levelFile, line)){
            if(lineCount > 0){
                m_container_map.push_back(line);
            }
            lineCount++;
        }
    } else {
        std::cout<<"Arquivo de jogo não encontrado"<<std::endl;
    }
}

bool Level::get_status_pacman(){
    return m_status_pacman;
}

std::vector<std::vector<int>> Level::update_assembler_maze(std::pair<int, int> position_pacman, std::pair<int, int> position_food, bool debug, int move){ 

    if(debug){ //!Caso esteja rodando o DEBUG
        if(position_pacman.first >= m_maze_map.size() || position_pacman.second >= m_maze_map.at(0).size() || position_pacman.first  < 0 || position_pacman.second < 0){
            /*Modo Random limites no labirinto*/
            if(position_pacman.first >= m_maze_map.size())
                    position_pacman.first = m_maze_map.size()-1;
            else if(position_pacman.first < 0)
                    position_pacman.first = 1;
                
            if(position_pacman.second >=  m_maze_map.at(0).size())
                    position_pacman.second = m_maze_map.at(0).size()-1;
            else if(position_pacman.second < 0)
                    position_pacman.second = 1;

            if(m_maze_map.at(position_pacman.first).at(position_pacman.second) == 0){
                   status_pacman_maze = false; 
            }
            
            m_maze_map.at(position_pacman.first).at(position_pacman.second) = 3;
            m_maze_map.at(position_food.first).at(position_food.second) = 4;

        }else{ //!Caso o random esteja dentro dos limites do labirinto
            if(m_maze_map.at(position_pacman.first).at(position_pacman.second) == 0){
                   status_pacman_maze = false; 
                   m_maze_map.at(position_pacman.first).at(position_pacman.second) = -1; //!Bateu na parede
            }else{
                m_maze_map.at(position_pacman.first).at(position_pacman.second) = 3;
                m_maze_map.at(position_food.first).at(position_food.second) = 4;
                status_pacman_maze = true;
                if(move == 1){
                    m_maze_map.at(position_pacman.first+1).at(position_pacman.second) = 11;
                } else if(move == 2){
                    m_maze_map.at(position_pacman.first-1).at(position_pacman.second) = 12;
                } else if(move == 3){
                    m_maze_map.at(position_pacman.first).at(position_pacman.second+1) = 13;
                } else if(move == 4){
                    m_maze_map.at(position_pacman.first).at(position_pacman.second-1) = 14;
                }
            }   
        }

    }else{//!Normal
        m_maze_map.at(m_pacman_position_found.first).at(m_pacman_position_found.second) = 1;
        if(m_maze_map.at(position_pacman.first).at(position_pacman.second) == 0){ //ele não se meche
            status_pacman_maze = false;
        } else if(m_maze_map.at(position_pacman.first).at(position_pacman.second) == 4){
            m_maze_map.at(position_food.first).at(position_food.second) = 3; //Sobressair o pacman 
            m_pacman_position_found.first = position_pacman.first;
            m_pacman_position_found.second = position_pacman.second;
            status_pacman_maze = true;
        } else {
            status_pacman_maze = true;
            m_maze_map.at(position_pacman.first).at(position_pacman.second) = 3;
            m_pacman_position_found.first = position_pacman.first;
            m_pacman_position_found.second = position_pacman.second;
            m_maze_map.at(position_food.first).at(position_food.second) = 4;  
        }    
    }
    return m_maze_map;
}

bool Level::status_player_maze(){
    return status_pacman_maze;
}

bool Level::end_of_level(int amount_delicious){
    this->m_round_atual = m_amount_foods;
    return amount_delicious == m_amount_foods ? true : false;
}

std::vector<std::pair<int,int>> Level::get_coords_free(){
    return m_coord_free;
}

void Level::set_coords_food(std::pair<int,int> c_free){
    this->m_coords_food.push_back(c_free);
}

float Level::increment_score(int number_of_steps){
    return (100*1)/((number_of_steps/50)+1);
}

void Level::reset_food_dispo(){
    m_amount_foods = m_amount_foods - 1;
}

void Level::reset_round(){
    m_round_atual = 0;
}

void Level::set_pacman_position_found_new_level(std::pair<int,int> pac_pos){
    this->m_pacman_position_found = pac_pos;
}